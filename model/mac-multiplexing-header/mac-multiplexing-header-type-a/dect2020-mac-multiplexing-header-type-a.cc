#include "dect2020-mac-multiplexing-header-type-a.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeA");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeA);

Dect2020MacMultiplexingHeaderTypeA::Dect2020MacMultiplexingHeaderTypeA()
{
    m_IEType = 0;
}

Dect2020MacMultiplexingHeaderTypeA::~Dect2020MacMultiplexingHeaderTypeA()
{
}

void
Dect2020MacMultiplexingHeaderTypeA::SetIEType(NoPayloadIEType ieType)
{
    m_IEType = ieType;
}

uint8_t
Dect2020MacMultiplexingHeaderTypeA::GetIEType() const
{
    return m_IEType;
}

TypeId
Dect2020MacMultiplexingHeaderTypeA::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeaderTypeA");
    return tid;
}

TypeId
Dect2020MacMultiplexingHeaderTypeA::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020MacMultiplexingHeaderTypeA::GetSerializedSize() const
{
    return 1;
}

void
Dect2020MacMultiplexingHeaderTypeA::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = (m_macExtension << 6) & 0xC0;
    byte0 |= (m_length << 5) & 0x20;
    byte0 |= (m_IEType & 0x1F);
    start.WriteU8(byte0);
}

uint32_t
Dect2020MacMultiplexingHeaderTypeA::Deserialize(Buffer::Iterator start)
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
Dect2020MacMultiplexingHeaderTypeA::Print(std::ostream &os) const
{
    os << "Dect2020MacMultiplexingHeaderTypeA: " << "MAC Extension: " << m_macExtension << " Length: " << m_length << " IE Type: " << m_IEType;
}

} // namespace ns3
