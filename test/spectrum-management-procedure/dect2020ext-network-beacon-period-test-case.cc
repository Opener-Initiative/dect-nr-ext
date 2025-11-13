#include "dect2020ext-network-beacon-period-test-case.h"

#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/node.h"
#include "ns3/waveform-generator.h"
#include "ns3/spectrum-value.h"
#include "ns3/spectrum-model.h"
#include "ns3/mobility-module.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtNetworkBeaconPeriodTestCase");

Dect2020ExtNetworkBeaconPeriodTestCase::Dect2020ExtNetworkBeaconPeriodTestCase()
    : TestCase("Dect2020ExtNetworkBeaconPeriodTestCase")
{
}

Dect2020ExtNetworkBeaconPeriodTestCase::~Dect2020ExtNetworkBeaconPeriodTestCase()
{
}

void
Dect2020ExtNetworkBeaconPeriodTestCase::DoSetup()
{
}

static void EnableLogging()
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_DEBUG | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
}

void
Dect2020ExtNetworkBeaconPeriodTestCase::DoRun()
{
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

    // FT Node
    Ptr<Node> ftNode = CreateObject<Node>();

    // FT PHY
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();
    ftPHY->SetChannel(channel);

    // FT MAC
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetPhy(ftPHY);
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::FT);

    ftPHY->SetMac(ftMAC);

    // FT Net Device
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);
    ftNetDevice->SetNode(ftNode);
    
    ftNode->AddDevice(ftNetDevice);
  
    // Mobility Model
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(50.0, 50.0, 0.0));  // Position FT node at (50, 50)
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(ftNode);

    Simulator::Schedule(Seconds(1.21), &EnableLogging);

    ftMAC->Start();

    Simulator::Stop(Seconds(2));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3