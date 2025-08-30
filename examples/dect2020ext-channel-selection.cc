#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/ptr.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannelSelection");

void
viewChannels(std::vector<Ptr<Dect2020ExtChannel>> channels)
{
    for (auto channel : channels)
    {
        NS_LOG_INFO("Channel: " << channel->m_channelId << " Center Frequency: " << channel->m_centerFrequency);
    }
}

int main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtChannelSelection", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtMac", LOG_LEVEL_FUNCTION);
    LogComponentEnable("Dect2020ExtMac", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtMac", LOG_PREFIX_TIME);

    uint8_t bandNumber = 1;
    uint8_t subcarrierScalingFactor = 1;
    Dect2020ExtChannelManager channelManager;
    Ptr<Node> ftNode = CreateObject<Node>();
    Ptr<Dect2020ExtNetDevice> ftDev = CreateObject<Dect2020ExtNetDevice>();
    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();

    ftNode->AddDevice(ftDev);
    ftDev->SetBandNumber(bandNumber);
    ftDev->SetMac(ftMac);

    channelManager.InitializeChannels(bandNumber, subcarrierScalingFactor);
    viewChannels(Dect2020ExtChannelManager::GetValidChannels(bandNumber));

    ftMac->ScheduleBackgroundScan(Seconds(1.0));

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
