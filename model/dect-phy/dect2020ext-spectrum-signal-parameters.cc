#include "dect2020ext-spectrum-signal-parameters.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/spectrum-signal-parameters.h"

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
    // NS_LOG_INFO("Dect2020ExtSpectrumSignalParameters::Copy(): txPacket size = "
    //             << txPacket->GetSize() << ", UID = " << txPacket->GetUid());

    Ptr<Dect2020ExtSpectrumSignalParameters> copy = Create<Dect2020ExtSpectrumSignalParameters>();

    copy->duration = this->duration;
    copy->txPhy = this->txPhy;
    copy->txAntenna = this->txAntenna;
    copy->psd = this->psd ? this->psd->Copy() : nullptr;

    copy->txPacket = this->txPacket ? this->txPacket->Copy() : nullptr;
    copy->m_currentChannelId = m_currentChannelId;

    return copy;
}

} // namespace ns3