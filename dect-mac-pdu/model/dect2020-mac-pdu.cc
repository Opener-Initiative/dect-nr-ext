#include "ns3/dect2020-mac-pdu.h"

namespace ns3
{

Dect2020MacPdu::Dect2020MacPdu(){}
Dect2020MacPdu::~Dect2020MacPdu(){}

void
Dect2020MacPdu::SetPduType(PduType type)
{
    m_pduType = type;
}

Dect2020MacPdu::PduType
Dect2020MacPdu::GetPduType() const
{
    return m_pduType;
}

void
Dect2020MacPdu::TypeAssigned()
{
    if(m_pduType != PduType::BROADCAST && m_pduType != PduType::MESH && m_pduType != PduType::UNICAST)
    {
        throw std::runtime_error("[ERROR] - MAC PDU type was not assigned!");
    }
}

// MAC Header Type | MAC Common Header | Network Beacon Message
Ptr<Packet>
Dect2020MacPdu::CreateBroadcastPdu()
{
    Ptr<Packet> packet = Create<Packet>();

    packet->AddHeader(m_macHeaderType);
    packet->AddHeader(m_beaconHeader); // MAC Common Header
    packet->AddHeader(m_beaconMessage); // Network Beacon Message

    return packet;
}

// MAC Header Type | MAC Common Header | Network Beacon Message | Route Info
Ptr<Packet>
Dect2020MacPdu::CreateMeshPdu()
{
    Ptr<Packet> packet = Create<Packet>();

    packet->AddHeader(m_macHeaderType);
    packet->AddHeader(m_beaconHeader); // MAC Common Header
    packet->AddHeader(m_beaconMessage); // Network Beacon Message
    packet->AddHeader(m_routeInfo); // Mesh Header

    return packet;
}

// MAC Header Type | MAC Unicast Header | Cluster Beacon Message ???
Ptr<Packet>
Dect2020MacPdu::CreateUnicastPdu()
{

}

Ptr<Packet>
Dect2020MacPdu::CreateMacPdu()
{

    Ptr<Packet> pdu = Create<Packet>();

    try
    {
        TypeAssigned();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    switch(m_pduType)
    {
        case BROADCAST:
            pdu = CreateBroadcastPdu();
            break;
        case MESH:
            pdu = CreateMeshPdu();
            break;
        case UNICAST:
            pdu = CreateUnicastPdu();
            break;
    }
    
    return pdu;
}

} // namespace ns3