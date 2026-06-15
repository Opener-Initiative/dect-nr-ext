#include "ns3/dect2020ext-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExample");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtMacScheduler", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME));
    LogComponentEnable("Dect2020ExtStatistics", LogLevel(LOG_LEVEL_INFO));

    NetDeviceContainer devices;

    // Configuration
    Dect2020ExtConfig config;

    Dect2020ExtAssociationControlIe associationControlIe;
    associationControlIe.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIe.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIe.SetUlPeriod(UlPeriod::S_10); // association maintenance period 10 seconds

    // Response window = (Max RACH Tx length * 2) + 3 (HARQ feedback) + 1 (additional subslot to cover propagation delay)
    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
    randomAccessResourceIe.SetRepeat(0);
    randomAccessResourceIe.SetSystemFrameNumberFieldIncluded(false);
    randomAccessResourceIe.SetChannelFieldIncluded(false);
    randomAccessResourceIe.SetSeparateChannelFieldIncluded(false);
    randomAccessResourceIe.SetStartSubslot(10);
    randomAccessResourceIe.SetLengthType(false); // RACH resources length in subslots
    randomAccessResourceIe.SetRaraLength(30);
    randomAccessResourceIe.SetMaxRachLengthType(false); // Max RACH Tx length in subslots
    randomAccessResourceIe.SetMaxRachLength(5); // in subslots
    randomAccessResourceIe.SetCwMinSig(1);
    randomAccessResourceIe.SetDectDelay(false);
    randomAccessResourceIe.SetResponseWindow(14); // in subslots
    randomAccessResourceIe.SetCwMaxSig(3);

    // ################
    // ###### FT ######
    // ################

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ftPhy);

    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
    ftMac->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftMac->SetPhy(ftPhy);
    ftMac->ConfigureRandomAccessResources(randomAccessResourceIe);
    // ftMac->ProceedToAssociationResponse(false);
    config.ApplyToMac(ftMac);

    ftPhy->SetMac(ftMac);

    Ptr<Node> ftNode = CreateObject<Node>();

    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
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
        // ptMac->ProceedToDataTransmission(false);
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

    ftMac->Initialize();
    ftMac->Start();
    ftPhy->Start();

    // Dummy MAC Scheduler to log frames
    Dect2020ExtMacScheduler dummyMacScheduler;
    dummyMacScheduler.Dummy(true);
    dummyMacScheduler.SetBandNumber(BAND_NUMBER);
    dummyMacScheduler.SetSubcarrierScalingFactor(SUBCARRIER_SCALING_FACTOR);
    dummyMacScheduler.SetFourierTransformScalingFactor(FOURIER_TRANSFORM_SCALING_FACTOR);
    dummyMacScheduler.SetChannelManager(config.m_channelManager);
    dummyMacScheduler.Initialize();
    dummyMacScheduler.StartMacScheduler();

    Simulator::Stop(Seconds(6));
    Simulator::Run();
    Simulator::Destroy();

    Dect2020ExtStatistics::OutputAllStatistics();
    CreatePythonConnectionGraph(devices);

    return 0;
}