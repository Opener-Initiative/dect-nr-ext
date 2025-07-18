#include "ns3/log.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SendingMacPduExample");

bool
ReceivePacket1(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO("Received packet: " << packet->GetSize() << " bytes");
    return true;
}

bool
ReceivePacket2(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO("Received packet: " << packet->GetSize() << " bytes");
    return true;
}

int main(int argc, char *argv[])
{
    // Enable logging for this component
    LogComponentEnable("SendingMacPduExample", LOG_LEVEL_INFO);
    
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Create SimpleChannel
    Ptr<SimpleChannel> channel = CreateObject<SimpleChannel>();
    channel->SetAttribute("Delay", TimeValue(MilliSeconds(1)));

    // Create SimpleNetDeviceHelper
    SimpleNetDeviceHelper simpleNetDeviceHelper;

    // Install SimpleNetDevices and SimpleChannel on nodes
    NetDeviceContainer devices = simpleNetDeviceHelper.Install(nodes, channel);

    // Set ReceiveCallback functions
    devices.Get(0)->SetReceiveCallback(MakeCallback(&ReceivePacket1));
    devices.Get(1)->SetReceiveCallback(MakeCallback(&ReceivePacket2));

    // Get device addresses for sending
    Address addr0 = devices.Get(0)->GetAddress();
    Address addr1 = devices.Get(1)->GetAddress();
    
    NS_LOG_INFO("Node 0 address: " << addr0);
    NS_LOG_INFO("Node 1 address: " << addr1);

    // Send packets from node 0 to node 1
    Ptr<Packet> packet1 = Create<Packet>(100);
    Ptr<Packet> packet2 = Create<Packet>(200);
    Ptr<Packet> packet3 = Create<Packet>(150);

    // Schedule packet transmissions from node 0 to node 1
    NS_LOG_INFO("Scheduling packet transmissions...");
    Simulator::Schedule(Seconds(1.0), &NetDevice::Send, devices.Get(0), packet1, addr1, 0x0800);
    Simulator::Schedule(Seconds(2.0), &NetDevice::Send, devices.Get(0), packet2, addr1, 0x0800);
    Simulator::Schedule(Seconds(3.0), &NetDevice::Send, devices.Get(0), packet3, addr1, 0x0800);

    Simulator::Stop(Seconds(5.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}