#include "dect2020ext-mac-multiplexing-header-type-a.h"

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
Dect2020ExtMacMultiplexingHeaderTypeA::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacMultiplexingHeaderTypeA");
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
    m_ieTypeFieldEncoding = (byte0 & 0x1F);            // Bit 3-7

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

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeA::GetMacExtensionFieldEncoding() const
{
    return m_macExtensionFieldEncoding;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetMacExtensionFieldEncoding(uint8_t encoding)
{
    m_ieTypeFieldEncoding = encoding;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeA::GetLengthField() const
{
    return m_length;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetLengthField(uint8_t length)
{
    m_length = length & 0x01;
}

Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding
Dect2020ExtMacMultiplexingHeaderTypeA::GetIeTypeFieldEncoding() const
{
    return static_cast<Dect2020ExtIETypeFieldEncoding>(m_ieTypeFieldEncoding);
}



void
Dect2020ExtMacMultiplexingHeaderTypeA::SetIeTypeFieldEncoding(Dect2020ExtIETypeFieldEncoding type)
{
    m_ieTypeFieldEncoding = static_cast<uint8_t>(type);
}

} // namespace ns3
