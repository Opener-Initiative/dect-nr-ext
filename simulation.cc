#include "dect-channel.h"
#include "dect-phy.h"

#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("DECTSimulation");

int
main(int argc, char* argv[])
{
    //hey
    // Create Sta Node
    NodeContainer staNode;
    staNode.Create(1);

    // Create AP Node
    NodeContainer apNode;
    apNode.Create(1);

    DectChannel channel;
    channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
    DectPhy phy;
    phy.SetChannel(channel.Create());


    Simulator::Run();
    Simulator::Destroy();
    return 0;

}
