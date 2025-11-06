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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSpectrumManagementProcedure");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtMac", LOG_LEVEL_ALL);
    // LogComponentEnable("Dect2020ExtMac", LOG_PREFIX_TIME);

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

    // FT PHY
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();
    ftPHY->SetChannel(channel);
    // ftPHY->SetChannelManager(channelManager);

    // FT MAC
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetPhy(ftPHY);
    // ftMAC->SetChannelManager(channelManager);
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::FT);

    ftPHY->SetMac(ftMAC);

    // FT Net Device
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);

    // FT Node
    Ptr<Node> ftNode = CreateObject<Node>();
    ftNode->AddDevice(ftNetDevice);

    ftMAC->Start();

    Simulator::Stop(Seconds(20));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}