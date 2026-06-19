#include "dect2020ext-config.h"
#include "dect2020ext-config-values.h"

#include "ns3/propagation-delay-model.h"
#include "ns3/config.h"
#include "ns3/pointer.h"

namespace ns3
{

using DlDataReception = ns3::Dect2020ExtAssociationControlIe::DlDataReception;
using UlPeriod = ns3::Dect2020ExtAssociationControlIe::UlPeriod;

Dect2020ExtConfig::Dect2020ExtConfig()
{
    m_spectrumChannel = CreateConfiguredSpectrumChannel();
    m_channelManager = CreateConfiguredChannelManager();
}

Dect2020ExtConfig::~Dect2020ExtConfig()
{
}

Ptr<SPECTRUM_CHANNEL_TYPE>
Dect2020ExtConfig::CreateConfiguredSpectrumChannel()
{
    Ptr<SPECTRUM_CHANNEL_TYPE> spectrumChannel = CreateObject<SPECTRUM_CHANNEL_TYPE>();
    spectrumChannel->SetPropagationDelayModel(CreateObject<PROPAGATION_DELAY_MODEL>());
    spectrumChannel->AddPropagationLossModel(CreateObject<PROPAGATION_LOSS_MODEL>());

    return spectrumChannel;
}

Ptr<Dect2020ExtChannelManager>
Dect2020ExtConfig::CreateConfiguredChannelManager()
{
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(BAND_NUMBER);
    channelManager->SetSubcarrierScalingFactor(SUBCARRIER_SCALING_FACTOR);
    channelManager->SetFourierTransformScalingFactor(FOURIER_TRANSFORM_SCALING_FACTOR);

    return channelManager;
}

Ptr<IsotropicAntennaModel>
Dect2020ExtConfig::CreateConfiguredIsotropicAntennaModel()
{
    Ptr<IsotropicAntennaModel> antenna = CreateObject<IsotropicAntennaModel>();

    if (RX_GAIN_INDEX > 8)
    {
        NS_ABORT_MSG("Invalid RX gain" << static_cast<int>(RX_GAIN_INDEX));
    }

    DoubleValue rxGain = static_cast<int8_t>(-10 + RX_GAIN_INDEX * 2);
    antenna->SetAttribute("Gain", rxGain);

    return antenna;
}

void
Dect2020ExtConfig::ApplyToPhy(Ptr<Dect2020ExtPhy> phy)
{
    phy->SetBandNumber(BAND_NUMBER);
    phy->SetSubcarrierScalingFactor(SUBCARRIER_SCALING_FACTOR);
    phy->SetFourierTransformScalingFactor(FOURIER_TRANSFORM_SCALING_FACTOR);
    phy->SetChannel(m_spectrumChannel);
    phy->SetChannelManager(m_channelManager);
    phy->SetAntenna(CreateConfiguredIsotropicAntennaModel());
}

void
Dect2020ExtConfig::ApplyToMac(Ptr<Dect2020ExtMac> mac)
{
    mac->SetBandNumber(BAND_NUMBER);
    mac->SetSubcarrierScalingFactor(SUBCARRIER_SCALING_FACTOR);
    mac->SetFourierTransformScalingFactor(FOURIER_TRANSFORM_SCALING_FACTOR);
    mac->SetChannelManager(m_channelManager);
}

Dect2020ExtRandomAccessResourceIe
Dect2020ExtConfig::GetDefaultRandomAccessResourceIe()
{
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
    randomAccessResourceIe.SetMaxRachLength(5); // enough length for common MAC PDU Messages in subslots
    randomAccessResourceIe.SetCwMinSig(1);
    randomAccessResourceIe.SetDectDelay(false);
    randomAccessResourceIe.SetResponseWindow(14); // in subslots
    randomAccessResourceIe.SetCwMaxSig(3);

    return randomAccessResourceIe;
}

Dect2020ExtAssociationControlIe
Dect2020ExtConfig::GetDefaultAssociationControlIe()
{
    Dect2020ExtAssociationControlIe associationControlIe;
    associationControlIe.SetCbM(0); // maintain Cluster Beacon reception
    associationControlIe.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIe.SetUlPeriod(UlPeriod::S_10); // association maintenance period 10 seconds

    return associationControlIe;
}

} // namespace ns3