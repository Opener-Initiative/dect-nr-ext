#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-net-device.h"

#include "ns3/log.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/simulator.h"
#include "ns3/simple-net-device.h"

#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/dect2020-network-beacon-message-ie.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    Ptr<Dect2020Channel> channel = CreateObject<Dect2020Channel>();
    channel->SetAttribute("Delay", TimeValue(Seconds(0.001)));

    Ptr<Dect2020NetDevice> device0 = CreateObject<Dect2020NetDevice>();

    device0->SetChannel(channel);

    return 0;
}