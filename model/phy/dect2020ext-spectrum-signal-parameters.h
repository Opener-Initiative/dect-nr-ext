#ifndef DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS
#define DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS

#include "ns3/antenna-model.h"
#include "ns3/packet.h"
#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-signal-parameters.h"
#include "ns3/spectrum-value.h"

namespace ns3
{

class Dect2020ExtSpectrumSignalParameters : public SpectrumSignalParameters
{
  public:
    Dect2020ExtSpectrumSignalParameters();
    virtual ~Dect2020ExtSpectrumSignalParameters();
    Ptr<SpectrumSignalParameters> Copy() const override;

    Ptr<Packet> txPacket;
    uint32_t m_currentChannelNumber;
    bool m_data;
};

} // namespace ns3

#endif // DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS