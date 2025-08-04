#include "ns3/log.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/simulator.h"
#include "ns3/simple-net-device.h"

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
    Ptr<SimpleChannel> channel = CreateObject<SimpleChannel>();
    channel->SetAttribute("Delay", TimeValue(MilliSeconds(1)));

    // Create SimpleNetDevices
    Ptr<SimpleNetDevice> device0 = CreateObject<SimpleNetDevice>();
    Ptr<SimpleNetDevice> device1 = CreateObject<SimpleNetDevice>();

    // Install SimpleNetDevices on nodes and attach SimpleChannel to the devices
    device0->SetChannel(channel);
    device1->SetChannel(channel);

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
    Ptr<Packet> packet2 = Create<Packet>(100);

    // Schedule packet transmission from node 0 to node 1
    NS_LOG_INFO("Scheduling packet transmission...");
    Simulator::Schedule(Seconds(1.0), &NetDevice::SendFrom, device0, packet1, addr0, addr1, 0x0103);
    Simulator::Schedule(Seconds(1.0), &NetDevice::SendFrom, device1, packet2, addr1, addr0, 0x0103);

    NS_LOG_INFO("Device 0 MTU: " << device0->GetMtu());
    NS_LOG_INFO("Device 1 MTU: " << device1->GetMtu());

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}