#ifndef DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS
#define DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS

#include "ns3/antenna-model.h"
#include "ns3/packet.h"
#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-signal-parameters.h"
#include "ns3/spectrum-value.h" // Include the header for SpectrumValue

/**
 * @file dect2020ext-spectrum-signal-parameters.h
 * @brief Defines signal parameter structure for DECT-2020 NR PHY layer transmissions.
 *
 * This header extends ns-3's SpectrumSignalParameters to include DECT-specific transmission
 * attributes such as the currently used channel ID and the pointer to the transmitted packet.
 *
 * It is used by the Dect2020ExtPhy layer to transmit packets via ns-3's SpectrumChannel framework.
 */

namespace ns3
{

/**
 * @class Dect2020ExtSpectrumSignalParameters
 * @brief Signal parameters used by the DECT-2020 EXT PHY layer in SpectrumChannel transmissions.
 *
 * This class inherits from ns3::SpectrumSignalParameters and encapsulates:
 * - The packet to be transmitted over the spectrum channel
 * - The DECT-2020 EXT specific channel ID on which the transmission takes place
 *
 * These parameters are passed to ns-3's SpectrumChannel::StartTx() method
 * and received by other PHY entities through StartRx().
 *
 * This structure allows DECT-2020 EXT devices to embed channel-specific transmission metadata
 * into the physical layer for RSSI calculation, collision detection, and subslot-based scheduling.
 */

class Dect2020ExtSpectrumSignalParameters : public SpectrumSignalParameters
{
  public:
    Dect2020ExtSpectrumSignalParameters();
    virtual ~Dect2020ExtSpectrumSignalParameters();
    virtual Ptr<SpectrumSignalParameters> Copy() const override;

    Ptr<Packet> txPacket;
    uint32_t m_currentChannelId;
    bool m_data;
};

} // namespace ns3

#endif // DECT2020EXT_SPECTRUM_SIGNAL_PARAMETERS