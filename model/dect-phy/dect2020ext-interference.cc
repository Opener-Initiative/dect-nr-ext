#include "dect2020ext-interference.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtInterference");

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
    NS_LOG_FUNCTION(this);
    
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
    
    NS_LOG_INFO("Initialized interference model with " << m_allSignals->GetSpectrumModel()->GetNumBands() 
                << " channels, noise floor: -111 dBm for " << m_mac);
}

void
Dect2020ExtInterference::AddSignal(Ptr<const SpectrumValue> psd, const Time duration, uint16_t channelIndex)
{
    NS_LOG_FUNCTION(this << channelIndex << duration);
    
    double powerToAdd = (*psd)[channelIndex];
    double powerDbm = 10.0 * std::log10(powerToAdd) + 30.0;
    
    NS_LOG_INFO(m_mac << ": channelIndex=" << channelIndex 
                << ", power=" << powerDbm << " dBm (" << powerToAdd << " W)"
                << ", duration=" << duration.As(Time::MS));
    
    DoAddSignal(psd, channelIndex);
    Simulator::Schedule(duration, &Dect2020ExtInterference::DoSubtractSignal, this, powerToAdd, channelIndex);
}

void
Dect2020ExtInterference::DoAddSignal(Ptr<const SpectrumValue> psd, uint16_t channelIndex)
{
    NS_LOG_FUNCTION(this << channelIndex);
    
    // Check if m_allSignals is initialized, if not -> initialize it with the same SpectrumModel as the psd
    if (m_allSignals == nullptr)
    {
        m_allSignals = psd->Copy();
        NS_LOG_INFO("DoAddSignal: Initialized m_allSignals from PSD");
    }
    else
    {
        double powerBefore = (*m_allSignals)[channelIndex];
        double powerBeforeDbm = 10.0 * std::log10(powerBefore) + 30.0;
        
        (*m_allSignals)[channelIndex] += (*psd)[channelIndex];
        
        double powerAfter = (*m_allSignals)[channelIndex];
        double powerAfterDbm = 10.0 * std::log10(powerAfter) + 30.0;
        
        NS_LOG_INFO("DoAddSignal: channelIndex=" << channelIndex 
                    << ", powerBefore=" << powerBeforeDbm << " dBm"
                    << ", powerAfter=" << powerAfterDbm << " dBm"
                    << " (added " << (*psd)[channelIndex] << " W)");
    }
}

void
Dect2020ExtInterference::DoSubtractSignal(double powerToSubtract, uint16_t channelIndex)
{
    NS_LOG_FUNCTION(this << powerToSubtract << channelIndex);
    
    double powerBefore = (*m_allSignals)[channelIndex];
    double powerBeforeDbm = 10.0 * std::log10(powerBefore) + 30.0;
    
    (*m_allSignals)[channelIndex] -= powerToSubtract;
    
    double powerAfter = (*m_allSignals)[channelIndex];
    double powerAfterDbm = 10.0 * std::log10(powerAfter) + 30.0;
    
    NS_LOG_INFO("DoSubtractSignal: channelIndex=" << channelIndex 
                << ", powerBefore=" << powerBeforeDbm << " dBm"
                << ", powerAfter=" << powerAfterDbm << " dBm"
                << " (removed " << powerToSubtract << " W)");
}

} // namespace ns3