#ifndef DECT2020EXT_MAC_PDU_HELPER_H
#define DECT2020EXT_MAC_PDU_HELPER_H

#include "ns3/dect2020ext-mac-header-type.h"

#include "ns3/packet.h"

namespace ns3
{

typedef Dect2020ExtMacHeaderType::MacSecurityField MacSecurityField;

/**
 * Helper class which helps to create MAC PDUs
 */
class Dect2020ExtMacPduHelper
{
    public:
        Dect2020ExtMacPduHelper();
        ~Dect2020ExtMacPduHelper();

        /**
         * Creates MAC PDU object with:
         * 
         * - MAC Header type field and MAC Common header as DATA MAC PDU header
         * 
         * - MAC Header type version set to 0
         * 
         * Other header values are supplied through the function's parameters
         * @param security MAC security info
         * @param reset DATA MAC PDU reset value
         * @param sequenceNumber DATA MAC PDU sequence number
         * @returns Ptr<Packet> A smart pointer to the created packet (MAC PDU)
         */
        static Ptr<Packet> DATAMACPDU(MacSecurityField security,
                                        uint8_t reset,
                                        uint16_t sequenceNumber);

        /**
         * Creates MAC PDU object with:
         * 
         * - MAC Header type field and MAC Common header as Beacon Header
         * 
         * - MAC Header type version set to 0
         * 
         * Other header values are supplied through the function's parameters
         * @param security MAC security info
         * @param networkId Beacon Header network ID
         * @param transmitterAddress Beacon Header transmitter address
         * @returns Ptr<Packet> A smart pointer to the created packet (MAC PDU)
         */
        static Ptr<Packet> BEACON(MacSecurityField security,
                                    uint32_t networkId,
                                    uint32_t transmitterAddress);

        /**
         * Creates MAC PDU object with:
         * 
         * - MAC Header type field and MAC Common header as Unicast Header
         * 
         * - MAC Header type version set to 0
         * 
         * Other header values are supplied through the function's parameters
         * @param security MAC security info
         * @param macSequence Unicast Header MAC sequence number
         * @param sequenceNumber Unicast Header sequence number
         * @param receiverAddress Unicast Header receiver address
         * @param transmitterAddress Unicast Header transmitter address
         * @returns Ptr<Packet> A smart pointer to the created packet (MAC PDU)
         */
        static Ptr<Packet> UNICAST(MacSecurityField security,
                                    uint8_t macSequence,
                                    uint8_t sequenceNumber,
                                    uint32_t receiverAddress,
                                    uint32_t transmitterAddress);

        /**
         * Creates MAC PDU object with:
         * 
         * - MAC Header type field and MAC Common header as RD Broadcasting Header
         * 
         * - MAC Header type version set to 0
         * 
         * Other header values are supplied through the function's parameters
         * @param security MAC security info
         * @param reset RD Broadcasting Header reset value
         * @param sequenceNumber RD Broadcasting Header sequence number
         * @param transmitterAddress RD Broadcasting Header transmitter address
         * @returns Ptr<Packet> A smart pointer to the created packet (MAC PDU)
         */
        static Ptr<Packet> RDBROADCASTING(MacSecurityField security,
                                            uint8_t reset,
                                            uint16_t sequenceNumber,
                                            uint32_t transmitterAddress);
};

} // namespace ns3

#endif // DECT2020EXT_MAC_PDU_HELPER_H