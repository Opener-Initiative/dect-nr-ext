#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSimulation1");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtSimulation1", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtChannelManager", LOG_LEVEL_INFO);
    LogComponentEnable("SpectrumAnalyzer", LOG_LEVEL_INFO);

    Dect2020ExtChannelManager channelManager;
    channelManager.InitializeChannels(1, 1);
    channelManager.GetRssiDbm();

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}