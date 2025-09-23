#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"

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
    const auto rssiResults = Dect2020ExtChannelManager::GetRssiResults();
    auto channels = Dect2020ExtChannelManager::GetValidChannels(1);
    
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
}

void
Dect2020ExtMac::ScheduleBackgroundScan(Time delay)
{
    NS_LOG_FUNCTION(this);

    // Prepare for the background scan
    uint32_t numSubslots = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    auto validChannels = Dect2020ExtChannelManager::GetValidChannels(1);
    uint16_t numOfValidChannels = validChannels.size();

    // Schedule the background scan
    Simulator::Schedule(delay, &Dect2020ExtMac::BackgroundScan, this, numSubslots, validChannels, numOfValidChannels);
}

void
Dect2020ExtMac::BackgroundScan(uint32_t numSubslots, std::vector<Ptr<Dect2020ExtChannel>> validChannels, uint16_t numOfValidChannels)
{
    NS_LOG_FUNCTION(this << numSubslots << validChannels.size() << numOfValidChannels);
    
    const uint32_t slotsPerFrame = 24; // #ETSI TS 103 636-3 V1.5.1, Section 4.4
    const double slotDurationNs = 10000000 / slotsPerFrame;         // 0,41667 milliseconds in nanoseconds
    double subslotDurationNs = slotDurationNs / numSubslots; // Subslot Duration in nanoseconds
    NS_LOG_INFO("Subslot Duration: " << subslotDurationNs << " ns");
    double subslotRssiMeasurementDelayNs = 0;
    for (uint32_t i = 0; i < 24*numSubslots; i++)
    {
        Simulator::Schedule(NanoSeconds(subslotRssiMeasurementDelayNs), &Dect2020ExtChannelManager::MeasureRssi, TimeValue(NanoSeconds(subslotDurationNs)));
        subslotRssiMeasurementDelayNs += subslotDurationNs;
    }
    Simulator::Schedule(NanoSeconds(subslotRssiMeasurementDelayNs + 100), &Dect2020ExtMac::OperatingChannelSelection, this);
}

void
Dect2020ExtMac::SelectOperatingChannel(void)
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_netDevice = device;
}

} // namespace ns3