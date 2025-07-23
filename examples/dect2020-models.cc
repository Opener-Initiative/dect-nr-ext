#include "ns3/node.h"
#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"

#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/node-container.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ModelsExample");

bool
ReceiveCallback(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO(packet->GetSize() << " bytes received from " << sender);
    return true;
}

int main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ModelsExample", LOG_LEVEL_INFO);

    Ptr<Node> node0 = CreateObject<Node>();
    Ptr<Node> node1 = CreateObject<Node>();

    // Configuring the channel
    Ptr<Dect2020Channel> channel = CreateObject<Dect2020Channel>();
    channel->SetAttribute("Delay", TimeValue(Seconds(1)));

    Ptr<Dect2020NetDevice> device0 = CreateObject<Dect2020NetDevice>();
    Ptr<Dect2020NetDevice> device1 = CreateObject<Dect2020NetDevice>();

    // Adding the channel to the devices
    device0->SetChannel(channel->GetWrappedChannel());
    device1->SetChannel(channel->GetWrappedChannel());

    // Adding the devices to the nodes
    node0->AddDevice(device0);
    node1->AddDevice(device1);

    // Setting the receive callback
    device0->SetReceiveCallback(MakeCallback(&ReceiveCallback));
    device1->SetReceiveCallback(MakeCallback(&ReceiveCallback));

    //Address address0 = device0->GetAddress();
    Address address1 = device1->GetAddress();

    // Creating a packet
    Ptr<Packet> packet = Create<Packet>(100);

    // Scheduling packet transmission
    Simulator::Schedule(Seconds(1.0), &NetDevice::Send, device0, packet, address1, 0x0103);

    Simulator::Stop(Seconds(3.0)); // Stop the simulator after 2 seconds
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}