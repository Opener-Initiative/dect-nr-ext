#include "dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeC");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeC);

TypeId
Dect2020MacMultiplexingHeaderTypeC::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeaderTypeC")
        .SetParent<Header>()
        .AddConstructor<Dect2020MacMultiplexingHeaderTypeC>()
    ;
    return tid;
}

Dect2020MacMultiplexingHeaderTypeC::Dect2020MacMultiplexingHeaderTypeC()
    : m_payload(nullptr)
{
}

Dect2020MacMultiplexingHeaderTypeC::~Dect2020MacMultiplexingHeaderTypeC()
{
    delete m_payload;
}

void
Dect2020MacMultiplexingHeaderTypeC::AddPayload(Header* payload, int payloadSize)
{
    if (m_payload != nullptr)
    {
        delete m_payload;
    }
    m_payload = payload;
    m_payloadSize = payloadSize;
}

void
Dect2020MacMultiplexingHeaderTypeC::RemovePayload()
{
    delete m_payload;
    m_payload = nullptr;
    m_payloadSize = 0;
}

TypeId
Dect2020MacMultiplexingHeaderTypeC::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020MacMultiplexingHeaderTypeC::SetIEType(ExtendedIEType IEType)
{
    m_IEType = (uint8_t)IEType;
}

uint8_t
Dect2020MacMultiplexingHeaderTypeC::GetIEType() const
{
    return m_IEType;
}

uint32_t
Dect2020MacMultiplexingHeaderTypeC::GetSerializedSize() const
{
    if (m_payload == nullptr)
    {
        return 1; // Just the header byte
    }
    return m_payloadSize + 1;
}

void
Dect2020MacMultiplexingHeaderTypeC::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = (m_macExtension & 0x03) << 6;
    byte0 |= (m_IEType & 0x3F);
    start.WriteU8(byte0);

    // SDU
    if (m_payload != nullptr)
    {
        m_payload->Serialize(start);
    }
}

uint32_t
Dect2020MacMultiplexingHeaderTypeC::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;
    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_macExtension = (byte0 >> 6) & 0x03;
    m_IEType = byte0 & 0x3F;
    bytesRead++;

    // SDU
    bytesRead += m_payload->Deserialize(start);

    return bytesRead;
}

void
Dect2020MacMultiplexingHeaderTypeC::Print(std::ostream &os) const
{
    // TODO: Implement printing logic
    os << "Dect2020MacMultiplexingHeaderTypeC";
}

} // namespace ns3
