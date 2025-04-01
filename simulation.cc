#include "dect-channel.h"
#include "dect-phy.h"

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/ssid.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/mobility-helper.h"
#include "ns3/application-container.h"
#include "ns3/udp-echo-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("DECTSimulation");

int
main(int argc, char* argv[])
{
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

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
    phy.SetErrorRateModel("ns3::TableBasedErrorRateModel");
    phy.SetChannel(channel.Create());

    // Configure MAC
    WifiHelper wifi;
    wifi.SetRemoteStationManager("ns3::IdealWifiManager");

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns3-ssid");
    mac.SetType("ns3::StaWifiMac",
    "Ssid", SsidValue(ssid),
    "ActiveProbing", BooleanValue(false));

    NetDeviceContainer staDevice = wifi.Install(phy, mac, staNode);

    mac.SetType("ns3::ApWifiMac",
    "Ssid", SsidValue(ssid));

    NetDeviceContainer apDevice = wifi.Install(phy, mac, apNode);

    // Install the internet stack
    InternetStackHelper stack;
    stack.Install(apNode);
    stack.Install(staNode);

    // Assign IP addresses to the devices

    Ipv4AddressHelper address;
    address.SetBase("10.0.0.0", "255.255.255.0");

    Ipv4InterfaceContainer staInterface = address.Assign(staDevice);
    Ipv4InterfaceContainer apInterface = address.Assign(apDevice);

    // Configure mobility
    
    GridPositionAllocator positionAllocator;
    positionAllocator.SetN(3);
    positionAllocator.SetLayoutType(GridPositionAllocator::LayoutType::ROW_FIRST);
    positionAllocator.SetMinX(0.0);
    positionAllocator.SetMinY(0.0);
    positionAllocator.SetDeltaX(5.0);
    positionAllocator.SetDeltaY(5.0);

    // Configure mobility model
    MobilityHelper mobility;
    
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
    "MinX", DoubleValue(0.0),
    "MinY", DoubleValue(0.0),
    "DeltaX", DoubleValue(5.0),
    "DeltaY", DoubleValue(5.0),
    "GridWidth", UintegerValue(3),
    "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(staNode.Get(0));
    mobility.Install(apNode.Get(0));

    // Set up applications
    uint16_t port = 9;

    UdpEchoServerHelper echoServer(port);
    ApplicationContainer serverApp = echoServer.Install(apNode.Get(0));

    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(apInterface.GetAddress(0), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp = echoClient.Install(staNode.Get(0));

    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(10.0));

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();
    return 0;

}
