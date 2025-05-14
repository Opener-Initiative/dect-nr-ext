#include "dect2020-rd-broadcasting-header.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020RdBroadcastingHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020RdBroadcastingHeader);

Dect2020RdBroadcastingHeader::Dect2020RdBroadcastingHeader()
{
}

Dect2020RdBroadcastingHeader::~Dect2020RdBroadcastingHeader()
{
}

// Virtual Header base class methods

TypeId
Dect2020RdBroadcastingHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020RdBroadcastingHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020")
                            .AddConstructor<Dect2020RdBroadcastingHeader>();
    return tid;
}

TypeId
Dect2020RdBroadcastingHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t Dect2020RdBroadcastingHeader::GetSerializedSize() const
{
    return 6;
}

void Dect2020RdBroadcastingHeader::Serialize(Buffer::Iterator start) const
{
    // byte 0
    // 3 first 
    uint8_t byte0 = (m_reset << 4);
    byte0 |= (m_sequenceNumber >> 8);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = (m_sequenceNumber & 0x0FF);
    start.WriteU8(byte1);

    // 4-byte transmitter address
    start.WriteU32(m_transmitterAddress);
}

uint32_t Dect2020RdBroadcastingHeader::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_reset = ((byte0 >> 4) & 0x1);
    m_sequenceNumber = (byte0 & 0x0F);

    bytesRead++;

    // byte 1
    uint8_t byte1 = start.ReadU8();
    m_sequenceNumber |= (m_sequenceNumber << 8);
    m_sequenceNumber |= (byte1);

    bytesRead++;

    // 4-byte transmitter address
    m_transmitterAddress = start.ReadU32();

    bytesRead += 4;

    return bytesRead;
}

void
Dect2020RdBroadcastingHeader::Print(std::ostream &os) const
{
    os << "Reset value: 0x" << std::hex << (int)GetReset() << "\n";
    os << "Sequence Number: 0x" << std::hex << (int)GetSequenceNumber() << "\n";
    os << "Transmitter Address: 0x" << std::hex << (int)GetTransmitterAddress() << "\n";
}

// Setters and Getters

void Dect2020RdBroadcastingHeader::SetReset(uint8_t value)
{
    if(value != 0x0 && value != 0x1)
    {
        throw std::runtime_error("[ERROR] Reset value can only be zero or one");
    }
    else
    {
        m_reset = value;
    }
}

uint8_t Dect2020RdBroadcastingHeader::GetReset() const
{
    return m_reset;
}

void Dect2020RdBroadcastingHeader::SetSequenceNumber(uint16_t sequenceNumber)
{
    if((sequenceNumber & 0xF000) != 0)
    {
        throw std::runtime_error("[ERROR] Sequence Number can only be 12-bit long");
    }
    else
    {
        m_sequenceNumber = sequenceNumber;
    }
}

uint16_t Dect2020RdBroadcastingHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

void Dect2020RdBroadcastingHeader::SetTransmitterAddress(uint32_t transmitterAddress)
{
    m_transmitterAddress = transmitterAddress;
}

uint32_t Dect2020RdBroadcastingHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

} // namespace ns3

