#include "dect2020ext-data-mac-pdu-header.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020ExtDataMacPduHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtDataMacPduHeader);

Dect2020ExtDataMacPduHeader::Dect2020ExtDataMacPduHeader ()
{
}

Dect2020ExtDataMacPduHeader::~Dect2020ExtDataMacPduHeader ()
{
}

TypeId
Dect2020ExtDataMacPduHeader::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::Dect2020ExtDataMacPduHeader")
								.SetParent<Header> ()
								.SetGroupName("Dect2020Ext")
								.AddConstructor<Dect2020ExtDataMacPduHeader>();
	return tid;
}

TypeId
Dect2020ExtDataMacPduHeader::GetInstanceTypeId (void) const
{
	return GetTypeId();
}

uint32_t
Dect2020ExtDataMacPduHeader::GetSerializedSize (void) const
{
	return 2;
}

void
Dect2020ExtDataMacPduHeader::Serialize (Buffer::Iterator start) const
{
	// byte 0
	// 3 first bytes are reserved
    uint8_t byte0 = (m_reset << 4);
    byte0 |= (m_sequenceNumber >> 8);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = (m_sequenceNumber & 0x0FF);
    start.WriteU8(byte1);
}

uint32_t
Dect2020ExtDataMacPduHeader::Deserialize (Buffer::Iterator start)
{
    int bytesRead = 0;

    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_reset = ((byte0 >> 4) & 0x1);
    m_sequenceNumber = static_cast<uint16_t>((byte0 & 0x0F) << 8);

    bytesRead++;

    // byte 1
    uint8_t byte1 = start.ReadU8();
    m_sequenceNumber |= (byte1);

    bytesRead++;

	return bytesRead;
}

void
Dect2020ExtDataMacPduHeader::Print (std::ostream &os) const
{
    os << "[DATA MAC PDU HEADER]" << "\n"
        << "Reset value: 0x" << std::hex << (int)GetReset() << "\n"
        << "Sequence Number: 0x" << std::hex << (int)GetSequenceNumber() << "\n";
}

// Setters and Getters

void Dect2020ExtDataMacPduHeader::SetReset(uint8_t value)
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

uint8_t Dect2020ExtDataMacPduHeader::GetReset() const
{
    return m_reset;
}

void Dect2020ExtDataMacPduHeader::SetSequenceNumber(uint16_t sequenceNumber)
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

uint16_t Dect2020ExtDataMacPduHeader::GetSequenceNumber() const
{
    return m_sequenceNumber;
}

} // namespace ns3 