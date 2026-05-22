// For each FT to finish background scan - 210 milliseconds

#include "dect2020ext-channel-selection-test-case.h"

#include "ns3/dect2020ext-config.h"
#include "ns3/dect2020ext-utilities.h"

#include "ns3/simulator.h"
#include "ns3/net-device-container.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannelSelectionTestCase");

Dect2020ExtChannelSelectionTestCase::Dect2020ExtChannelSelectionTestCase()
    : TestCase("Dect2020ExtChannelSelectionTestCase")
{
}

Dect2020ExtChannelSelectionTestCase::~Dect2020ExtChannelSelectionTestCase()
{
}

void
Dect2020ExtChannelSelectionTestCase::DoSetup()
{
}

void
Dect2020ExtChannelSelectionTestCase::DoRun()
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtStatistics", LogLevel(LOG_LEVEL_INFO));

    NetDeviceContainer devices;

    // Configuration
    Dect2020ExtConfig config;
    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
    randomAccessResourceIe.SetRepeat(0);
    randomAccessResourceIe.SetSystemFrameNumberFieldIncluded(false);
    randomAccessResourceIe.SetChannelFieldIncluded(false);
    randomAccessResourceIe.SetSeparateChannelFieldIncluded(false);
    randomAccessResourceIe.SetStartSubslot(10);
    randomAccessResourceIe.SetLengthType(false); // length in subslots
    randomAccessResourceIe.SetRaraLength(50);
    randomAccessResourceIe.SetMaxRachLengthType(false); // length in subslots
    randomAccessResourceIe.SetMaxRachLength(4); // Max 4 subslots (2 slots) / transmission
    randomAccessResourceIe.SetCwMinSig(1);
    randomAccessResourceIe.SetDectDelay(false);
    randomAccessResourceIe.SetResponseWindow(5);
    randomAccessResourceIe.SetCwMaxSig(5);

    // ################
    // ###### FT ######
    // ################

    int ftNumber = 21;
    NodeContainer ftNodes;
    Ptr<UniformRandomVariable> randomRadius = CreateObject<UniformRandomVariable>();
    randomRadius->SetAttribute("Min", DoubleValue(10.0));
    randomRadius->SetAttribute("Max", DoubleValue(50.0));
    Time baseStartDelay = MilliSeconds(0);

    for (int i = 0; i < ftNumber; i++)
    {
        Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
        config.ApplyToPhy(ftPhy);

        Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
        ftMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
        ftMac->SetPhy(ftPhy);
        ftMac->ConfigureRandomAccessResources(randomAccessResourceIe);
        ftMac->ProceedToAssociation(false); // IMPORTANT FOR THIS TEST
        ftMac->DisplayChannelEvaluations(true); // IMPORTANT FOR THIS TEST
        config.ApplyToMac(ftMac);

        ftPhy->SetMac(ftMac);

        Ptr<Node> ptNode = CreateObject<Node>();

        Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
        ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
        ftNetDevice->SetPhy(ftPhy);
        ftNetDevice->SetMac(ftMac);
        ftNetDevice->SetNode(ptNode);

        ptNode->AddDevice(ftNetDevice);

        InstallCircularMobilityModel(i, ftNumber, ptNode, randomRadius);
        ftPhy->SetMobility(ptNode->GetObject<MobilityModel>());

        devices.Add(ftNetDevice);

        ftMac->Initialize();
        Simulator::Schedule(baseStartDelay, &Dect2020ExtMac::Start, ftMac);
        Simulator::Schedule(baseStartDelay, &Dect2020ExtPhy::Start, ftPhy);
        baseStartDelay += MilliSeconds(210);
    }

    Simulator::Stop(MilliSeconds(210 * ftNumber));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3