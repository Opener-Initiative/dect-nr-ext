#include "dect2020-unicast-header.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020UnicastHeader");

NS_OBJECT_ENSURE_REGISTERED(Dect2020UnicastHeader);

Dect2020UnicastHeader::Dect2020UnicastHeader()
{
}

Dect2020UnicastHeader::~Dect2020UnicastHeader()
{
}

void
Dect2020UnicastHeader::SetMacSequence(uint32_t number)
{
    m_macSequence = number;
}

uint32_t
Dect2020UnicastHeader::GetMacSequence() const
{
    return m_macSequence;
}

void
Dect2020UnicastHeader::SetSequenceNumber(uint8_t number)
{
    m_sequenceNumber = number;
}

uint8_t
Dect2020UnicastHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

void
Dect2020UnicastHeader::SetReceiverAddress(uint32_t address)
{
    m_receiverAddress = address;
}

uint32_t
Dect2020UnicastHeader::GetReceiverAddress() const
{
    return m_receiverAddress;
}

void
Dect2020UnicastHeader::SetTransmitterAddress(uint32_t address)
{
    m_transmitterAddress = address;
}

uint32_t
Dect2020UnicastHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

TypeId
Dect2020UnicastHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020UnicastHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020")
                            .AddConstructor<Dect2020UnicastHeader>();
    return tid; 
}

TypeId
Dect2020UnicastHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020UnicastHeader::Print(std::ostream &os) const
{
    os << "Dect2020UnicastHeader";
}

uint32_t
Dect2020UnicastHeader::GetSerializedSize() const
{
    return 10;
}

void
Dect2020UnicastHeader::Serialize(Buffer::Iterator start) const
{
    // Byte 0
    uint8_t byte0 = 0;
    // Bit 0-2 reserved
    // Bit 3 reset
    byte0 |= m_macSequence;

    start.WriteU8(byte0); // MAC Sequence
    start.WriteU8(m_sequenceNumber); // Sequence Number
    start.WriteU32(m_receiverAddress); // Receiver Address
    start.WriteU32(m_transmitterAddress); // Transmitter Address
}

uint32_t
Dect2020UnicastHeader::Deserialize(Buffer::Iterator start)
{
    // Byte 0
    uint8_t byte0 = 0;
    byte0 = start.ReadU8();
    m_macSequence = byte0;
    m_sequenceNumber = start.ReadU8();
    m_receiverAddress = start.ReadU32();
    m_receiverAddress = start.ReadU32();

    return 10;
}

    
} // namespace ns3