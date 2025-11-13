#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/dect2020ext-mac-pdu-helper.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"
#include "ns3/pointer.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
    m_networkBeaconSweepCount = 0;
    m_nextNetworkBeaconTime = Seconds(0);
    InitializeDevice();
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
    if (m_netDevice->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        NS_LOG_INFO("FT Device " << this << " starts the network initialization at: " << Simulator::Now().GetMilliSeconds());
        InitializeNetwork();
    }
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
    }
    else
    {
        NS_LOG_WARN("No channel evaluations available for selection");
    }

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
    StartNetworkBeaconTransmission();
    StartClusterBeaconTransmission();
}

void
Dect2020ExtMac::StartNetworkBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
    Time networkBeaconPeriod = m_networkBeaconPeriodTime; // gap between each transmission
    Simulator::Schedule(networkBeaconPeriod, &Dect2020ExtMac::SendNetworkBeaconOnChannel, this, m_operatingChannelId);

    // Schedule the next network beacon transmission
    // Calculate the dynamic offset for the next network beacon transmission
    Time dynamicOffset = MilliSeconds(25 + (m_networkBeaconSweepCount % 3) * 2);
    m_nextNetworkBeaconTime = Simulator::Now() + networkBeaconPeriod + dynamicOffset;

    NS_LOG_DEBUG("Network beacon sweep count: " << m_networkBeaconSweepCount);
    NS_LOG_DEBUG("Dynamic offset: " << dynamicOffset);
    NS_LOG_DEBUG("Next network beacon transmission scheduled at: " << m_nextNetworkBeaconTime);

    m_networkBeaconSweepCount++;
    Simulator::Schedule(networkBeaconPeriod + dynamicOffset, &Dect2020ExtMac::StartNetworkBeaconTransmission, this);
}

void
Dect2020ExtMac::StartClusterBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
    Time clusterBeaconPeriod = m_clusterBeaconPeriodTime; // gap between each transmission
    Simulator::Schedule(Simulator::Now(), &Dect2020ExtMac::SendClusterBeaconOnChannel, this, m_operatingChannelId);

    // Schedule the next cluster beacon transmission
    Simulator::Schedule(clusterBeaconPeriod, &Dect2020ExtMac::StartClusterBeaconTransmission, this);
}

void
Dect2020ExtMac::SendNetworkBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> networkBeacon = BuildNetworkBeacon();
    m_phy->Send(networkBeacon, BuildPhysicalHeaderField(1, networkBeacon->GetSize()));
}

void
Dect2020ExtMac::SendClusterBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> packet = BuildClusterBeacon(channelId);
    m_phy->Send(packet, BuildPhysicalHeaderField(1, packet->GetSize()));
}

Ptr<Packet>
Dect2020ExtMac::BuildNetworkBeacon(void)
{
    NS_LOG_FUNCTION(this);

    // MAC Header Type
    // MAC Common Header
    Ptr<Packet> beacon = Dect2020ExtMacPduHelper::BEACON(MacSecurityField::MAC_SECURITY_NOT_USED,
                                                         m_networkId,
                                                         m_longRadioDeviceId);

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtension(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLength(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIEType(Dect2020ExtMacMultiplexingHeader::ExtendedIEType::NetworkBeaconMessage);

    beacon->AddHeader(mxHeaderTypeA);

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

    return beacon;
}

Ptr<Packet>
Dect2020ExtMac::BuildClusterBeacon(uint32_t clusterChannelId)
{
    NS_LOG_FUNCTION(this);

    // MAC Header Type
    // MAC Common Header
    Ptr<Packet> beacon = Dect2020ExtMacPduHelper::BEACON(MacSecurityField::MAC_SECURITY_NOT_USED,
                                                         m_networkId,
                                                         m_longRadioDeviceId);

    // MAC Multiplexing Header Type A
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtension(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLength(0); // No length field included in the IE Header'
    
    beacon->AddHeader(mxHeaderTypeA);

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
    randomAccessResourceIE.SetRaraLength(m_netDevice->m_randomAccessResourceLength);
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

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField;
    packet->RemoveHeader(physicalHeaderField);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    packet->RemoveHeader(macHeaderType);

    // --- Beacon Header ---
    if (macHeaderType.GetMacHeaderTypeField() ==
        Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER)
    {
        // If the candidate has sent a beacon message before, we can update its entry in the FT
        // Candidate List
        Dect2020ExtFtCandidateInfo* ft = FindOrCreateFtCandidate(physicalHeaderField.GetTransmitterIdentity());
        ft->rssiDbm = rssiDbm;
        ft->receptionTime = Simulator::Now();
        ft->ftPhyHeaderField = physicalHeaderField;
        ft->shortFtId = physicalHeaderField.GetTransmitterIdentity();
        ft->shortNetworkId = physicalHeaderField.GetShortNetworkID();
        ft->clusterChannelId =
            this->GetCurrentChannelId(); // ReceiveFromPhy can only be called if the sent Packet is
                                         // on the current channel

        HandleBeaconPacket(packet, ft);
    }
    // --- Unicast Header ---
    else if (macHeaderType.GetMacHeaderTypeField() ==
             Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER)
    {
        HandleUnicastPacket(packet);
    }

    // Forward the packet to the net device
    m_netDevice->Receive(packet);

    // m_rxPacketTrace(packet);
}

void
Dect2020ExtMac::HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this << packet);
}

void
Dect2020ExtMac::HandleUnicastPacket(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
}

Dect2020ExtFtCandidateInfo*
Dect2020ExtMac::FindOrCreateFtCandidate(uint16_t shortFtId)
{
    NS_LOG_FUNCTION(this << shortFtId);
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].shortFtId == shortFtId)
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
    m_netDevice = device;
}

Ptr<Dect2020ExtNetDevice>
Dect2020ExtMac::GetNetDevice(void) const
{
    return m_netDevice;
}

/**
 * Assigns the PHY to the MAC
 */
void
Dect2020ExtMac::SetPhy(Ptr<Dect2020ExtPhy> phy)
{
    m_phy = phy;
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

        NS_LOG_INFO("Set Long Radio Device ID: 0x" << std::hex << rdId << " on Device " << this);
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
        NS_LOG_INFO("Set Short Radio Device ID: 0x" << std::hex << rdId << " on Device " << this);
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

uint16_t
Dect2020ExtMac::GetCurrentChannelId(void) const
{
    return m_currentChannelId;
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