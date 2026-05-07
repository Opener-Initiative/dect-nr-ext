#include "dect2020ext-mac-multiplexing-header-type-a.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacMultiplexingHeaderTypeA");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacMultiplexingHeaderTypeA);

Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtMacMultiplexingHeaderTypeA()
{
}

Dect2020ExtMacMultiplexingHeaderTypeA::~Dect2020ExtMacMultiplexingHeaderTypeA()
{
}

TypeId
Dect2020ExtMacMultiplexingHeaderTypeA::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacMultiplexingHeaderTypeA")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtMacMultiplexingHeaderTypeA>();
    return tid;
}

TypeId
Dect2020ExtMacMultiplexingHeaderTypeA::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeA::GetSerializedSize() const
{
    return 1;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::Serialize(Buffer::Iterator start) const
{
    // Byte 0
    uint8_t byte0 = 0;
    byte0 |= (m_macExtensionFieldEncoding & 0x03) << 6; // Bit 0-1
    byte0 |= (m_length & 0x01) << 5;                    // Bit 2
    byte0 |= (m_ieTypeFieldEncoding & 0x1F) << 0;       // Bit 3-7

    start.WriteU8(byte0);
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeA::Deserialize(Buffer::Iterator start)
{
    // Byte 0
    uint8_t byte0 = start.ReadU8();

    m_macExtensionFieldEncoding = (byte0 >> 6) & 0x03; // Bit 0-1
    m_length = (byte0 >> 5) & 0x01;                    // Bit 2
    m_ieTypeFieldEncoding = static_cast<IeTypeFieldEncoding>(byte0 & 0x1F); // Bit 3-7

    return 1;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::Print(std::ostream &os) const
{
    os << "Mac Extension Field Encoding: " << static_cast<int>(m_macExtensionFieldEncoding)
       << std::endl
       << "Length: " << static_cast<int>(m_length) << std::endl
       << "IE Type Field Encoding: " << static_cast<int>(m_ieTypeFieldEncoding) << std::endl;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetMacExtensionFieldEncoding(uint8_t encoding)
{
    if (encoding > 3)
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid MAC Extension Field encoding, must be at most 2 bits", encoding);
    }
    m_macExtensionFieldEncoding = encoding;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeA::GetMacExtensionFieldEncoding() const
{
    return m_macExtensionFieldEncoding;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetLengthField(bool length)
{
    m_length = length;
}

bool
Dect2020ExtMacMultiplexingHeaderTypeA::GetLengthField() const
{
    return m_length;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetIeTypeFieldEncoding(IeTypeFieldEncoding type)
{
    m_ieTypeFieldEncoding = type;
}


Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding
Dect2020ExtMacMultiplexingHeaderTypeA::GetIeTypeFieldEncoding() const
{
    return m_ieTypeFieldEncoding;
}

} // namespace ns3