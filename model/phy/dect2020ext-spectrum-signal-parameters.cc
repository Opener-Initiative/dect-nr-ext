#include "dect2020ext-spectrum-signal-parameters.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/spectrum-signal-parameters.h"
#include "ns3/mobility-model.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSpectrumSignalParameters");

Dect2020ExtSpectrumSignalParameters::Dect2020ExtSpectrumSignalParameters()
    : SpectrumSignalParameters()
{
}

Dect2020ExtSpectrumSignalParameters::~Dect2020ExtSpectrumSignalParameters()
{
}

Ptr<SpectrumSignalParameters>
Dect2020ExtSpectrumSignalParameters::Copy() const
{
    Ptr<Dect2020ExtSpectrumSignalParameters> copy = Create<Dect2020ExtSpectrumSignalParameters>();

    copy->duration = this->duration;
    copy->txPhy = this->txPhy;
    copy->txAntenna = this->txAntenna;
    copy->psd = this->psd ? this->psd->Copy() : nullptr;
    copy->txMobility = this->txMobility;

    copy->m_txPacket = this->m_txPacket ? this->m_txPacket->Copy() : nullptr;
    copy->m_currentChannelNumber = m_currentChannelNumber;
    copy->m_random = m_random;

    return copy;
}

} // namespace ns3