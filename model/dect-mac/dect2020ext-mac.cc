#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/dect2020ext-mac-pdu-helper.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"
#include "ns3/pointer.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
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
    if (m_channelManager == nullptr)
    {
        NS_LOG_ERROR("Channel manager is not set for MAC");
    }
    SetLongRadioDeviceId(GenerateLongRadioDeviceId());
    SetShortRadioDeviceId(GenerateShortRadioDeviceId());
}
void
Dect2020ExtMac::Start(void)
{
    if (m_netDevice->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
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

void
Dect2020ExtMac::SelectOperatingChannel(void)
{
    std::vector<Dect2020ExtChannelEvaluation> evaluations;
    for (auto& channel : m_channelManager->GetChannels())
    {
        Dect2020ExtChannelEvaluation evaluation;
        evaluation.channelId = channel->m_channelId;

        for (auto& slot : channel->m_slots)
        {
            for (auto& subslot : slot.subslots)
            {
                if (subslot.status == SubslotStatus::FREE)
                {
                    evaluation.free++;
                }
                else if (subslot.status == SubslotStatus::POSSIBLE)
                {
                    evaluation.possible++;
                }
                else if (subslot.status == SubslotStatus::BUSY)
                {
                    evaluation.busy++;
                }
            }
        }
        evaluations.push_back(evaluation);
    }

    auto SortByFreeSubslotsLambda = [](const Dect2020ExtChannelEvaluation& a, const Dect2020ExtChannelEvaluation& b)
    {
        return a.free > b.free;
    };

    std::sort(evaluations.begin(), evaluations.end(), SortByFreeSubslotsLambda);
    m_operatingChannelId = evaluations[0].channelId;
    StartBeaconTransmission();
}

void
Dect2020ExtMac::ScheduleBackgroundScan(Time delay)
{
    NS_LOG_FUNCTION(this);

    // Prepare for the background scan
    uint32_t numSubslots = m_channelManager->GetSubslotsPerSlot();
    auto validChannels = m_channelManager->GetValidChannels(1);
    uint16_t numOfValidChannels = validChannels.size();
    m_channelManager->TraceConnectWithoutContext("RssiMeasured", MakeCallback(&Dect2020ExtMac::OnRssiMeasurement, this));

    // Schedule the background scan
    Simulator::Schedule(delay, &Dect2020ExtMac::BackgroundScan, this, numSubslots, validChannels, numOfValidChannels);
}

void
Dect2020ExtMac::OnRssiMeasurement(uint16_t channelId, double rssiDbm)
{
    NS_LOG_FUNCTION(this << channelId << rssiDbm);
    m_rssiByChannel[channelId].push_back(rssiDbm);
    auto validChannels = m_channelManager->GetValidChannels(1);
    const uint32_t totalSubslotsPerChannel = m_channelManager->GetSubslotsPerSlot() * 24;
    // TODO: Evaluate if the rssi by channel is complete based on the subslot scan contex
    if (m_rssiByChannel.size() == validChannels.size() && 
        std::all_of(m_rssiByChannel.begin(), m_rssiByChannel.end(), 
                    [totalSubslotsPerChannel](const auto& kv) { return kv.second.size() == totalSubslotsPerChannel; }))
    {
        EvaluateAllChannels();
    }
}

void
Dect2020ExtMac::MeasureRSSIOnChannel(Ptr<Dect2020ExtChannel> channel, int64_t frameDurationNs, uint32_t numSubslots)
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
            // TODO: Pass subslot scan context to the MeasureRssi method (create a new method for this in Dect2020ExtChannelManager)
            Simulator::Schedule(NanoSeconds(subslotRssiMeasurementDelayNs), &Dect2020ExtChannelManager::MeasureRssi, m_channelManager, channel->m_channelId, TimeValue(NanoSeconds(subslotDurationNs)));
            subslotIndex++;
        }
    }
}

void
Dect2020ExtMac::BackgroundScan(uint32_t numSubslots, std::vector<Ptr<Dect2020ExtChannel>> validChannels, uint16_t numOfValidChannels)
{
    const uint32_t slotsPerFrame = 24; // #ETSI TS 103 636-3 V1.5.1, Section 4.4
    const int64_t frameDurationNs = 10000000; // 10 milliseconds in nanoseconds
    const uint32_t totalSubslotsPerFrame = slotsPerFrame * numSubslots;
    NS_LOG_INFO("Total subslots per frame: " << totalSubslotsPerFrame);
    
    int64_t channelRssiMeasurementDelayNs = 0;
    for (auto channel : validChannels)
    {
        // Schedule the RSSI measurement for each channel with 10ms period
        // TODO: Schedule the RSSI measurement with subslot scan context
        Simulator::Schedule(NanoSeconds(channelRssiMeasurementDelayNs), &Dect2020ExtMac::MeasureRSSIOnChannel, this, channel, frameDurationNs, totalSubslotsPerFrame);
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
    static uint32_t sweepCount = 0;
    Time dynamicOffset = MilliSeconds(25 + (sweepCount++ % 3) * 2);
    Simulator::Schedule(networkBeaconPeriod + dynamicOffset, &Dect2020ExtMac::StartNetworkBeaconTransmission, this);
}

void
Dect2020ExtMac::StartClusterBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtMac::SendNetworkBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> networkBeacon = BuildNetworkBeacon();
    m_phy->Send(networkBeacon, CreatePhysicalHeaderField(1, networkBeacon->GetSize()));
}

void
Dect2020ExtMac::SendClusterBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> packet = Create<Packet>();
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
    networkBeaconMessageIE.SetTimeToNext(0); // TODO: Time to next = Next send time - current sim time

    beacon->AddHeader(networkBeaconMessageIE);

    return beacon;
}

Ptr<Packet>
Dect2020ExtMac::BuildClusterBeacon(void)
{
    NS_LOG_FUNCTION(this);

    // MAC Header Type
    // MAC Common Header

    Ptr<Packet> beacon = Dect2020ExtMacPduHelper::BEACON(MacSecurityField::MAC_SECURITY_NOT_USED,
                                                         m_networkId,
                                                         m_longRadioDeviceId);

    return beacon;
}

Dect2020ExtPHYControlFieldType1
Dect2020ExtMac::CreatePhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes)
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
    physicalHeaderField.SetTransmitPower(23); // TODO: How to determine the transmit power?
    physicalHeaderField.SetDFMCS(m_mcs);

    return physicalHeaderField;
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

        NS_LOG_INFO(Simulator::Now().GetMilliSeconds()
                    << ": Set Long Radio Device ID: 0x" << std::hex << rdId << " on Device "
                    << this);
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