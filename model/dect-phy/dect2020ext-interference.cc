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