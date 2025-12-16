#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/dect2020ext-mac-pdu-helper.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-association-request-message-ie.h"
#include "ns3/dect2020ext-unicast-header.h"
#include "ns3/dect2020ext-association-response-message-ie.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"
#include "ns3/pointer.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
    m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
}

Dect2020ExtMac::~Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtMac::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMac")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtMac>()

                                .AddAttribute("config_BandNumber",
                                              "DECT-2020 band number configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_bandNumber),
                                              MakeUintegerChecker<uint8_t>(1, 21))

                                .AddAttribute("config_SubcarrierScalingFactor",
                                              "DECT-2020 subcarrier scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_subcarrierScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8))

                                .AddAttribute("config_FourierTransformScalingFactor",
                                              "DECT-2020 Fourier transform scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_fourierTransformScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8))

                                .AddAttribute("config_ChannelManager",
                                              "Dect2020ExtChannelManager configuration",
                                              PointerValue(nullptr),
                                              MakePointerAccessor(&Dect2020ExtMac::m_channelManager),
                                              MakePointerChecker<Dect2020ExtChannelManager>());
    return tid;
}

void
Dect2020ExtMac::InitializeDevice(void)
{
    NS_LOG_FUNCTION(this);
    SetLongRadioDeviceId(GenerateLongRadioDeviceId());
    SetShortRadioDeviceId(GenerateShortRadioDeviceId());
}
void
Dect2020ExtMac::Start(void)
{
    if (m_channelManager == nullptr)
    {
        NS_LOG_ERROR("Channel manager is not set for MAC");
    }
    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        NS_LOG_INFO("FT Device " << this << " starts the network initialization");
        InitializeNetwork();
    }
    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
    {
        NS_LOG_INFO("PT Device " << this << " starts the network discovery");
        DiscoverNetworks();
    }
}

void
Dect2020ExtMac::DiscoverNetworks(void)
{
    NS_LOG_FUNCTION(this);
    if (m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING ||
        m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT)
    {
        return; // Device is in association process --> abort discovering networks
    }

    std::vector<Ptr<Dect2020ExtChannel>> channelList = m_channelManager->GetValidChannels(this->GetBandNumber());

    if (m_associatedFTNetDeviceLongRdId != 0)
    {
        return; // Device is already associated --> abort
    }

    // Check if there are any valid channels
    if (channelList.empty())
    {
        NS_LOG_WARN("No valid channels available for band " << (uint32_t)this->GetBandNumber());
        return;
    }

    // find the current channel
    uint32_t current = m_currentChannelId;
    uint32_t nextChannelId = 0;

    // search if current + 1 is part of the channel list
    bool found = false;
    for (auto& ch : channelList)
    {
        if (ch->m_channelId == current + 1)
        {
            nextChannelId = ch->m_channelId;
            found = true;
            break;
        }
    }

    if (!found)
    {
        // no valid next channel -> take the first one of the band
        nextChannelId = channelList.front()->m_channelId;
    }

    // Switch to the next channel
    NS_LOG_LOGIC("Switching to channel " << nextChannelId);
    SetCurrentChannelId(nextChannelId);

    Time t = MilliSeconds(10); // Discover Network wait time
    Simulator::Schedule(t, &Dect2020ExtMac::DiscoverNetworks, this); // Schedule next discovery
}

void
Dect2020ExtMac::InitializeNetwork(void)
{
    uint32_t networkId = GenerateValidNetworkID();
    SetNetworkId(networkId);

    Simulator::Schedule(Seconds(1), &Dect2020ExtMac::ScheduleBackgroundScan, this, Seconds(0));
}

uint32_t
Dect2020ExtMac::GenerateValidNetworkID(void)
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint32_t networkId = 0;
    uint8_t lsb;
    uint32_t msb;
    do
    {
        networkId = randomVar->GetValue(1, std::numeric_limits<uint32_t>::max());
        lsb = networkId & 0xFF;
        msb = (networkId >> 8) & 0xFFFFFF;
    } while (lsb == 0x00 || msb == 0x000000);

    return networkId;
}

// TODO: Mitigate collision of long radio device IDs
uint32_t
Dect2020ExtMac::GenerateLongRadioDeviceId(void)
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint32_t rdId = 0;

    do
    {
        rdId = randomVar->GetValue(1, 0xFFFFFFFD); // Range 0x00000001 bis 0xFFFFFFFD
    } while (rdId == 0x00000000 || rdId == 0xFFFFFFFE || rdId == 0xFFFFFFFF);

    NS_LOG_DEBUG(Simulator::Now().GetMilliSeconds()
                 << ": Long Radio Device ID " << rdId << "generated.");

    return rdId;
}

uint16_t
Dect2020ExtMac::GenerateShortRadioDeviceId(void)
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint16_t rdId = 0;

    do
    {
        rdId = randomVar->GetValue(1, 0xFFFE); // Range 0x0001 to 0xFFFE
    } while (rdId == 0x0000 || rdId == 0xFFFF);

    NS_LOG_DEBUG(Simulator::Now().GetMilliSeconds()
                 << ": Short Radio Device ID " << rdId << "generated.");

    return rdId;
}

void
Dect2020ExtMac::EvaluateAllChannels(void)
{
    // TODO: Review this function and ensure that types are correct and that the logic is correct.
    NS_LOG_FUNCTION(this);
    const auto rssiResults = m_rssiByChannel;
    auto channels = m_channelManager->GetValidChannels(1);
    
    for (auto channel : channels)  // channel is Ptr<Dect2020ExtChannel>, so this is OK
    {
        auto rssiIt = rssiResults.find(channel->m_channelId);
        if (rssiIt != rssiResults.end())
        {
            auto rssiResultsIt = rssiIt->second.begin();
            
            for (auto& slot : channel->m_slots)  // REFERENCE to slot
            {
                for (auto& subslot : slot.subslots)  // REFERENCE to subslot
                {
                    if (rssiResultsIt != rssiIt->second.end())
                    {
                        if (*rssiResultsIt <= RSSI_THRESHOLD_MIN)
                        {
                            subslot.status = SubslotStatus::FREE;
                        }
                        if (*rssiResultsIt > RSSI_THRESHOLD_MIN && *rssiResultsIt <= RSSI_THRESHOLD_MAX)
                        {
                            subslot.status = SubslotStatus::POSSIBLE;
                        }
                        if (*rssiResultsIt > RSSI_THRESHOLD_MAX)
                        {
                            subslot.status = SubslotStatus::BUSY;
                        }
                        subslot.rssi = *rssiResultsIt;  // Now modifying the ORIGINAL
                        rssiResultsIt++;
                    }
                }
            }
        }
        else
        {
            NS_LOG_WARN("No RSSI results found for channel " << channel->m_channelId);
        }
    }
    SelectOperatingChannel();
}

SubslotStatus
Dect2020ExtMac::EvaluateSubslot(double rssiDbm)
{
    if (rssiDbm <= RSSI_THRESHOLD_MIN)
    {
        return SubslotStatus::FREE;
    }
    if (rssiDbm > RSSI_THRESHOLD_MIN && rssiDbm <= RSSI_THRESHOLD_MAX)
    {
        return SubslotStatus::POSSIBLE;
    }
    if (rssiDbm > RSSI_THRESHOLD_MAX)
    {
        return SubslotStatus::BUSY;
    }
    return SubslotStatus::BUSY;
}

void
Dect2020ExtMac::DisplayChannelEvaluations(void)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_DEBUG("============ Channel Evaluations Summary ============");
    
    // Display all channel evaluations
    if (m_channelEvaluations.empty())
    {
        NS_FATAL_ERROR("No channel evaluations available yet at: " << Simulator::Now().GetMilliSeconds());
        return;
    }
    
    // Display header
    NS_LOG_DEBUG("Channel ID | Free | Possible | Busy | Total | Free %");
    NS_LOG_DEBUG("-----------------------------------------------------");
    
    // Display each channel evaluation
    for (const auto& eval : m_channelEvaluations)
    {
        uint32_t total = eval.Total();
        double freePercentage = (total > 0) ? (eval.free * 100.0 / total) : 0.0;
        
        std::ostringstream oss;
        oss << std::setw(10) << eval.channelId << " | "
            << std::setw(4) << eval.free << " | "
            << std::setw(8) << eval.possible << " | "
            << std::setw(4) << eval.busy << " | "
            << std::setw(5) << total << " | "
            << std::fixed << std::setprecision(1) << std::setw(5) << freePercentage << "%";
        
        NS_LOG_DEBUG(oss.str());
    }
    
    NS_LOG_DEBUG("====================================================");
}

void
Dect2020ExtMac::SelectOperatingChannel(void)
{
    NS_LOG_FUNCTION(this);
    DisplayChannelEvaluations();

    // Sort by free subslots and select the best channel
    auto sortedEvaluations = m_channelEvaluations;
    auto SortByFreeSubslotsLambda = [](const Dect2020ExtChannelEvaluation& a, const Dect2020ExtChannelEvaluation& b)
    {
        return a.free > b.free;
    };
    
    std::stable_sort(sortedEvaluations.begin(), sortedEvaluations.end(), SortByFreeSubslotsLambda);
    
    if (!sortedEvaluations.empty())
    {
        m_operatingChannelId = sortedEvaluations[0].channelId;
        NS_LOG_INFO("Selected Operating Channel: " << m_operatingChannelId 
                    << " with " << sortedEvaluations[0].free << " free subslots");

        NS_LOG_LOGIC("Selected Operating Channel: " << m_operatingChannelId 
                    << " with " << sortedEvaluations[0].free << " free subslots");
    }
    else
    {
        NS_LOG_WARN("No channel evaluations available for selection");
    }
    m_operatingChannelId = sortedEvaluations[0].channelId;
    m_currentChannelId = m_operatingChannelId;
    StartBeaconTransmission();
}

/**
 * @brief Schedule the background scan with the given delay.
 * 
 * Connect the OnRssiMeasurement callback to RssiMeasured Trace Callback. 
 * Then, schedule the background scan with the given delay.
 */
void
Dect2020ExtMac::ScheduleBackgroundScan(Time delay)
{
    NS_LOG_FUNCTION(this);

    // Connect the OnRssiMeasurement callback to RssiMeasured Trace Callback
    m_channelManager->TraceConnectWithoutContext("RssiMeasured", MakeCallback(&Dect2020ExtMac::OnRssiMeasurement, this));

    // Schedule the background scan
    Simulator::Schedule(delay, &Dect2020ExtMac::BackgroundScan, this);
}

void
Dect2020ExtMac::OnRssiMeasurement(uint16_t channelId, double rssiDbm, std::shared_ptr<Dect2020ExtSubslotScanContext> context)
{
    context->subslotCount++;
    NS_LOG_FUNCTION(this << channelId << rssiDbm << Simulator::Now().GetSeconds());

    auto dectChannels = m_channelManager->GetChannels();

    SubslotStatus currentSubslotStatus = EvaluateSubslot(rssiDbm);
    if (currentSubslotStatus == SubslotStatus::FREE)
    {
        context->evaluation.free++;
    }
    else if (currentSubslotStatus == SubslotStatus::POSSIBLE)
    {
        context->evaluation.possible++;
    }
    else if (currentSubslotStatus == SubslotStatus::BUSY)
    {
        context->evaluation.busy++;
    }
    
    if (context->subslotCount == context->totalSubslots)
    {
        NS_LOG_DEBUG("Adding channel evaluation for channel " << context->channelId);
        m_channelEvaluations.push_back(context->evaluation);
    }

    if (m_channelEvaluations.size() == dectChannels.size())
    {
        SelectOperatingChannel();
    }
}

void
Dect2020ExtMac::MeasureRSSIOnChannel(Ptr<Dect2020ExtChannel> channel, int64_t frameDurationNs, uint32_t numSubslots, std::shared_ptr<Dect2020ExtSubslotScanContext> context)
{
    NS_LOG_FUNCTION(this << channel->m_channelId);
    uint32_t subslotIndex = 0;

    for (auto& slot : channel->m_slots)
    {
        for (auto& _ : slot.subslots)
        {
            // Calculate delay based on proportion of frame duration to avoid rounding accumulation
            int64_t subslotRssiMeasurementDelayNs = (frameDurationNs * subslotIndex) / numSubslots;
            int64_t subslotDurationNs = (frameDurationNs * (subslotIndex + 1)) / numSubslots - subslotRssiMeasurementDelayNs;

            Simulator::Schedule(NanoSeconds(subslotRssiMeasurementDelayNs), 
                              static_cast<void (Dect2020ExtChannelManager::*)(std::shared_ptr<Dect2020ExtSubslotScanContext>, TimeValue)>(&Dect2020ExtChannelManager::MeasureRssi), 
                              m_channelManager, context, TimeValue(NanoSeconds(subslotDurationNs)));
            subslotIndex++;
        }
    }
}

void
Dect2020ExtMac::BackgroundScan(void)
{
    NS_LOG_FUNCTION(this);
    const uint32_t slotsPerFrame = 24; // #ETSI TS 103 636-3 V1.5.1, Section 4.4
    const int64_t frameDurationNs = 10000000; // 10 milliseconds in nanoseconds
    uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    const uint32_t totalSubslotsPerFrame = slotsPerFrame * subslotsPerSlot;
    std::vector<Ptr<Dect2020ExtChannel>> dectChannels = m_channelManager->GetChannels();
    
    int64_t channelRssiMeasurementDelayNs = 0;
    for (auto channel : dectChannels)
    {
        auto context = std::make_shared<Dect2020ExtSubslotScanContext>();
        context->channelId = channel->m_channelId;
        context->evaluation.channelId = channel->m_channelId;
        context->totalSubslots = totalSubslotsPerFrame;

        // Schedule the RSSI measurement for each channel with 10ms period
        Simulator::Schedule(NanoSeconds(channelRssiMeasurementDelayNs), &Dect2020ExtMac::MeasureRSSIOnChannel, this, channel, frameDurationNs, totalSubslotsPerFrame, context);
        channelRssiMeasurementDelayNs += frameDurationNs;
    }
}

void
Dect2020ExtMac::StartBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_LOGIC("Starting Network and Cluster Beacon transmission on channel (operating channel)" << m_currentChannelId);
    StartNetworkBeaconTransmission();
    StartClusterBeaconTransmission();
}

void
Dect2020ExtMac::StartNetworkBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);

    // Schedule the network beacon transmission immediately (or with small offset)
    Time initialOffset = MilliSeconds(0);
    Simulator::Schedule(initialOffset, &Dect2020ExtMac::SendNetworkBeaconOnChannel, this, m_currentChannelId);

    // Schedule the next transmission with dynamic offset
    static uint32_t sweepCount = 0;
    Time dynamicOffset = MilliSeconds(25 + (sweepCount++ % 3) * 2); // 25, 27, 29ms
    Time basePeriod = MilliSeconds(80); // Base period to get 105, 107, 109ms total

    NS_LOG_DEBUG("Dynamic offset: " << dynamicOffset.GetSeconds());
    NS_LOG_DEBUG("Next network beacon transmission scheduled with delay: " << (basePeriod + dynamicOffset).GetSeconds());

    // Schedule next transmission from when the actual beacon is sent (dynamicOffset + period)
    // This ensures the gap between actual transmissions is always networkBeaconPeriod
    Simulator::Schedule(basePeriod + dynamicOffset, &Dect2020ExtMac::StartNetworkBeaconTransmission, this);
}

void
Dect2020ExtMac::StartClusterBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
    Time clusterBeaconPeriod = m_clusterBeaconPeriodTime; // gap between each transmission
    Simulator::Schedule(m_clusterBeaconPeriodTime, &Dect2020ExtMac::SendClusterBeaconOnChannel, this, m_operatingChannelId);

    // Schedule the next cluster beacon transmission
    Simulator::Schedule(clusterBeaconPeriod, &Dect2020ExtMac::StartClusterBeaconTransmission, this);
}

void
Dect2020ExtMac::SendNetworkBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    NS_LOG_DEBUG(this);
    Ptr<Packet> networkBeacon = BuildNetworkBeacon();

    NS_LOG_INFO("Sending Network Beacon on channel " << m_currentChannelId);
    m_phy->Send(networkBeacon, BuildPhysicalHeaderField(1, networkBeacon->GetSize()));
}

void
Dect2020ExtMac::SendClusterBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> packet = BuildClusterBeacon(channelId);

    NS_LOG_INFO("Sending Cluster Beacon on channel " << m_currentChannelId);
    m_phy->Send(packet, BuildPhysicalHeaderField(1, packet->GetSize()));
}

Ptr<Packet>
Dect2020ExtMac::BuildNetworkBeacon(void)
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> beacon = Create<Packet>();
    
    // Network Beacon Message IE
    Dect2020ExtNetworkBeaconMessageIE networkBeaconMessageIE;
    networkBeaconMessageIE.SetTxPowerIncluded(1);
    networkBeaconMessageIE.SetPowerConstraints(0);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(0);
    networkBeaconMessageIE.SetNetworkBeaconChannels(0);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    networkBeaconMessageIE.SetNextClusterChannel(m_operatingChannelId);

    Time timeToNext = m_nextNetworkBeaconTime - Simulator::Now();
    networkBeaconMessageIE.SetTimeToNext(timeToNext.GetMilliSeconds());

    beacon->AddHeader(networkBeaconMessageIE);

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    beacon->AddHeader(mxHeaderTypeA);

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(m_networkId);
    beaconHeader.SetTransmitterAddress(m_longRadioDeviceId);

    beacon->AddHeader(beaconHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    beacon->AddHeader(macHeaderType);

    return beacon;
}

Ptr<Packet>
Dect2020ExtMac::BuildClusterBeacon(uint32_t clusterChannelId)
{
    NS_LOG_FUNCTION(this);
    Ptr<Packet> beacon = Create<Packet>();

    // MAC Multiplexing Header Type A
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::CLUSTER_BEACON_MESSAGE);

    // Cluster beacon message is always followed by the Random Access Ressource IE
    Dect2020ExtRandomAccessResourceIE randomAccessResourceIE = BuildRandomAccessResourceIE();
    beacon->AddHeader(randomAccessResourceIE);
    
    // Cluster Beacon Message IE
    Dect2020ExtClusterBeaconMessageIE clusterBeaconMessageIE;
    clusterBeaconMessageIE.SetSFN(m_phy->GetCurrentSFN());
    clusterBeaconMessageIE.SetTxPowerIncluded(0);
    clusterBeaconMessageIE.SetPowerConstraints(0);
    clusterBeaconMessageIE.SetFrameOffsetIncluded(0);
    clusterBeaconMessageIE.SetNextChannelIncluded(0);
    clusterBeaconMessageIE.SetTimeToNextFieldPresent(0);
    clusterBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    clusterBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    clusterBeaconMessageIE.SetCountToTrigger(15);
    clusterBeaconMessageIE.SetRelQuality(0);
    clusterBeaconMessageIE.SetMinQuality(0);

    beacon->AddHeader(clusterBeaconMessageIE);

    beacon->AddHeader(mxHeaderTypeA);

    // Beacon Header
    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(m_networkId);
    beaconHeader.SetTransmitterAddress(m_longRadioDeviceId);
    beacon->AddHeader(beaconHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);
    beacon->AddHeader(macHeaderType);

    return beacon;
}

Dect2020ExtRandomAccessResourceIE
Dect2020ExtMac::BuildRandomAccessResourceIE(void)
{
    NS_LOG_FUNCTION(this);
    Dect2020ExtRandomAccessResourceIE randomAccessResourceIE;
    randomAccessResourceIE.SetRepeat(0);
    randomAccessResourceIE.SetSystemFrameNumberFieldIncluded(0);
    randomAccessResourceIE.SetChannelFieldIncluded(0);
    randomAccessResourceIE.SetSeparateChannelFieldIncluded(0);

    uint8_t m_startSubslot = CalculateAbsoluteStartSubslot();
    randomAccessResourceIE.SetStartSubslot(m_startSubslot);
    randomAccessResourceIE.SetLengthType(0); // length in subslots
    randomAccessResourceIE.SetRaraLength(m_device->m_randomAccessResourceLength);
    randomAccessResourceIE.SetMaxRachLengthType(0); // length in subslots
    randomAccessResourceIE.SetMaxRachLength(2);     // Max 2  subslots / transmission
    randomAccessResourceIE.SetCwMinSig(0);          // CW min 0 --> backoff not yet implemented
    randomAccessResourceIE.SetDectDelay(m_dectDelay);
    randomAccessResourceIE.SetResponseWindow(5);
    randomAccessResourceIE.SetCwMaxSig(0); // CW max 0 --> backoff not yet implemented

    return randomAccessResourceIE;
}

uint8_t
Dect2020ExtMac::CalculateAbsoluteStartSubslot(void)
{
    NS_LOG_FUNCTION(this);
    uint8_t absoluteStartSubslot = m_nextAvailableSubslot;
    uint8_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;

    // If the next available subslot is greater than the number of subslots per frame,
    // wrap around to the beginning of the frame
    if (absoluteStartSubslot >= subslotsPerFrame)
    {
        absoluteStartSubslot = absoluteStartSubslot % subslotsPerFrame;
        m_nextAvailableSubslot = absoluteStartSubslot;
    }

    m_nextAvailableSubslot += 2;

    return absoluteStartSubslot;
}

Dect2020ExtPHYControlFieldType1
Dect2020ExtMac::BuildPhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes)
{
    Dect2020ExtPHYControlFieldType1 physicalHeaderField;
    physicalHeaderField.SetPacketLengthType(packetLengthType); // 0 = in Subslots, 1 = in Slots

    if (packetLengthType == 0) // Packet length is given in subslots
    {
        // TBD
    }
    else if (packetLengthType == 1) // Packet length is given in slots
    {
        double packetLengthBits = packetLengthBytes * 8;
        double mcsTransportBlockSizeBits =
            this->m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor,
                                                  m_fourierTransformScalingFactor,
                                                  m_mcs);
        uint16_t packetLengthInSlots = 0;
        packetLengthInSlots = std::ceil(packetLengthBits / mcsTransportBlockSizeBits);

        physicalHeaderField.SetPacketLength(
            packetLengthInSlots); // Size of packet in slots/subslots
    }

    physicalHeaderField.SetShortNetworkID(m_networkId);
    physicalHeaderField.SetTransmitterIdentity(m_shortRadioDeviceId);
    physicalHeaderField.SetTransmitPower(23); // Dbm -- device power class 1
    physicalHeaderField.SetDFMCS(m_mcs);

    return physicalHeaderField;
}

void
Dect2020ExtMac::ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm)
{
    NS_LOG_FUNCTION(this << packet);
    NS_LOG_DEBUG("Received packet from PHY with RSSI " << rssiDbm << " dBm on channel " << m_currentChannelId);

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField;
    packet->RemoveHeader(physicalHeaderField);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    packet->RemoveHeader(macHeaderType);

    if (macHeaderType.GetMacHeaderTypeField() ==
        Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER)
    {
        // If the candidate has sent a beacon message before, we can update its entry in the FT candidate list
        Dect2020ExtFtCandidateInfo* ft = FindOrCreateFtCandidate(physicalHeaderField.GetTransmitterIdentity());
        ft->ftPhyHeaderField = physicalHeaderField;
        ft->receptionTime = Simulator::Now();

        HandleBeaconPacket(packet, ft);
    }

    else if (macHeaderType.GetMacHeaderTypeField() == Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER)
    {
        NS_LOG_INFO("Received Unicast Packet");
        HandleUnicastPacket(packet);
    }

    // Forward the packet to the net device for minimal implementation
    m_device->Receive(packet);

    // m_rxPacketTrace(packet);
}

void
Dect2020ExtMac::HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this << packet);

    // --- Beacon Header ---
    Dect2020ExtBeaconHeader beaconHeader;
    packet->RemoveHeader(beaconHeader);
    ft->ftBeaconHeader = beaconHeader;
    ft->networkId = beaconHeader.GetNetworkId();
    ft->longFtId = beaconHeader.GetTransmitterAddress();

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    packet->RemoveHeader(mxHeader);
    m_lastFtMacMxHeader = mxHeader;
    Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding ieType = mxHeader.GetIeTypeFieldEncoding();

    // --- Network Beacon Message ---
    if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE)
    {
        NS_LOG_INFO("Received Network Beacon Message");
        // --- Network Beacon Message IE ---
        Dect2020ExtNetworkBeaconMessageIE networkBeaconMessage;
        packet->RemoveHeader(networkBeaconMessage);
        ft->ftNetworkBeaconMessage = networkBeaconMessage;

        // Get the cluster channel id from the network beacon message
        uint16_t ftBeaconNextClusterChannel = networkBeaconMessage.GetNextClusterChannel();
        ft->clusterChannelId = ftBeaconNextClusterChannel;

        // if this RD is not on the cluster channel, and the association status is not associated
        // nor association pending, and the device is not currently waiting for a Cluster Beacon
        // (has recently switched the channel because it received a network beacon), switch to the
        // cluster channel.
        if (GetCurrentChannelId() != ftBeaconNextClusterChannel &&
            m_associationStatus != ASSOCIATED && m_associationStatus != ASSOCIATION_PENDING &&
            !m_isWaitingForClusterBeacon)
        {
            m_isWaitingForClusterBeacon = true;

            m_operatingChannelId = ftBeaconNextClusterChannel;
        }
    }
    // --- Cluster Beacon Message ---
    else if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::CLUSTER_BEACON_MESSAGE)
    {
        NS_LOG_INFO("Received Cluster Beacon Message");
        // Cluster Beacon received --> Reset the flag
        m_isWaitingForClusterBeacon = false;

        // if the PT has received a Cluster Beacon and no Network Beacon before, we assume the
        // cluster Channel is the current Channel
        if (ft->ftNetworkBeaconMessage.GetNextClusterChannel() == 0 &&
            !ft->ftClusterBeaconMessage.GetNextChannelIncluded())
        {
            ft->ftNetworkBeaconMessage.SetNextClusterChannel(this->m_currentChannelId);
        }

        Dect2020ExtClusterBeaconMessageIE clusterBeaconMessage;
        packet->RemoveHeader(clusterBeaconMessage);
        ft->ftClusterBeaconMessage = clusterBeaconMessage;

        Dect2020ExtRandomAccessResourceIE randomAccessResourceIE;
        // packet->RemoveHeader(randomAccessResourceIE);
        // ft->ftRandomAccessResourceIE = randomAccessResourceIE;

        EvaluateClusterBeacon(clusterBeaconMessage, randomAccessResourceIE, ft);
    }
}

void
Dect2020ExtMac::HandleUnicastPacket(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    packet->RemoveHeader(unicastHeader);

    // Check if the Unicast Message is for this device
    uint32_t receiverAddress = unicastHeader.GetReceiverAddress();
    uint32_t transmitterAddress = unicastHeader.GetTransmitterAddress();

    if (receiverAddress != this->GetLongRadioDeviceId())
    {
        return; // Packet is not for this device
    }

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    packet->RemoveHeader(mxHeader);

    Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding ieType = mxHeader.GetIeTypeFieldEncoding();

    // Message is an association request
    if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE)
    {
        NS_LOG_INFO("Received Association Request");
        // check whether this device is an FT --> a PT cannot process an association request
        if (m_device->GetTerminationPointType() !=
            Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
        {
            NS_LOG_WARN("Association Request received, but this device is not an FT.");
            return;
        }

        // Strip the message headers
        Dect2020ExtAssociationRequestMessageIE associationRequestMessage;
        packet->RemoveHeader(associationRequestMessage);

        // Dect2020RdCapabilityIE rdCapabilityIE;
        // packet->RemoveHeader(rdCapabilityIE);

        // Dect2020AssociationControlIE associationControlIE;
        // packet->RemoveHeader(associationControlIE);

        // process the association request
        // ProcessAssociationRequest(associationRequestMessage,
        //                           rdCapabilityIE,
        //                           associationControlIE,
        //                           transmitterAddress);

        ProcessAssociationRequest(associationRequestMessage, transmitterAddress);
    }
    // Message is an association response
    else if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE)
    {
        // check whether this device is an PT --> TODO: implement associationable FTs
        if (m_device->GetTerminationPointType() !=
            Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
        {
            NS_LOG_WARN("Association Response received, but this device is not an PT. FT "
                        "association not yet implemented.");
            return;
        }

    //     // // Strip the message headers
        Dect2020ExtAssociationResponseMessageIE associationResponseMessage;
        packet->RemoveHeader(associationResponseMessage);

    //     // Dect2020RdCapabilityIE rdCapabilityIE;
    //     // packet->RemoveHeader(rdCapabilityIE);

        ProcessAssociationResponse(associationResponseMessage, transmitterAddress);
    }
}

Dect2020ExtFtCandidateInfo*
Dect2020ExtMac::FindOrCreateFtCandidate(uint16_t shortFtId)
{
    NS_LOG_FUNCTION(this << shortFtId);
    // for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    // {
    //     if (m_ftCandidates[idx].shortFtId == shortFtId)
    //     {
    //         return &m_ftCandidates[idx];
    //     }
    // }

    // No candidate found, create one
    Dect2020ExtFtCandidateInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

void
Dect2020ExtMac::EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIE& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIE& randomAccessResourceIE, Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Evaluating Cluster Beacon");
    // ft->sfn = clusterBeaconMessage.GetSFN();
    // m_lastSfn = clusterBeaconMessage.GetSFN();

    // if device is not associated, switch the status to preapring association and start the "select
    // best FT" timer
    if (m_associationStatus == Dect2020ExtAssociationStatus::NOT_ASSOCIATED)
    {
    //     // This is the time in seconds for the PT to listen to other channels to collect potential
    //     // FT candidates
    //     Time searchBestFtTimeSeconds = Seconds(2);

    //     // association status preparing means the device is scanning the network to find the best FT
    //     // candidate
        m_associationStatus = Dect2020ExtAssociationStatus::ASSOCIATION_PREPARING;
    //     DiscoverNetworks();
    //     Simulator::Schedule(searchBestFtTimeSeconds, &Dect2020ExtMac::SelectBestFtCandidate, this);
    }

    // prepare association request --> The device is waiting fot the cluster Beacon of the selected
    // FT
    if (m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT)// &&
        // m_selectedFtCandidate.longFtId == ft->longFtId)
    {
        // Calculate the time from the Random Access Resource IE to send the association request
        // uint16_t txSubslot = 0;
        // uint16_t startSubslot = randomAccessResourceIE.GetStartSubslot();
        // bool lengthInSubslots = !randomAccessResourceIE.GetLengthType();
        // uint8_t length = randomAccessResourceIE.GetRaraLength();

        // Select a random subslot to reduce the propability of collision
        // std::random_device rd;
        // std::mt19937 gen(rd());
        // if (lengthInSubslots)
        // {
        //     std::uniform_int_distribution<> dis(startSubslot, startSubslot + length - 1);
        //     txSubslot = dis(gen);

        //     if (txSubslot >= m_channelManager->GetSubslotsPerSlot() * 24)
        //     {
        //         txSubslot = txSubslot % (m_channelManager->GetSubslotsPerSlot() * 24);
        //         m_lastSfn++;
        //     }
        // }
        // else
        // {
        //     // tbd --> Length in Slots
        // }

        // uint8_t slot = txSubslot / m_channelManager->GetSubslotsPerSlot();
        // uint8_t subslot = txSubslot % m_channelManager->GetSubslotsPerSlot();

        // Time t = m_phy->GetTimeToNextAbsoluteSubslot(m_lastSfn, slot, subslot);

        // Schedule the Association Request to be sent in the next subslot
    }
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendAssociationRequest, this, ft);
}

// void
// Dect2020ExtMac::SelectBestFtCandidate(void)
// {
//     NS_LOG_FUNCTION(this);

//     // Step 1: Find the FT with the best (highest) RSSI-2
//     double bestRssiDbm = -150.0; // start with very low RSSI
//     Dect2020ExtFtCandidateInfo bestCandidate;
//     bool found = false;

//     for (const auto& ft : m_ftCandidates)
//     {
//         if (ft.rssiDbm > bestRssiDbm)
//         {
//             bestRssiDbm = ft.rssiDbm;
//             NS_LOG_INFO("DEBUG: ft.clusterChannelId == " << ft.clusterChannelId);
//             NS_LOG_INFO("DEBUG: ft.clusterChannelId == "
//                         << ft.ftNetworkBeaconMessage.GetNextClusterChannel());
//             bestCandidate = ft;
//             found = true;
//         }
//     }

//     if (!found)
//     {
//         NS_LOG_WARN("No valid FT candidate found. Staying in NOT_ASSOCIATED state.");
//         m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
//         return;
//     }

//     // Step 2: Save the best FT info
//     m_selectedFtCandidate = bestCandidate;

//     // Step 3: Switch to the FT's channel
//     m_operatingChannelId = bestCandidate.clusterChannelId;

//     // Step 4: Set status and wait for next cluster beacon of selected FT
//     m_associationStatus = Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT;

//     // Cluster Beacon triggers next step in EvaluateClusterBeacon
//     // Verify after 2 Seconds if the device is still waiting for the selected FT. This should not
//     // be the case because it should have received a Cluster Beacon and sent an Association Request
//     // and the status is either associated or association pending.
//     Simulator::Schedule(Seconds(2),
//                         &Dect2020ExtMac::VerifyWaitingForSelectedFtAssociationStatus,
//                         this);
// }

void
Dect2020ExtMac::VerifyWaitingForSelectedFtAssociationStatus()
{
    if (this->m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT)
    {
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
}

void
Dect2020ExtMac::SendAssociationRequest(Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this << ft);
    NS_LOG_INFO("Sending Association Request on channel " << m_currentChannelId);
    // Set the Association Status to ASSOCIATION_PENDING

    m_associationStatus = Dect2020ExtAssociationStatus::ASSOCIATION_PENDING;

    Simulator::Schedule(MilliSeconds(200),
                        &Dect2020ExtMac::VerifyPendingAssociationStatus,
                        this); // Verify the association status after 200 ms

    Ptr<Packet> packet = Create<Packet>();

    // --- RD Capability IE ---
    // Dect2020RdCapabilityIE rdCapabilityIE;
    // rdCapabilityIE.SetRelease(this->m_device->m_release);
    // rdCapabilityIE.SetGroupAssignment(this->m_device->m_supportGroupAssignment);
    // rdCapabilityIE.SetPaging(this->m_device->m_supportPaging);
    // rdCapabilityIE.SetOperatingModes(this->m_device->m_operatingModes);
    // rdCapabilityIE.SetMesh(this->m_device->m_mesh);
    // rdCapabilityIE.SetScheduledAccessDataTransfer(this->m_device->m_scheduledAccessDataTransfer);
    // rdCapabilityIE.SetMacSecurity(this->m_device->m_macSecurity);
    // rdCapabilityIE.SetDlcServiceType(this->m_device->m_dlcServiceType);
    // rdCapabilityIE.SetRdPowerClass(this->m_device->m_rdPowerClass);
    // rdCapabilityIE.SetMaxNssFoRx(this->m_device->m_maxNssFoRx);
    // rdCapabilityIE.SetRxForTxDiversity(this->m_device->m_rxForTxDiversity);
    // rdCapabilityIE.SetRxGain(this->m_device->m_rxGain);
    // rdCapabilityIE.SetMaxMcs(this->m_device->m_maxMcs);
    // rdCapabilityIE.SetSoftBufferSize(this->m_device->m_softBufferSize);
    // rdCapabilityIE.SetNumOfHarqProcesses(this->m_device->m_numOfHarqProcesses);
    // rdCapabilityIE.SetHarqFeedbackDelay(this->m_device->m_harqFeedbackDelay);
    // rdCapabilityIE.SetDDelay(this->m_device->m_dDelay);
    // rdCapabilityIE.SetHalfDulp(this->m_device->m_halfDulp);

    // packet->AddHeader(rdCapabilityIE);

    // --- Association Request Message ---
    Dect2020ExtAssociationRequestMessageIE associationRequestMessage;
    associationRequestMessage.SetSetupCause(0); // Initial association
    associationRequestMessage.SetNumberOfFlows(0);
    associationRequestMessage.SetPowerConstraints(0);
    associationRequestMessage.SetFtMode(0);
    associationRequestMessage.SetCurrent(0);
    associationRequestMessage.SetHarqProcessesTx(0);
    associationRequestMessage.SetMaxHarqReTxDelay(0);
    associationRequestMessage.SetHarqProcessesRx(0);
    associationRequestMessage.SetMaxHarqReRxDelay(0);
    associationRequestMessage.SetFlowId(0);

    packet->AddHeader(associationRequestMessage);

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA muxHeader;
    muxHeader.SetMacExtensionFieldEncoding(0);
    muxHeader.SetLengthField(0);
    muxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE);

    packet->AddHeader(muxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(0);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(ft->longFtId);
    unicastHeader.SetTransmitterAddress(this->GetLongRadioDeviceId());

    packet->AddHeader(unicastHeader);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER);

    packet->AddHeader(macHeaderType);

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField = BuildPhysicalHeaderField(1, packet->GetSize());
    physicalHeaderField.SetShortNetworkID(m_potentialShortNetworkId);

    m_phy->Send(packet, physicalHeaderField); // Send the packet to the PHY
}

void
Dect2020ExtMac::VerifyPendingAssociationStatus(void)
{
    if (this->m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING)
    {
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
}

void
Dect2020ExtMac::ProcessAssociationRequest(const Dect2020ExtAssociationRequestMessageIE& associationRequestMessage, uint32_t transmitterAddress)
{
    NS_LOG_FUNCTION(this << associationRequestMessage << transmitterAddress);
    NS_LOG_INFO("Processing Association Request");

    Dect2020ExtAssociatedPtInfo ptInfo;
    ptInfo.associationEstablished = Simulator::Now();
    ptInfo.longRdId = transmitterAddress;
    // ptInfo.numberOfFlows = associationRequestMessage.GetNumberOfFlows();
    // ptInfo.powerConstraints = associationRequestMessage.GetPowerConstraints();
    // ptInfo.ftMode = associationRequestMessage.GetFtMode();
    // ptInfo.harqProcessesTx = associationRequestMessage.GetHarqProcessesTx();
    // ptInfo.maxHarqReTxDelay = associationRequestMessage.GetMaxHarqReTxDelay();
    // ptInfo.harqProcessesRx = associationRequestMessage.GetHarqProcessesRx();
    // ptInfo.maxHarqReRxDelay = associationRequestMessage.GetMaxHarqReRxDelay();
    // ptInfo.flowId = associationRequestMessage.GetFlowId();
    // // ptInfo.cb_m = assoControlIe.GetClusterBeaconMonitoring() == 1 ? true : false;
    // // ptInfo.rdCapabilityIe = rdCapabilityIe;

    m_associatedPtDevices.push_back(ptInfo);

    // // TODO: implement logic to accept or reject device. Currently every device is accepted

    // // calculate the time to send the association response. DECT_Delay == 0 --> subslot n +
    // // HARQ_feedback_delay +1
    // uint32_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;

    // uint32_t currentAbsSubslot = m_phy->GetCurrentAbsoluteSubslot();

    // uint8_t harqDelay = rdCapabilityIe.GetHarqFeedbackDelay();
    // uint32_t responseAbsSubslot;

    // if (m_dectDelay == 0)
    // {
    //     responseAbsSubslot = currentAbsSubslot + harqDelay + 1;
    // }
    // else
    // {
    //     responseAbsSubslot = currentAbsSubslot + (subslotsPerFrame / 2); // 0.5 frame later
    // }

    // Time t = CalculcateTimeOffsetFromCurrentSubslot(responseAbsSubslot - currentAbsSubslot);

    // NS_LOG_INFO(Simulator::Now().GetMicroSeconds()
    //             << ": Dect2020Mac::ProcessAssociationRequest: Scheduling Association Response to 0x"
    //             << std::hex << ptInfo.longRdId << std::dec << " in "
    //             << (responseAbsSubslot - currentAbsSubslot) << " subslots and in "
    //             << t.GetMicroSeconds() << " us. Current AbsSubslot == " << currentAbsSubslot
    //             << " & response Subslot == " << responseAbsSubslot);

    Simulator::Schedule(MilliSeconds(0), &Dect2020ExtMac::SendAssociationResponse, this, ptInfo);
}

void
Dect2020ExtMac::SendAssociationResponse(Dect2020ExtAssociatedPtInfo ptInfo)
{
    // TODO: Add RD Capability IE
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Sending Association Response");

    Ptr<Packet> packet = Create<Packet>();

    // --- Association Response Message ---
    Dect2020ExtAssociationResponseMessageIE associationResponseMessage;
    associationResponseMessage.SetAssociationAccepted(true);
    associationResponseMessage.SetHarqMod(0);
    associationResponseMessage.SetNumberOfFlows(7); // all flows are accepted
    associationResponseMessage.SetGroupIdAndResourceTagIncluded(0);

    packet->AddHeader(associationResponseMessage);

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    mxHeader.SetMacExtensionFieldEncoding(0);
    mxHeader.SetLengthField(0);
    mxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE);

    packet->AddHeader(mxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(0);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(ptInfo.longRdId);
    unicastHeader.SetTransmitterAddress(this->GetLongRadioDeviceId());

    packet->AddHeader(unicastHeader);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER);

    packet->AddHeader(macHeaderType);

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField = BuildPhysicalHeaderField(1, packet->GetSize());
    physicalHeaderField.SetShortNetworkID(m_networkId);

    m_phy->Send(packet, physicalHeaderField); // Send the packet to the PHY
}

void
Dect2020ExtMac::ProcessAssociationResponse(Dect2020ExtAssociationResponseMessageIE associationResponseMessage, uint32_t transmitterAddress)
{
    NS_LOG_FUNCTION(this << associationResponseMessage << transmitterAddress);

    if (m_associationStatus != ASSOCIATION_PENDING)
    {
        NS_LOG_WARN(
            "Association Response received, but association status is not ASSOCIATION_PENDING. "
            "Ignoring the response.");
        return;
    }

    if (associationResponseMessage.GetAssociationAccepted())
    {
        // Currently, we accept all association responses
        m_associationStatus = ASSOCIATED;

        // If we dont receive a Cluster Beacon every 60 Seconds, we assume the FT is offline
        Simulator::Schedule(Seconds(60), &Dect2020ExtMac::VerifyAssociatedStatus, this);

        // Store the associated FTs Long RD ID
        m_associatedFTNetDeviceLongRdId = transmitterAddress;
        NS_LOG_INFO(Simulator::Now().GetMicroSeconds()
                    << ": Dect2020Mac::HandleUnicastPacket: Association Response by 0x" << std::hex
                    << this->GetLongRadioDeviceId() << " accepted");
    }
    else
    {
        // tbd
    }
}

void
Dect2020ExtMac::VerifyAssociatedStatus(void)
{
    Dect2020ExtFtCandidateInfo* ft = FindFtCandidateByLongId(m_associatedFTNetDeviceLongRdId);
    Time lastrBeaconReceived = ft->receptionTime;
    Time currentTime = Simulator::Now();
    Time VerifyAssociatedStatusPeriod = Seconds(60);
    NS_LOG_INFO("ASSOCIATED");

    // Check if the last received Cluster Beacon is older than 60 seconds
    if (currentTime - lastrBeaconReceived > VerifyAssociatedStatusPeriod)
    {
        NS_LOG_WARN(
            Simulator::Now().GetMicroSeconds()
            << ": Dect2020Mac::VerifyAssociatedStatus: No Cluster Beacon received in the last "
            << "60 seconds. Setting association status to NOT_ASSOCIATED.");
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
    else
    {
        // Check again in VerifyAssociatedStatusPeriod
        Simulator::Schedule(VerifyAssociatedStatusPeriod,
                            &Dect2020ExtMac::VerifyAssociatedStatus,
                            this);
    }
}

Dect2020ExtFtCandidateInfo*
Dect2020ExtMac::FindFtCandidateByLongId(uint32_t longFtId)
{
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].longFtId == longFtId)
        {
            return &m_ftCandidates[idx];
        }
    }

    // No candidate found, create one
    Dect2020ExtFtCandidateInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

void
Dect2020ExtMac::SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType)
{
    m_terminationPointType = terminationPointType;
}

Dect2020ExtNetDevice::Dect2020ExtTerminationPointType
Dect2020ExtMac::GetTerminationPointType(void) const
{
    return m_terminationPointType;
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_device = device;
}

Ptr<Dect2020ExtNetDevice>
Dect2020ExtMac::GetNetDevice(void) const
{
    return m_device;
}

Ptr<Node>
Dect2020ExtMac::GetNode(void) const
{
    if (m_device)
    {
        return m_device->GetNode();
    }
    return nullptr;
}

/**
 * Assigns the PHY to the MAC
 */
void
Dect2020ExtMac::SetPhy(Ptr<Dect2020ExtPhy> phy)
{
    m_phy = phy;
    m_phy->SetReceiveCallback(MakeCallback(&Dect2020ExtMac::ReceiveFromPhy, this));
}

Ptr<Dect2020ExtPhy>
Dect2020ExtMac::GetPhy(void) const
{
    return m_phy;
}

void
Dect2020ExtMac::SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager)
{
    m_channelManager = channelManager;
}

void
Dect2020ExtMac::SetNetworkId(uint32_t networkId)
{
    m_networkId = networkId;
}

uint32_t
Dect2020ExtMac::GetNetworkId(void) const
{
    return m_networkId;
}

void
Dect2020ExtMac::SetLongRadioDeviceId(uint32_t rdId)
{
    if (!(rdId == 0x00000000))
    {
        m_longRadioDeviceId = rdId;

        if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
        {
            NS_LOG_INFO("Set Long Radio Device ID: 0x" << std::hex << rdId << " on FT Device " << this);
        }
        else if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
        {
            NS_LOG_INFO("Set Long Radio Device ID: 0x" << std::hex << rdId << " on PT Device " << this);
        }
    }
    else
    {
        NS_LOG_WARN(Simulator::Now().GetMilliSeconds()
                    << ": Invalid Long Radio Device ID detected. Generate a new one.");

        SetLongRadioDeviceId(GenerateLongRadioDeviceId());
    }
}

uint32_t
Dect2020ExtMac::GetLongRadioDeviceId(void) const
{
    return m_longRadioDeviceId;
}

void
Dect2020ExtMac::SetShortRadioDeviceId(uint16_t rdId)
{
    if (!(rdId == 0x0000))
    {
        m_shortRadioDeviceId = rdId;
        if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
        {
            NS_LOG_INFO("Set Short Radio Device ID: 0x" << std::hex << rdId << " on FT Device " << this);
        }
        else if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
        {
            NS_LOG_INFO("Set Short Radio Device ID: 0x" << std::hex << rdId << " on PT Device " << this);
        }
    }
    else
    {
        NS_LOG_WARN(Simulator::Now().GetMilliSeconds()
                    << ": Invalid Short Radio Device ID detected. Generate a new one.");

        SetShortRadioDeviceId(GenerateShortRadioDeviceId());
    }
}

uint16_t
Dect2020ExtMac::GetShortRadioDeviceId(void) const
{
    return m_shortRadioDeviceId;
}

uint32_t
Dect2020ExtMac::GetOperatingChannelId(void) const
{
    return m_operatingChannelId;
}

std::vector<Dect2020ExtChannelEvaluation>
Dect2020ExtMac::GetChannelEvaluations(void) const
{
    return m_channelEvaluations;
}

void
Dect2020ExtMac::SetCurrentChannelId(uint16_t channelId)
{
    m_currentChannelId = channelId;
}

uint16_t
Dect2020ExtMac::GetCurrentChannelId(void) const
{
    return m_currentChannelId;
}

double
Dect2020ExtMac::GetRxGainFromIndex(uint8_t index) const
{
    if (index > 8)
    {
        NS_LOG_WARN("Invalid RX gain index: " << static_cast<int>(index));
        return 0; // default fallback
    }

    return static_cast<int8_t>(-10 + index * 2);
}

void
Dect2020ExtMac::SetBandNumber(uint8_t bandNumber)
{
    m_bandNumber = bandNumber;
}

uint8_t
Dect2020ExtMac::GetBandNumber(void) const
{
    return m_bandNumber;
}

void
Dect2020ExtMac::SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor)
{
    m_subcarrierScalingFactor = subcarrierScalingFactor;
}

uint8_t
Dect2020ExtMac::GetSubcarrierScalingFactor(void) const
{
    return m_subcarrierScalingFactor;
}

void
Dect2020ExtMac::SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor)
{
    m_fourierTransformScalingFactor = fourierTransformScalingFactor;
}

uint8_t
Dect2020ExtMac::GetFourierTransformScalingFactor(void) const
{
    return m_fourierTransformScalingFactor;
}

void
Dect2020ExtMac::PrintConfiguration(void) const
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("DECT-2020 MAC Configuration:");
    NS_LOG_INFO("Band Number: " << static_cast<uint32_t>(m_bandNumber));
    NS_LOG_INFO("Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO("Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor));

    std::cout << "DECT-2020 MAC Configuration:" << "\n";
    std::cout << "Band Number: " << static_cast<uint32_t>(m_bandNumber) << "\n";
    std::cout << "Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor) << "\n";
    std::cout << "Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor) << "\n";
}

} // namespace ns3