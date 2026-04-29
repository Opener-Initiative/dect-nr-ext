#include "dect2020ext-rd-broadcasting-header.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRdBroadcastingHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtRdBroadcastingHeader);

Dect2020ExtRdBroadcastingHeader::Dect2020ExtRdBroadcastingHeader()
{
}

Dect2020ExtRdBroadcastingHeader::~Dect2020ExtRdBroadcastingHeader()
{
}

TypeId
Dect2020ExtRdBroadcastingHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtRdBroadcastingHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtRdBroadcastingHeader>();
    return tid;
}

TypeId
Dect2020ExtRdBroadcastingHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtRdBroadcastingHeader::Print(std::ostream &os) const
{
    os << "[DECT2020 RD BROADCASTING HEADER]" << "\n"
        << "Reset value: 0x" << std::hex << static_cast<uint32_t>(GetReset()) << "\n"
        << "Sequence Number: 0x" << std::hex << GetSequenceNumber() << "\n"
        << "Transmitter Address: 0x" << std::hex << GetTransmitterAddress() << "\n";
}

uint32_t Dect2020ExtRdBroadcastingHeader::GetSerializedSize() const
{
    return 6;
}

void Dect2020ExtRdBroadcastingHeader::Serialize(Buffer::Iterator start) const
{
    // byte 0
    // 3 first bits are reserved
    uint8_t byte0 = static_cast<uint8_t>(m_reset) << 4; 
    byte0 |= (m_sequenceNumber >> 8);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = (m_sequenceNumber & 0x0FF);
    start.WriteU8(byte1);

    // 4-byte transmitter address
    start.WriteHtonU32(m_transmitterAddress);
}

uint32_t Dect2020ExtRdBroadcastingHeader::Deserialize(Buffer::Iterator start)
{
    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_reset = ((byte0 >> 4) & 0x1);
    m_sequenceNumber = static_cast<uint16_t>((byte0 & 0x0F) << 8);

    // byte 1
    uint8_t byte1 = start.ReadU8();
    m_sequenceNumber |= byte1;

    // 4-byte transmitter address
    m_transmitterAddress = start.ReadNtohU32();

    return GetSerializedSize();
}

void Dect2020ExtRdBroadcastingHeader::SetReset(bool value)
{
    if(value != 0x0 && value != 0x1)
    {
        DECT2020EXT_MAC_PDU_FATAL("Reset can only be 0 or 1", value);
    }
    m_reset = value;
}

bool Dect2020ExtRdBroadcastingHeader::GetReset() const
{
    return m_reset;
}

void Dect2020ExtRdBroadcastingHeader::SetSequenceNumber(uint16_t sequenceNumber)
{
    if(sequenceNumber > 0xFFF)
    {
        DECT2020EXT_MAC_PDU_FATAL("Sequence Number can only be 12 bit long", sequenceNumber);
    }
    m_sequenceNumber = sequenceNumber;
}

uint16_t Dect2020ExtRdBroadcastingHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

void Dect2020ExtRdBroadcastingHeader::SetTransmitterAddress(uint32_t transmitterAddress)
{
    m_transmitterAddress = transmitterAddress;
}

uint32_t Dect2020ExtRdBroadcastingHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

} // namespace ns3