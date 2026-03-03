/*
Random Access Test Case 1 (mu = 1, beta = 1)

Random Access Resource IE:
Start subslot = 10
Length type = 0 (length in subslots)
Length = 50 (50 subslots)
MAX RACH Len type = 0 (MAX length in subslots)
MAX RACH Length = 4 (MAX 4 subslots or 2 slots per transmission)
Cwmin_sig = 1
DECT_Delay = false
Response window = 5 (subslots)
Cwmax_sig = 5

CW_MIN = 8 * Cwmin_sig = 8
GI_US = 18.52; // GI in microseconds GI = (4/9) * T_u_symb
STF_US = 64.815; // STF = (14/9) * T_u_symb
STF_US + GI_US = LBT_MINIMUM_PERIOD_US = 83.335
LBT_MINIMUM_PERIOD_MS = 0.083335

Correct flow of simulation:
+0.010000000s InitiateRandomAccess -> wait till subslot 10
+0.012083330s InitiateBackoff -> selected random backoff value = 8
+0.012749330s PerformLBT
+0.012749330s Dect2020ExtMac:SampleLBTRSSI()
+0.012781330s Dect2020ExtMac:SampleLBTRSSI()
+0.012813330s Dect2020ExtMac:SampleLBTRSSI()
+0.012823330s Dect2020ExtMac:SampleLBTRSSI()
+0.012832330s Dect2020ExtMac:SampleLBTRSSI()
+0.012832330s Dect2020ExtMac:EvaluateLBTChannel(): Re-initiating backoff with doubled CW_CURRENT -> the channel is busy (avg RSSI: 40 dBm)
+0.012832330s Dect2020ExtMac:InitiateBackoff(): PT_MAC[0]: Initiating backoff at subslot - 13, position - 9
+0.012832330s Dect2020ExtMac:InitiateBackoff(): PT_MAC[0]: Random backoff value: 15
+0.012832330s Dect2020ExtMac:InitiateBackoff(): PT_MAC[0]: Target position time us: 2000.04
+0.012832330s Dect2020ExtMac:InitiateBackoff(): PT_MAC[0]: Current positioin time us: 750.015
+0.012832330s Dect2020ExtMac:InitiateBackoff(): PT_MAC[0]: Backoff duration us: 1250.03
+0.014082330s Dect2020ExtMac:PerformLBT(): PT_MAC[0]: Performing LBT at subslot 19, position - 24
+0.014082330s Dect2020ExtMac:PerformLBT(): PT_MAC[0]: LBT at position 24 (elapsed: 1999 us)
+0.014082330s Dect2020ExtMac:SampleLBTRSSI(): PT_MAC[0]: Sampling LBT RSSI
+0.014114330s Dect2020ExtMac:SampleLBTRSSI(): PT_MAC[0]: Sampling LBT RSSI
+0.014146330s Dect2020ExtMac:SampleLBTRSSI(): PT_MAC[0]: Sampling LBT RSSI
+0.014156330s Dect2020ExtMac:SampleLBTRSSI(): PT_MAC[0]: Sampling LBT RSSI
+0.014165330s Dect2020ExtMac:SampleLBTRSSI(): PT_MAC[0]: Sampling LBT RSSI
+0.014165330s Dect2020ExtMac:EvaluateLBTChannel(): PT_MAC[0]: Evaluating LBT channel at subslot 19, position - 25
+0.014165330s Dect2020ExtMac:EvaluateLBTChannel(): PT_MAC[0]: LBT RSSI - Average: -111.484 dBm, Max: -111.484 dBm
+0.014165330s Dect2020ExtMac:CheckIfMacPduFitsInMaxRandomAccessTxTime(): MAC PDU size: 3 subslots
+0.014165330s Dect2020ExtMac:EvaluateLBTChannel(): PT_MAC[0]: Transmitting -> power on channel is -111.484 dBm

Result: the RD should re-initiate the backoff process due to occupied channel and then transmit when the channel is free
*/

#include "dect2020ext-random-access-test-case2.h"

#include "ns3/dect2020ext-module.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRandomAccessTestCase2");

Dect2020ExtRandomAccessTestCase2::Dect2020ExtRandomAccessTestCase2()
    : TestCase("Dect2020ExtRandomAccessTestCase2")
{
}

Dect2020ExtRandomAccessTestCase2::~Dect2020ExtRandomAccessTestCase2()
{
}

void
Dect2020ExtRandomAccessTestCase2::DoSetup()
{
}

void
Dect2020ExtRandomAccessTestCase2::DoRun()
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_DEBUG | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_DEBUG | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));

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
    ptMac->ConfigureRngStream(0);
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

    Ptr<SpectrumModel> spectrumModel = ptMac->GetChannelManager()->GetSpectrumModel(1);
    Ptr<SpectrumValue> psd = Create<SpectrumValue>(spectrumModel);

    for (uint32_t i = 0; i < psd->GetSpectrumModel()->GetNumBands(); ++i)
    {
        (*psd)[i] = 10.0; // initialize all channels to 10
    }
    Time duration = Time(MilliSeconds(3));

    Simulator::Schedule(MilliSeconds(10), &Dect2020ExtMac::InitiateRandomAccess, ptMac, randomAccessResourceIE, packet, physicalHeaderField);

    uint16_t channelIndex = 0;

    Simulator::Schedule(MilliSeconds(10), [ptPhy, psd, duration, channelIndex]() {
        ptPhy->m_interference->AddSignal(psd, duration, channelIndex+1);
    });

    ptMac->Start();
    ptPhy->Start();

    ftPhy->Start();

    Simulator::Stop(Seconds(0.5));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3

