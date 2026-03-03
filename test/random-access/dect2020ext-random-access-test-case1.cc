#include "dect2020ext-random-access-test-case1.h"

#include "ns3/dect2020ext-module.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRandomAccessTestCase1");

Dect2020ExtRandomAccessTestCase1::Dect2020ExtRandomAccessTestCase1()
    : TestCase("Dect2020ExtRandomAccessTestCase1")
{
}

Dect2020ExtRandomAccessTestCase1::~Dect2020ExtRandomAccessTestCase1()
{
}

void
Dect2020ExtRandomAccessTestCase1::DoSetup()
{
}

void
Dect2020ExtRandomAccessTestCase1::DoRun()
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));

    // ################
    // ###### PT ######
    // ################

    Dect2020ExtConfig config;

    Ptr<Dect2020ExtPhy> ptPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ptPhy);

    Dect2020ExtAssociationControlIE associationControlIE;
    associationControlIE.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIE.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIE.SetUlPeriod(ULPeriod::S_10); // association maintenance period 10 seconds

    Ptr<Dect2020ExtMac> ptMac = CreateObject<Dect2020ExtMac>();
    ptMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
    ptMac->SetPhy(ptPhy);
    ptMac->ConfigureAssociationControlIE(associationControlIE);
    config.ApplyToMac(ptMac);

    ptPhy->SetMac(ptMac);

    Ptr<Node> ptNode = CreateObject<Node>();

    Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
    ptNetDevice->SetPhy(ptPhy);
    ptNetDevice->SetMac(ptMac);
    ptNetDevice->SetNode(ptNode);

    ptNode->AddDevice(ptNetDevice);

    InstallCenterMobilityModel(ptNode);
    ptPhy->SetMobility(ptNode->GetObject<MobilityModel>());

    ptMac->Initialize();

    // ################
    // ###### FT ######
    // ################

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ftPhy);

    Dect2020ExtRandomAccessResourceIE ftRandomAccessResources;

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

    ftMac->Initialize();

    Ptr<Packet> packet = ftMac->BuildClusterBeacon(1657);
    Dect2020ExtRandomAccessResourceIE randomAccessResourceIE = ftMac->BuildRandomAccessResourceIE();
    Dect2020ExtPHYControlFieldType1 physicalHeaderField = ftMac->BuildPhysicalHeaderField(1, packet->GetSize());

    Simulator::Schedule(MilliSeconds(10), &Dect2020ExtMac::InitiateRandomAccess, ptMac, randomAccessResourceIE, packet, physicalHeaderField);

    ptMac->Start();
    ptPhy->Start();

    ftPhy->Start();

    Simulator::Stop(Seconds(1));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3

