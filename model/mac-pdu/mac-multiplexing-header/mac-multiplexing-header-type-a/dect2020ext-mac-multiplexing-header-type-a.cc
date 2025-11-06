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

void
Dect2020ExtMacMultiplexingHeaderTypeA::SetIEType(ExtendedIEType ieType)
{
    m_IEType = ieType;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeA::GetIEType() const
{
    return m_IEType;
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
    // byte 0
    uint8_t byte0 = (m_macExtension << 6) & 0xC0;
    byte0 |= (m_length << 5) & 0x20;
    byte0 |= (m_IEType & 0x1F);
    start.WriteU8(byte0);
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeA::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;
    uint8_t byte0 = start.ReadU8();
    m_macExtension = (byte0 >> 6) & 0x03;
    m_length = (byte0 >> 5) & 0x01;
    m_IEType = byte0 & 0x1F;
    bytesRead = 1;

    return bytesRead;
}

void
Dect2020ExtMacMultiplexingHeaderTypeA::Print(std::ostream &os) const
{
    os << "Dect2020ExtMacMultiplexingHeaderTypeA: " << "MAC Extension: " << m_macExtension << " Length: " << m_length << " IE Type: " << m_IEType;
}

} // namespace ns3
