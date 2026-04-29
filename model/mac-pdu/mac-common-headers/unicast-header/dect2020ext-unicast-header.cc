#include "dect2020ext-unicast-header.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

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
Dect2020ExtUnicastHeader::Print(std::ostream& os) const
{
    os << "[DECT2020 UNICAST HEADER]" << "\n"
       << "Reset: 0x" << std::hex << static_cast<uint32_t>(m_reset) << "\n"
       << "MAC Sequence: 0x" << std::hex << static_cast<uint32_t>(m_macSequence) << "\n"
       << "Sequence Number: 0x" << std::hex << static_cast<uint32_t>(m_sequenceNumber) << "\n"
       << "Receiver Address: 0x" << std::hex << m_receiverAddress << "\n"
       << "Transmitter Address: 0x" << std::hex << m_transmitterAddress << std::dec;
}

uint32_t
Dect2020ExtUnicastHeader::GetSerializedSize() const
{
    return 10;
}

void
Dect2020ExtUnicastHeader::Serialize(Buffer::Iterator start) const
{
    // byte 0
    // 3 first bits are reserved
    uint8_t byte0 = static_cast<uint8_t>(m_reset) << 4;
    byte0 |= (m_macSequence & 0x0F);
    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_sequenceNumber);

    // 4-byte Receiver Address
    start.WriteHtonU32(m_receiverAddress);

    // 4-byte Transmitter Address
    start.WriteHtonU32(m_transmitterAddress);
}

uint32_t
Dect2020ExtUnicastHeader::Deserialize(Buffer::Iterator start)
{
    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_reset = ((byte0 >> 4) & 0x1);
    m_macSequence = byte0 & 0x0F;

    // byte 1
    m_sequenceNumber = start.ReadU8();

    // 4-byte Receiver Address
    m_receiverAddress = start.ReadNtohU32();

    // 4-byte Transmitter Address
    m_transmitterAddress = start.ReadNtohU32();

    return GetSerializedSize();
}

void
Dect2020ExtUnicastHeader::SetReset(bool reset)
{
    m_reset = reset;
}

bool
Dect2020ExtUnicastHeader::GetReset() const
{
    return m_reset;
}

void
Dect2020ExtUnicastHeader::SetMacSequence(uint8_t macSequence)
{
    if (macSequence > 0xF)
    {
        DECT2020EXT_MAC_PDU_FATAL("MAC Sequence must be at most 4 bits long",
                                  static_cast<uint32_t>(macSequence));
    }
    m_macSequence = macSequence;
}

uint8_t
Dect2020ExtUnicastHeader::GetMacSequence() const
{
    return m_macSequence;
}

void
Dect2020ExtUnicastHeader::SetSequenceNumber(uint8_t sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
}

uint8_t
Dect2020ExtUnicastHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

void
Dect2020ExtUnicastHeader::SetReceiverAddress(uint32_t receiverAddress)
{
    m_receiverAddress = receiverAddress;
}

uint32_t
Dect2020ExtUnicastHeader::GetReceiverAddress() const
{
    return m_receiverAddress;
}

void
Dect2020ExtUnicastHeader::SetTransmitterAddress(uint32_t transmitterAddress)
{
    m_transmitterAddress = transmitterAddress;
}

uint32_t
Dect2020ExtUnicastHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

} // namespace ns3
