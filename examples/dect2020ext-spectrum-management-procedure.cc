#include "ns3/dect2020ext-types.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/mobility-helper.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSpectrumManagementProcedure");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtChannelManager", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtNetDevice", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    
    // Spectrum Channel
    Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();
    channel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());
    channel->AddPropagationLossModel(CreateObject<LogDistancePropagationLossModel>());

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->SetSpectrumChannel(channel);
    channelManager->Configure();
    channelManager->InitializeChannels();

    // FT and PT Nodes
    Ptr<Node> ftNode = CreateObject<Node>();
    Ptr<Node> ptNode = CreateObject<Node>();

    // FT and PT PHYs
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();
    ftPHY->SetChannel(channel);
    Ptr<Dect2020ExtPhy> ptPHY = CreateObject<Dect2020ExtPhy>();
    ptPHY->SetChannel(channel);

    // FT and PT MACs
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetPhy(ftPHY);
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    Ptr<Dect2020ExtMac> ptMAC = CreateObject<Dect2020ExtMac>();
    ptMAC->SetPhy(ptPHY);
    ptMAC->SetTerminationPointType(Dect2020ExtNetDevice::PT);

    ftPHY->SetMac(ftMAC);
    ptPHY->SetMac(ptMAC);

    // FT and PT Net Devices
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);
    ftNetDevice->SetNode(ftNode);
    Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::PT);
    ptNetDevice->SetMac(ptMAC);
    ptNetDevice->SetPhy(ptPHY);
    ptNetDevice->SetNode(ptNode);

    ptNode->AddDevice(ptNetDevice);
    ftNode->AddDevice(ftNetDevice);
  
    // Mobility Model
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(50.0, 50.0, 0.0));  // Position FT node at (50, 50)
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(ftNode);
    mobility.Install(ptNode);

    ftMAC->Start();

    // NetAnim Configuration
    AnimationInterface anim("dect2020ext-spectrum-management.xml");
    anim.EnablePacketMetadata(true);
    
    // Set node descriptions for easier identification
    anim.UpdateNodeDescription(ftNode, "FT-Node");
    anim.UpdateNodeDescription(ptNode, "PT-Node");
    
    // Set node colors (FT=Blue, PT=Red)
    anim.UpdateNodeColor(ftNode, 0, 0, 255);    // Blue for FT
    anim.UpdateNodeColor(ptNode, 255, 0, 0);    // Red for PT
    
    // Set node sizes for better visibility
    anim.UpdateNodeSize(ftNode->GetId(), 5.0, 5.0);
    anim.UpdateNodeSize(ptNode->GetId(), 5.0, 5.0);

    Simulator::Stop(Seconds(5));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}