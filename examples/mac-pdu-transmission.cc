#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/dect2020ext-mac-pdu-helper.h"
#include "ns3/dect2020-net-device.h"
#include "ns3/dect2020-channel-manager.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduTransmissionExample");

int
main(int argc, char* argv[])
{
    LogComponentEnable("MacPduTransmissionExample", LOG_LEVEL_ALL);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}