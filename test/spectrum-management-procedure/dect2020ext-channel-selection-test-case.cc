#include "dect2020ext-channel-selection-test-case.h"

#include "ns3/dect2020ext-config.h"
#include "ns3/dect2020ext-utilities.h"
#include "ns3/dect2020ext-statistics.h"

#include "ns3/simulator.h"
#include "ns3/net-device-container.h"

namespace ns3
{

using DlDataReception = Dect2020ExtAssociationControlIe::DlDataReception;
using UlPeriod = Dect2020ExtAssociationControlIe::UlPeriod;

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

    // Information Elements
    Dect2020ExtConfig config;
    Dect2020ExtAssociationControlIe associationControlIe;
    associationControlIe.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIe.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIe.SetUlPeriod(UlPeriod::S_10); // association maintenance period 10 seconds

    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
    // randomAccessResourceIE.SetRepeat(0);
    // randomAccessResourceIE.SetSystemFrameNumberFieldIncluded(false);
    // randomAccessResourceIE.SetChannelFieldIncluded(false);
    // randomAccessResourceIE.SetSeparateChannelFieldIncluded(false);
    // randomAccessResourceIE.SetStartSubslot(10);
    // randomAccessResourceIE.SetLengthType(0); // length in subslots
    // randomAccessResourceIE.SetRaraLength(50);
    // randomAccessResourceIE.SetMaxRachLengthType(0); // length in subslots
    // randomAccessResourceIE.SetMaxRachLength(4); // Max 4 subslots (2 slots) / transmission
    // randomAccessResourceIE.SetCwMinSig(1);
    // randomAccessResourceIE.SetDectDelay(false);
    // randomAccessResourceIE.SetResponseWindow(5);
    // randomAccessResourceIE.SetCwMaxSig(5);

    // ################
    // ###### FT ######
    // ################

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ftPhy);

    Dect2020ExtRandomAccessResourceIe ftRandomAccessResources;

    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
    ftMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftMac->SetPhy(ftPhy);
    ftMac->ConfigureRandomAccessResources(ftRandomAccessResources);
    config.ApplyToMac(ftMac);

    ftPhy->SetMac(ftMac);

    Ptr<Node> ftNode = CreateObject<Node>();

    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftNetDevice->SetPhy(ftPhy);
    ftNetDevice->SetMac(ftMac);
    ftNetDevice->SetNode(ftNode);

    ftNode->AddDevice(ftNetDevice);

    InstallCenterMobilityModel(ftNode);
    ftPhy->SetMobility(ftNode->GetObject<MobilityModel>());

    devices.Add(ftNetDevice);

    // ################
    // ###### PT ######
    // ################

    int ptNumber = 2;
    NodeContainer ptNodes;
    Ptr<UniformRandomVariable> randomRadius = CreateObject<UniformRandomVariable>();
    randomRadius->SetAttribute("Min", DoubleValue(10.0));
    randomRadius->SetAttribute("Max", DoubleValue(50.0));

    for (int i = 0; i < ptNumber; i++)
    {
        Ptr<Dect2020ExtPhy> ptPhy = CreateObject<Dect2020ExtPhy>();
        config.ApplyToPhy(ptPhy);

        Ptr<Dect2020ExtMac> ptMac = CreateObject<Dect2020ExtMac>();
        ptMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptMac->SetPhy(ptPhy);
        ptMac->ConfigureAssociationControlIe(associationControlIe);
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

    ftMac->Initialize();
    ftMac->Start();
    ftPhy->Start();

    Simulator::Stop(Seconds(4));
    Simulator::Run();
    Simulator::Destroy();

    Dect2020ExtStatistics::OutputDroppedPackets();
    Dect2020ExtStatistics::OutputAcceptedPackets();
    CreatePythonConnectionGraph(devices);
}

} // namespace ns3