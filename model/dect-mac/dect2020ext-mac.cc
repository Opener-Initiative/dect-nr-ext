#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
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
Dect2020ExtMac::OperatingChannelSelection(void)
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtMac::ScheduleBackgroundScan(Time delay)
{
    NS_LOG_FUNCTION(this);

    // Prepare for the background scan
    uint32_t numSubslots = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    auto validChannels = Dect2020ExtChannelManager::GetValidChannels(m_netDevice->GetBandNumber());
    uint16_t numOfValidChannels = validChannels.size();

    // Schedule the background scan
    Simulator::Schedule(delay, &Dect2020ExtMac::BackgroundScan, this, numSubslots, validChannels, numOfValidChannels);
}

void
Dect2020ExtMac::BackgroundScan(uint32_t numSubslots, std::vector<Ptr<Dect2020ExtChannel>> validChannels, uint16_t numOfValidChannels)
{
    NS_LOG_FUNCTION(this << numSubslots << validChannels.size() << numOfValidChannels);
    for (auto channel : validChannels)
    {
        ChannelEvaluation evaluation;
        evaluation.channelId = channel->m_channelId;

        for (auto slot : channel->m_slots)
        {
            for (auto subslot : slot.subslots)
            {
                if (subslot.rssi <= RSSI_THRESHOLD_MIN)
                {
                    evaluation.free++;
                    break;
                }

                if (subslot.rssi > RSSI_THRESHOLD_MIN || subslot.rssi <= RSSI_THRESHOLD_MAX)
                {
                    evaluation.possible++;
                    break;
                }

                if (subslot.rssi > RSSI_THRESHOLD_MAX)
                {
                    evaluation.busy++;
                    break;
                }
            }
        }
        m_scanEvaluations[evaluation.channelId] = evaluation;
    }
    SelectOperatingChannel();
}

void
Dect2020ExtMac::SelectOperatingChannel(void)
{
    NS_LOG_FUNCTION(this);
    for (auto& evaluation : m_scanEvaluations)
    {
        // NS_LOG_INFO("Channel ID: " << evaluation.first << " RSSI: " << Dect2020ExtChannelManager::GetRssiDbm(evaluation.first, MilliSeconds(1)));
    }
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_netDevice = device;
}

} // namespace ns3