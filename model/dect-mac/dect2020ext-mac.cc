#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
    // Dect2020ExtChannelManager::m_rssiMeasured.ConnectWithoutContext(MakeCallback(&Dect2020ExtMac::OnRssiMeasurement, this));
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
                            .AddConstructor<Dect2020ExtMac>();
    return tid;
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
}

void
Dect2020ExtMac::StartClusterBeaconTransmission(void)
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_netDevice = device;
}

void
Dect2020ExtMac::SetPhy(Ptr<Dect2020ExtPhy> phy)
{
    m_phy = phy;
    m_phy->SetMac(this);
}

void
Dect2020ExtMac::SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager)
{
    m_channelManager = channelManager;
}

} // namespace ns3