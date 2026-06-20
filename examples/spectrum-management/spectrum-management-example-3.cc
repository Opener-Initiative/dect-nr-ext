// Spectrum Management Procedure: Example 4 - operating channel selection for 4 FTs during background scan
// Create a square mobility model where each square edge is FT device
// OutputSelectedOperatingChannels is supposed to output:

#include "ns3/dect2020ext-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExample");

int
main(int argc, char *argv[])
{
    // LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    // LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    // LogComponentEnable("Dect2020ExtMacScheduler", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME));
    LogComponentEnable("Dect2020ExtStatistics", LogLevel(LOG_LEVEL_INFO));

    NetDeviceContainer devices;

    // Configuration
    Dect2020ExtConfig config;

    Time startFtDelay = MilliSeconds(300); // one background scan duration
    Time startPtDelay = MilliSeconds(300);

    // ################
    // ###### FT ######
    // ################

    int ftNumber = 4;
    NodeContainer ftNodes;
    NetDeviceContainer ftDevices;
    Ptr<UniformRandomVariable> randomFtRadius = CreateObject<UniformRandomVariable>();
    randomFtRadius->SetAttribute("Min", DoubleValue(0.0));
    randomFtRadius->SetAttribute("Max", DoubleValue(1.0));

    for (int i = 0; i < ftNumber; i++)
    {
        Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
        config.ApplyToPhy(ftPhy);

        Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
        ftMac->SetTerminationPointType(Dect2020ExtNetDevice::FT);
        ftMac->SetPhy(ftPhy);
        ftMac->ConfigureRandomAccessResources(config.GetDefaultRandomAccessResourceIe());
        ftMac->DisplayChannelEvaluations(true);
        config.ApplyToMac(ftMac);

        ftPhy->SetMac(ftMac);

        Ptr<Node> ftNode = CreateObject<Node>();

        Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
        ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
        ftNetDevice->SetPhy(ftPhy);
        ftNetDevice->SetMac(ftMac);
        ftNetDevice->SetNode(ftNode);

        ftNode->AddDevice(ftNetDevice);
        ftNodes.Add(ftNode);

        devices.Add(ftNetDevice);
        ftDevices.Add(ftNetDevice);

        ftMac->Initialize();

        Simulator::Schedule(startFtDelay * i, &Dect2020ExtMac::Start, ftMac);
        Simulator::Schedule(startFtDelay * i, &Dect2020ExtPhy::Start, ftPhy);
    }

    // ################
    // ###### PT ######
    // ################

    int ptNumber = 10;
    NodeContainer ptNodes;
    Ptr<UniformRandomVariable> randomRadius = CreateObject<UniformRandomVariable>();
    randomRadius->SetAttribute("Min", DoubleValue(0.0));
    randomRadius->SetAttribute("Max", DoubleValue(1.0));

    for (int i = 0; i < ptNumber; i++)
    {
        Ptr<Dect2020ExtPhy> ptPhy = CreateObject<Dect2020ExtPhy>();
        config.ApplyToPhy(ptPhy);

        Ptr<Dect2020ExtMac> ptMac = CreateObject<Dect2020ExtMac>();
        ptMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptMac->SetPhy(ptPhy);
        ptMac->ConfigureAssociationControlIe(config.GetDefaultAssociationControlIe());
        ptMac->SetSearchBestFtTimeMs(10);
        config.ApplyToMac(ptMac);

        ptPhy->SetMac(ptMac);

        Ptr<Node> ptNode = CreateObject<Node>();

        Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
        ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptNetDevice->SetPhy(ptPhy);
        ptNetDevice->SetMac(ptMac);
        ptNetDevice->SetNode(ptNode);

        ptNode->AddDevice(ptNetDevice);

        InstallCircularMobilityModel(i, ptNumber, ptNode, randomRadius, Vector(0.0, 0.0, 0.0));
        ptPhy->SetMobility(ptNode->GetObject<MobilityModel>());

        devices.Add(ptNetDevice);

        ptMac->Initialize();
        
        Simulator::Schedule(startPtDelay * i, &Dect2020ExtMac::Start, ptMac);
        Simulator::Schedule(startPtDelay * i, &Dect2020ExtPhy::Start, ptPhy);
    }

    // Place FT1 at -2.5, -2.5, 0
    InstallConstantMobilityModel(ftNodes.Get(0), Vector(-2.5, -2.5, 0.0));
    Ptr<Dect2020ExtNetDevice> ftDevice1 = DynamicCast<Dect2020ExtNetDevice>(ftDevices.Get(0));
    ftDevice1->GetPhy()->SetMobility(ftNodes.Get(0)->GetObject<MobilityModel>());

    // Place FT2 at 2.5, 2.5, 0
    InstallConstantMobilityModel(ftNodes.Get(1), Vector(2.5, 2.5, 0.0));
    Ptr<Dect2020ExtNetDevice> ftDevice2 = DynamicCast<Dect2020ExtNetDevice>(ftDevices.Get(1));
    ftDevice2->GetPhy()->SetMobility(ftNodes.Get(1)->GetObject<MobilityModel>());

    // Place FT3 at -2.5, 2.5, 0
    InstallConstantMobilityModel(ftNodes.Get(2), Vector(-2.5, 2.5, 0.0));
    Ptr<Dect2020ExtNetDevice> ftDevice3 = DynamicCast<Dect2020ExtNetDevice>(ftDevices.Get(2));
    ftDevice3->GetPhy()->SetMobility(ftNodes.Get(2)->GetObject<MobilityModel>());

    // Place FT4 at 2.5, -2.5, 0
    InstallConstantMobilityModel(ftNodes.Get(3), Vector(2.5, -2.5, 0.0));
    Ptr<Dect2020ExtNetDevice> ftDevice4 = DynamicCast<Dect2020ExtNetDevice>(ftDevices.Get(3));
    ftDevice4->GetPhy()->SetMobility(ftNodes.Get(3)->GetObject<MobilityModel>());

    Simulator::Stop(Seconds(6));
    Simulator::Run();
    Simulator::Destroy();

    Dect2020ExtStatistics::OutputAllStatistics();
    CreatePythonConnectionGraph(devices);
    OutputSelectedOperatingChannels(devices);

    return 0;
}