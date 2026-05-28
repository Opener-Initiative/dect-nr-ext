#include "dect2020ext-phy-test-case-1.h"

#include "ns3/dect2020ext-module.h"
#include "ns3/dect2020ext-spectrum-signal-parameters.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhyTestCase1");

Dect2020ExtPhyTestCase1::Dect2020ExtPhyTestCase1()
    : TestCase("Dect2020ExtPhyTestCase1")
{
}

Dect2020ExtPhyTestCase1::~Dect2020ExtPhyTestCase1()
{
}

void
Dect2020ExtPhyTestCase1::DoSetup()
{
}

void
Dect2020ExtPhyTestCase1::DoRun()
{
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_DEBUG | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));

    Dect2020ExtConfig config;
    Dect2020ExtAssociationControlIE associationControlIE;
    associationControlIE.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIE.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIE.SetUlPeriod(ULPeriod::S_10); // association maintenance period 10 seconds

    // ################
    // ###### FT ######
    // ################

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ftPhy);
    ftPhy->m_phyTest = true;

    Dect2020ExtRandomAccessResourceIE ftRandomAccessResources;

    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
    ftMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftMac->SetPhy(ftPhy);
    ftMac->ConfigureRandomAccessResources(ftRandomAccessResources);
    ftMac->ConfigureAssociationControlIE(associationControlIE);
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
    ftMac->Start();
    ftPhy->Start();

    Ptr<Dect2020ExtPhy> txPhy = CreateObject<Dect2020ExtPhy>();
    Time duration = Time(MicroSeconds(500));

    // Create and initialize the transmission parameters
    Ptr<Dect2020ExtSpectrumSignalParameters> params = Create<Dect2020ExtSpectrumSignalParameters>();
    params->txPhy = txPhy;
    params->txPacket = Create<Packet>(10);
    params->m_currentChannelNumber = 1657;
    params->duration = duration;

    double txPowerWatt = ftMac->GetChannelManager()->DbmToW(ftNetDevice->m_txPowerDbm);

    // Create PSD and initialize
    Ptr<const SpectrumModel> spectrumModel = ftMac->GetChannelManager()->GetSpectrumModel(1);
    Ptr<SpectrumValue> psd = Create<SpectrumValue>(spectrumModel);
    
    for (uint32_t i = 0; i < psd->GetSpectrumModel()->GetNumBands(); ++i)
    {
        (*psd)[i] = 0.0; // Initialize all channels to 0.0
    }

    // Set power only on the current channel
    (*psd)[0] = txPowerWatt;
    params->psd = psd;

    for (int i = 0; i < 1; i++)
    {
        Simulator::Schedule(MilliSeconds(210), &SpectrumChannel::StartTx, ftPhy->GetChannel(), params);
    }

    Simulator::Stop(Seconds(1));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3
