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

#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-net-device.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TransmittingMacPduExample");

bool
ReceiveCallback(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO("Received packet: " << packet->GetSize() << " bytes" << " at " << Simulator::Now().GetSeconds() << " seconds");
    return true;
}

int main(int argc, char *argv[])
{
    // Enable logging for this component
    LogComponentEnable("TransmittingMacPduExample", LOG_LEVEL_INFO);
    
    // Create nodes
    Ptr<Node> node0 = CreateObject<Node>();
    Ptr<Node> node1 = CreateObject<Node>();

    // Create and configure SimpleChannel
    Ptr<Dect2020Channel> channel = CreateObject<Dect2020Channel>();
    channel->SetAttribute("Delay", TimeValue(MilliSeconds(1)));

    // Create Dect2020NetDevices
    Ptr<Dect2020NetDevice> device0 = CreateObject<Dect2020NetDevice>();
    Ptr<Dect2020NetDevice> device1 = CreateObject<Dect2020NetDevice>();

    // Install Dect2020NetDevices on nodes and attach Dect2020Channel to the devices
    device0->SetChannel(channel->GetWrappedChannel());
    device1->SetChannel(channel->GetWrappedChannel());

    node0->AddDevice(device0);
    node1->AddDevice(device1);

    // Set ReceiveCallback functions
    device0->SetReceiveCallback(MakeCallback(&ReceiveCallback));
    device1->SetReceiveCallback(MakeCallback(&ReceiveCallback));

    // Get device addresses
    Address addr0 = device0->GetAddress();
    Address addr1 = device1->GetAddress();
    
    NS_LOG_INFO("Node 0 address: " << addr0);
    NS_LOG_INFO("Node 1 address: " << addr1);

    // Create packets
    Ptr<Packet> packet1 = Create<Packet>(100);
    Ptr<Packet> packet2 = Create<Packet>(200);
    Ptr<Packet> packet3 = Create<Packet>(150);

    // Schedule packet transmission from node 0 to node 1
    NS_LOG_INFO("Scheduling packet transmission...");
    Simulator::Schedule(Seconds(1.0), &NetDevice::Send, device0, packet1, addr1, 0x0103); // DECT NR protocol number???
    Simulator::Schedule(Seconds(2.0), &NetDevice::Send, device0, packet2, addr1, 0x0103);
    Simulator::Schedule(Seconds(3.0), &NetDevice::Send, device0, packet3, addr1, 0x0103);

    Simulator::Stop(Seconds(5.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}