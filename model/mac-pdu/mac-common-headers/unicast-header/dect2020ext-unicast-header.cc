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

TypeId
Dect2020ExtUnicastHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtUnicastHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtUnicastHeader>();
    return tid; 
}

TypeId
Dect2020ExtUnicastHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtUnicastHeader::Print(std::ostream &os) const
{
    os << "[DECT2020 UNICAST HEADER]" << "\n"
        << "Reset: " << m_reset << "\n"
        << "MAC Sequence: 0x" << std::hex << static_cast<uint32_t>(m_macSequence) << "\n"
        << "Sequence number: 0x" << std::hex << static_cast<uint32_t>(m_sequenceNumber) << "\n"
        << "Receiver address: 0x" << std::hex << m_receiverAddress << "\n"
        << "Transmitter address: 0x" << std::hex << m_transmitterAddress << std::dec;
}

uint32_t
Dect2020ExtUnicastHeader::GetSerializedSize() const
{
    return 10;
}

void
Dect2020ExtUnicastHeader::Serialize(Buffer::Iterator start) const
{
    // Byte 0: Pack reset (1 bit) and MAC Sequence (4 bits)
    uint8_t byte0 = 0;
    byte0 |= (m_reset ? 1 : 0) << 7;  // Bit 7: Reset
    byte0 |= (m_macSequence & 0x0F);  // Bits 3-0: MAC Sequence
    start.WriteU8(byte0);
    
    start.WriteU8(m_sequenceNumber); // Sequence Number
    start.WriteHtonU32(m_receiverAddress); // Receiver Address
    start.WriteHtonU32(m_transmitterAddress); // Transmitter Address
}

uint32_t
Dect2020ExtUnicastHeader::Deserialize(Buffer::Iterator start)
{
    // Byte 0: Unpack reset (1 bit) and MAC Sequence (4 bits)
    uint8_t byte0 = start.ReadU8();
    m_reset = (byte0 >> 7) & 0x01;  // Bit 7: Reset
    m_macSequence = byte0 & 0x0F;   // Bits 3-0: MAC Sequence
    
    m_sequenceNumber = start.ReadU8();
    m_receiverAddress = start.ReadNtohU32();
    m_transmitterAddress = start.ReadNtohU32();

    return 10;
}

void
Dect2020ExtUnicastHeader::SetReset(bool reset)
{
    if(reset != 0x0 && reset != 0x1)
    {
        NS_FATAL_ERROR("[ERROR] Reset can only be 0 ir 1, got: " << reset);
    }
    m_reset = reset;
}

bool
Dect2020ExtUnicastHeader::GetReset() const
{
    return m_reset;
}

void
Dect2020ExtUnicastHeader::SetMacSequence(uint8_t number)
{
    if(number > 0xF)
    {
        NS_FATAL_ERROR("[ERROR] MAC Sequence can only be 4 bit long, got: " << number);
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
    
} // namespace ns3