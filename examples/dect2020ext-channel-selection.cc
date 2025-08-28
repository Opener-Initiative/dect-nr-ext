#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-channel.h"

#include "ns3/simulator.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    Dect2020ExtChannelManager channelManager;
    channelManager.InitializeChannels(1, 1, 1);
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
