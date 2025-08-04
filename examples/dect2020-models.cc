#include "ns3/node.h"
#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"
#include "ns3/dect2020-mac.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ModelsExample");

bool
ReceiveCallback(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO(packet->GetSize()
                    << " bytes received from " 
                    << sender << " at " 
                    << Simulator::Now().GetSeconds() << " seconds");
    return true;
}

int main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ModelsExample", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020Mac", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020NetDevice", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020Channel", LOG_LEVEL_ALL);

    Ptr<Node> node0 = CreateObject<Node>();
    Ptr<Node> node1 = CreateObject<Node>();

    // Configuring the channel
    Ptr<Dect2020Channel> channel = CreateObject<Dect2020Channel>();
    channel->SetAttribute("Delay", TimeValue(MilliSeconds(1)));

    Ptr<Dect2020NetDevice> device0 = CreateObject<Dect2020NetDevice>();
    Ptr<Dect2020NetDevice> device1 = CreateObject<Dect2020NetDevice>();

    device0->SetRDMode(Dect2020NetDevice::RDMode::FT);
    device1->SetRDMode(Dect2020NetDevice::RDMode::FT);

    // Adding the channel to the devices
    device0->SetChannel(channel);
    device1->SetChannel(channel);

    // Adding the MAC layer to the devices
    Ptr<Dect2020Mac> mac0 = CreateObject<Dect2020Mac>();
    Ptr<Dect2020Mac> mac1 = CreateObject<Dect2020Mac>();

    device0->SetMac(mac0);
    device1->SetMac(mac1);

    // Adding the devices to the nodes
    node0->AddDevice(device0);
    node1->AddDevice(device1);

    // Setting the receive callback
    device0->SetReceiveCallback(MakeCallback(&ReceiveCallback));
    device1->SetReceiveCallback(MakeCallback(&ReceiveCallback));

    // Setting addresses
    device0->SetAddress(Mac48Address::Allocate());
    device1->SetAddress(Mac48Address::Allocate());

    Address address0 = device0->GetAddress();
    Address address1 = device1->GetAddress();

    // Creating a packet
    Ptr<Packet> packet0 = Create<Packet>(150);
    Ptr<Packet> packet1 = Create<Packet>(150);

    // Scheduling packet transmission
    Simulator::Schedule(Seconds(1.0), &Dect2020NetDevice::SendFrom, device0, packet0, address0, address1, 0x0103);
    Simulator::Schedule(Seconds(1.0), &Dect2020NetDevice::SendFrom, device1, packet1, address1, address0, 0x0103);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}