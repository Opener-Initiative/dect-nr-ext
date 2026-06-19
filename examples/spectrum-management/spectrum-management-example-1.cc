// Spectrum Management Procedure: Example 1 - operating channel selection for 2 FTs
// 1. Establish communication for 1 FT and 1 PT
// 2. Launch another FT
// Goal: determine which channel is selected by two FT devices if communication is already
// happening on one channel
// OutputSelectedOperatingChannels is supposed to output:
// FT_MAC[0]: 1657
// FT_MAC[1]: 1658
// PT_MAC[2]: 1657

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

    // ################
    // ###### FT ######
    // ################

    int ftNumber = 2;
    NodeContainer ftNodes;
    Ptr<UniformRandomVariable> randomFtRadius = CreateObject<UniformRandomVariable>();
    randomFtRadius->SetAttribute("Min", DoubleValue(0.0));
    randomFtRadius->SetAttribute("Max", DoubleValue(1.0));
    Time startDelay = MilliSeconds(300);

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

        InstallCircularMobilityModel(i, ftNumber, ftNode, randomFtRadius);
        ftPhy->SetMobility(ftNode->GetObject<MobilityModel>());

        devices.Add(ftNetDevice);

        ftMac->Initialize();

        Simulator::Schedule(startDelay * i, &Dect2020ExtMac::Start, ftMac);
        Simulator::Schedule(startDelay * i, &Dect2020ExtPhy::Start, ftPhy);
    }

    // ################
    // ###### PT ######
    // ################

    int ptNumber = 1;
    NodeContainer ptNodes;
    Ptr<UniformRandomVariable> randomRadius = CreateObject<UniformRandomVariable>();
    randomRadius->SetAttribute("Min", DoubleValue(1.0));
    randomRadius->SetAttribute("Max", DoubleValue(5.0));

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

        InstallCircularMobilityModel(i, ptNumber, ptNode, randomRadius);
        ptPhy->SetMobility(ptNode->GetObject<MobilityModel>());

        devices.Add(ptNetDevice);

        ptMac->Initialize();
        ptMac->Start();
        ptPhy->Start();
    }

    Simulator::Stop(Seconds(6));
    Simulator::Run();
    Simulator::Destroy();

    Dect2020ExtStatistics::OutputAllStatistics();
    CreatePythonConnectionGraph(devices);
    OutputSelectedOperatingChannels(devices);

    return 0;
}