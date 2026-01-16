#include "ns3/dect2020ext-execution-grid.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/node.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExample");

int
main(int argc, char *argv[])
{
    // LogComponentEnable("Dect2020ExtExecutionGrid", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));

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

    // PT Node
    Ptr<Node> ptNode = CreateObject<Node>();
    
    // FT PHY
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();
    ftPHY->SetChannel(channel);

    // PT PHY
    Ptr<Dect2020ExtPhy> ptPHY = CreateObject<Dect2020ExtPhy>();
    ptPHY->SetChannel(channel);

    // FT Execution Grid
    Ptr<Dect2020ExtExecutionGrid> ftExecutionGrid = CreateObject<Dect2020ExtExecutionGrid>();
    ftExecutionGrid->Initialize();

    // PT Execution Grid
    Ptr<Dect2020ExtExecutionGrid> ptExecutionGrid = CreateObject<Dect2020ExtExecutionGrid>();
    ptExecutionGrid->Initialize();

    // FT MAC
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetChannelManager(channelManager);
    ftMAC->SetPhy(ftPHY);
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftMAC->SetExecutionGrid(ftExecutionGrid);

    ftPHY->SetMac(ftMAC);

    ftExecutionGrid->SetMac(ftMAC);

    // PT MAC
    Ptr<Dect2020ExtMac> ptMAC = CreateObject<Dect2020ExtMac>();
    ptMAC->SetChannelManager(channelManager);
    ptMAC->SetPhy(ptPHY);
    ptMAC->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
    ptMAC->SetExecutionGrid(ptExecutionGrid);

    ptPHY->SetMac(ptMAC);

    ptExecutionGrid->SetMac(ptMAC);

    // FT Net Device
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);
    ftNetDevice->SetNode(ftNode);

    ftNode->AddDevice(ftNetDevice);

    // PT Net Device
    Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::PT);
    ptNetDevice->SetMac(ptMAC);
    ptNetDevice->SetPhy(ptPHY);
    ptNetDevice->SetNode(ptNode);

    ptNode->AddDevice(ptNetDevice);

    // Mobility Model
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(50.0, 50.0, 0.0));  // Position FT node at (50, 50)
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(ftNode);
    mobility.Install(ptNode);

    ftMAC->InitializeDevice();
    ptMAC->InitializeDevice();

    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtMac::Start, ftMAC);
    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtPhy::Start, ftPHY);
    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtExecutionGrid::StartExecutionGrid, ftExecutionGrid);

    Simulator::ScheduleWithContext(ptNode->GetId(), Seconds(0), &Dect2020ExtMac::Start, ptMAC);
    Simulator::ScheduleWithContext(ptNode->GetId(), Seconds(0), &Dect2020ExtPhy::Start, ptPHY);
    Simulator::ScheduleWithContext(ptNode->GetId(), Seconds(0), &Dect2020ExtExecutionGrid::StartExecutionGrid, ptExecutionGrid);

    Simulator::Stop(Seconds(5));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}