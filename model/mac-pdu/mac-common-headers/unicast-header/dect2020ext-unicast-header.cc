#include "dect2020ext-unicast-header.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtUnicastHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtUnicastHeader);

Dect2020ExtUnicastHeader::Dect2020ExtUnicastHeader()
{
}

Dect2020ExtUnicastHeader::~Dect2020ExtUnicastHeader()
{
}

void
Dect2020ExtUnicastHeader::SetMacSequence(uint8_t number)
{
    if((number >> 4) != 0)
    {
        throw std::runtime_error("[ERROR] Argument of SetMacSequence function is longer than 4 bits!");
    } 
    m_macSequence = number;
}

uint8_t
Dect2020ExtUnicastHeader::GetMacSequence() const
{
    return m_macSequence;
}

void
Dect2020ExtUnicastHeader::SetSequenceNumber(uint8_t number)
{
    m_sequenceNumber = number;
}

uint8_t
Dect2020ExtUnicastHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

void
Dect2020ExtUnicastHeader::SetReceiverAddress(uint32_t address)
{
    m_receiverAddress = address;
}

uint32_t
Dect2020ExtUnicastHeader::GetReceiverAddress() const
{
    return m_receiverAddress;
}

void
Dect2020ExtUnicastHeader::SetTransmitterAddress(uint32_t address)
{
    m_transmitterAddress = address;
}

uint32_t
Dect2020ExtUnicastHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

TypeId
Dect2020ExtUnicastHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtUnicastHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtUnicastHeader>();
    return tid; 
}

TypeId
Dect2020ExtUnicastHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020ExtUnicastHeader::Print(std::ostream &os) const
{
    os << "[DECT2020 UNICAST HEADER]" << "\n"
        << "MAC Sequence:" << std::hex << m_macSequence
        << "Sequence number:" << std::hex << m_sequenceNumber
        << "Receiver address:" << std::hex << m_receiverAddress
        << "Transmitter address:" << std::hex << m_transmitterAddress;
}

uint32_t
Dect2020ExtUnicastHeader::GetSerializedSize() const
{
    return 10;
}

void
Dect2020ExtUnicastHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU8(m_macSequence); // MAC Sequence
    start.WriteU8(m_sequenceNumber); // Sequence Number
    start.WriteU32(m_receiverAddress); // Receiver Address
    start.WriteU32(m_transmitterAddress); // Transmitter Address
}

uint32_t
Dect2020ExtUnicastHeader::Deserialize(Buffer::Iterator start)
{
    m_macSequence = start.ReadU8();
    m_sequenceNumber = start.ReadU8();
    m_receiverAddress = start.ReadU32();
    m_transmitterAddress = start.ReadU32();

    return 10;
}

    
} // namespace ns3