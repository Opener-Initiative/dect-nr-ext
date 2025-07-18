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

NS_LOG_COMPONENT_DEFINE("TransmittingMacPduExample");

bool
ReceiveCallback(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol, const Address& sender)
{
    NS_LOG_INFO("Received packet: " << packet->GetSize() << " bytes" << " at " << Simulator::Now().GetSeconds() << " seconds");
    return true;
}

Ptr<Packet>
CreateNetworkBeaconMessage()
{
    Dect2020MacPduHelper macPduHelper;
    Dect2020MacMultiplexingHeaderTypeC mxHeader;
    Dect2020NetworkBeaconMessageIE networkBeaconMessageIE;

    // Creating MAC PDU (ns3::Packet) with:
    // MAC Header Type and MAC Common Header as Beacon Header
    Ptr<Packet> macPdu = macPduHelper.BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );

    // Setting values for Network Beacon Message IE
    networkBeaconMessageIE.SetTxPowerIncluded(true);
    networkBeaconMessageIE.SetPowerConstraints(false);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(true);
    networkBeaconMessageIE.SetNetworkBeaconChannels(2);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020NetworkBeaconMessageIE::NETWORK_PERIOD_1000MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020NetworkBeaconMessageIE::CLUSTER_PERIOD_500MS);
    networkBeaconMessageIE.SetNextClusterChannel(1234);
    networkBeaconMessageIE.SetTimeToNext(50000); // 50ms in microseconds
    networkBeaconMessageIE.SetClustersMaxTxPower(20); // 20 dBm
    networkBeaconMessageIE.SetCurrentClusterChannel(5678);
    
    // Set additional network beacon channels
    uint16_t additionalChannels[3] = {100, 200, 300};
    networkBeaconMessageIE.SetAdditionalNetworkBeaconChannels(additionalChannels);

    // Setting up the multiplexing header
    mxHeader.SetMacExtension(0x00); // 00 = No length is included in the IE Header. The IE type defines the length of the IE payload.
    mxHeader.SetIEType(Dect2020MacMultiplexingHeader::ExtendedIEType::NetworkBeaconMessage);
    mxHeader.AddPayload(&networkBeaconMessageIE, networkBeaconMessageIE.GetSerializedSize());

    // Inserting the multiplexing header together with the payload into the MAC PDU
    macPdu->AddHeader(mxHeader); 

    return macPdu;
}

int main(int argc, char *argv[])
{
    // Enable logging for this component
    LogComponentEnable("TransmittingMacPduExample", LOG_LEVEL_INFO);
    
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Create and configure SimpleChannel
    Ptr<SimpleChannel> channel = CreateObject<SimpleChannel>();
    channel->SetAttribute("Delay", TimeValue(MilliSeconds(1)));

    // Create SimpleNetDeviceHelper
    SimpleNetDeviceHelper simpleNetDeviceHelper;

    // Install SimpleNetDevices on nodes and attach SimpleChannel to the devices
    NetDeviceContainer devices = simpleNetDeviceHelper.Install(nodes, channel);

    // Set ReceiveCallback functions
    devices.Get(0)->SetReceiveCallback(MakeCallback(&ReceiveCallback));
    devices.Get(1)->SetReceiveCallback(MakeCallback(&ReceiveCallback));

    // Get device addresses
    Address addr0 = devices.Get(0)->GetAddress();
    Address addr1 = devices.Get(1)->GetAddress();
    
    NS_LOG_INFO("Node 0 address: " << addr0);
    NS_LOG_INFO("Node 1 address: " << addr1);

    // Create packets
    Ptr<Packet> packet1 = Create<Packet>(100);
    Ptr<Packet> packet2 = Create<Packet>(200);
    Ptr<Packet> packet3 = Create<Packet>(150);

    // Create Network Beacon Message
    Ptr<Packet> networkBeaconMessage = CreateNetworkBeaconMessage();

    // Schedule packet transmission from node 0 to node 1
    NS_LOG_INFO("Scheduling packet transmission...");
    Simulator::Schedule(Seconds(1.0), &NetDevice::Send, devices.Get(0), packet1, addr1, 0x0103); // DECT NR protocol number???
    Simulator::Schedule(Seconds(2.0), &NetDevice::Send, devices.Get(0), packet2, addr1, 0x0103);
    Simulator::Schedule(Seconds(3.0), &NetDevice::Send, devices.Get(0), packet3, addr1, 0x0103);
    Simulator::Schedule(Seconds(4.0), &NetDevice::Send, devices.Get(0), networkBeaconMessage, addr1, 0x0103);

    Simulator::Stop(Seconds(5.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}