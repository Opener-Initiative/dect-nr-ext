#ifndef DECT2020_MAC_PDU_HELPER_H
#define DECT2020_MAC_PDU_HELPER_H

#include "ns3/packet.h"

namespace ns3
{

class Dect2020MacPduHelper
{
    public:
        Dect2020MacPduHelper();
        ~Dect2020MacPduHelper();

        /**
         * Creates MAC PDU object with:
         * 
         * MAC header Type field and MAC Common header as DATA MAC PDU header
         * @returns Ptr<Packet> A smart pointer to the created packet
         */
        static Ptr<Packet> DATAMACPDU();

        /**
         * Creates MAC PDU object with:
         * 
         * MAC header Type field and MAC Common header as Beacon Header
         * @returns Ptr<Packet> A smart pointer to the created packet
         */
        static Ptr<Packet> BEACON();

        /**
         * Creates MAC PDU object with:
         * 
         * MAC header Type field and MAC Common header as Unicast Header
         * @returns Ptr<Packet> A smart pointer to the created packet
         */
        static Ptr<Packet> UNICAST();

        /**
         * Creates MAC PDU object with:
         * 
         * MAC header Type field and MAC Common header as RD Broadcasting Header
         * @returns Ptr<Packet> A smart pointer to the created packet
         */
        static Ptr<Packet> RDBROADCASTING();
};

} // namespace ns3

#endif // DECT2020_MAC_PDU_HELPER_H