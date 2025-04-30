#ifndef DECT2020_MAC_PDU_H
#define DECT2020_MAC_PDU_H

#include "ns3/dect2020-mac-header-type.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-beacon-message.h"
#include "ns3/dect2020-route-info.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

namespace ns3
{

class Dect2020MacHeaderType;
class Dect2020BeaconHeader;
class Dect2020BeaconMessage;
class Dect2020RouteInfo;

class Dect2020MacPdu
{
    public:
        Dect2020MacPdu();
        ~Dect2020MacPdu();

        enum PduType
        {
            BROADCAST,
            MESH,
            UNICAST
        };

        // Setters, getters
        void SetPduType(PduType type);
        PduType GetPduType() const;

        // Error-checking
        void TypeAssigned();

        // Create BROADCAST PDU
        Ptr<Packet> CreateBroadcastPdu();

        // Create MESH PDU
        Ptr<Packet> CreateMeshPdu();

        // Create UNICAST PDU
        Ptr<Packet> CreateUnicastPdu();

        // Create MAC PDU
        Ptr<Packet> CreateMacPdu();

    private:
        Dect2020MacHeaderType m_macHeaderType;
        Dect2020BeaconHeader m_beaconHeader;
        Dect2020BeaconMessage m_beaconMessage;
        Dect2020RouteInfo m_routeInfo;
        PduType m_pduType;
};

}

#endif