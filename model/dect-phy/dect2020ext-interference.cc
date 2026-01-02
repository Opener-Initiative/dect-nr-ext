#include "dect2020ext-interference.h"

#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtInterference::Dect2020ExtInterference()
{
}

Dect2020ExtInterference::~Dect2020ExtInterference()
{
}

TypeId
Dect2020ExtInterference::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtInterference").SetParent<Object>().SetGroupName("Dect2020Ext");
    return tid;
}

void
Dect2020ExtInterference::Initialize(Ptr<const SpectrumModel> spectrumModel)
{
    // Create an initial PSD (Power Spectral Density) entry for m_allSignals
    m_allSignals = Create<SpectrumValue>(spectrumModel);

    // Add a realistic noise floor for each channel (e.g -111 dBm per channel)
    // This corresponds to thermal noise: -174dBm/Hz + 10*log10(channel bandwidth)
    for (uint32_t i = 0; i < m_allSignals->GetSpectrumModel()->GetNumBands(); ++i)
    {
        double noiseDbm = -111.0;
        double noiseWatt = std::pow(10.0, 0.1 * (noiseDbm - 30.0)); // Convert dBm to linear Watt
        (*m_allSignals)[i] = noiseWatt; // Set the baseline noise power
    }
}

void
Dect2020ExtInterference::AddSignal(Ptr<const SpectrumValue> psd, const Time duration, uint16_t channelIndex)
{
    double powerToAdd = (*psd)[channelIndex];
    DoAddSignal(psd, channelIndex);
    Simulator::Schedule(duration, &Dect2020ExtInterference::DoSubtractSignal, this, powerToAdd, channelIndex);
}

void
Dect2020ExtInterference::DoAddSignal(Ptr<const SpectrumValue> psd, uint16_t channelIndex)
{
    // Check if m_allSinnals in initialized, if not -> initialize it with the same SpectrumModel as the psd
    if (m_allSignals == nullptr)
    {
        m_allSignals = psd->Copy();
    }
    else
    {
        (*m_allSignals)[channelIndex] += (*psd)[channelIndex];
    }
}

void
Dect2020ExtInterference::DoSubtractSignal(double powerToSubtract, uint16_t channelIndex)
{
    (*m_allSignals)[channelIndex] -= powerToSubtract;
}

} // namespace ns3