#include "dect2020ext-data-mac-pdu-header.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtDataMacPduHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtDataMacPduHeader);

Dect2020ExtDataMacPduHeader::Dect2020ExtDataMacPduHeader()
{
}

Dect2020ExtDataMacPduHeader::~Dect2020ExtDataMacPduHeader()
{
}

TypeId
Dect2020ExtDataMacPduHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtDataMacPduHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtDataMacPduHeader>();
    return tid;
}

TypeId
Dect2020ExtDataMacPduHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtDataMacPduHeader::Print(std::ostream& os) const
{
    os << "[DECT2020 DATA MAC PDU HEADER]" << "\n"
       << "Reset: 0x" << std::hex << static_cast<uint32_t>(m_reset) << "\n"
       << "Sequence Number: 0x" << std::hex << static_cast<uint32_t>(m_sequenceNumber) << std::dec;
}

uint32_t
Dect2020ExtDataMacPduHeader::GetSerializedSize() const
{
    return 2;
}

void
Dect2020ExtDataMacPduHeader::Serialize(Buffer::Iterator start) const
{
    // byte 0
    // 3 first bits are reserved
    uint8_t byte0 = static_cast<uint8_t>(m_reset) << 4;
    byte0 |= (m_sequenceNumber >> 8);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = (m_sequenceNumber & 0x0FF);
    start.WriteU8(byte1);
}

uint32_t
Dect2020ExtDataMacPduHeader::Deserialize(Buffer::Iterator start)
{
    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_reset = ((byte0 >> 4) & 0x1);
    m_sequenceNumber = static_cast<uint16_t>((byte0 & 0x0F) << 8);

    // byte 1
    uint8_t byte1 = start.ReadU8();
    m_sequenceNumber |= (byte1);

    return GetSerializedSize();
}

void
Dect2020ExtDataMacPduHeader::SetReset(bool reset)
{
    m_reset = reset;
}

bool
Dect2020ExtDataMacPduHeader::GetReset() const
{
    return m_reset;
}

void
Dect2020ExtDataMacPduHeader::SetSequenceNumber(uint16_t sequenceNumber)
{
    if (sequenceNumber > 0xFFF)
    {
        DECT2020EXT_MAC_PDU_FATAL("Sequence Number must be at most 12 bits long", sequenceNumber);
    }
    m_sequenceNumber = sequenceNumber;
}

uint16_t
Dect2020ExtDataMacPduHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

} // namespace ns3
