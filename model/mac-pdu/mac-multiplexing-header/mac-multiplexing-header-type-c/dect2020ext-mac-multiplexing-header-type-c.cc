#include "dect2020ext-mac-multiplexing-header-type-c.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacMultiplexingHeaderTypeC");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacMultiplexingHeaderTypeC);

TypeId
Dect2020ExtMacMultiplexingHeaderTypeC::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacMultiplexingHeaderTypeC")
        .SetParent<Header>()
        .AddConstructor<Dect2020ExtMacMultiplexingHeaderTypeC>()
    ;
    return tid;
}

Dect2020ExtMacMultiplexingHeaderTypeC::Dect2020ExtMacMultiplexingHeaderTypeC()
    : m_payload(nullptr)
{
}

Dect2020ExtMacMultiplexingHeaderTypeC::~Dect2020ExtMacMultiplexingHeaderTypeC()
{
    m_payload = nullptr;
}

void
Dect2020ExtMacMultiplexingHeaderTypeC::AddPayload(Header* payload, int payloadSize)
{
    m_payload = payload;
    m_payloadSize = payloadSize;
}

void
Dect2020ExtMacMultiplexingHeaderTypeC::RemovePayload()
{
    m_payload = nullptr;
    m_payloadSize = 0;
}

TypeId
Dect2020ExtMacMultiplexingHeaderTypeC::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtMacMultiplexingHeaderTypeC::SetIEType(ExtendedIEType IEType)
{
    m_IEType = (uint8_t)IEType;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeC::GetIEType() const
{
    return m_IEType;
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeC::GetSerializedSize() const
{
    if (m_payload == nullptr)
    {
        return 1; // Just the header byte
    }
    return m_payloadSize + 1;
}

void
Dect2020ExtMacMultiplexingHeaderTypeC::Serialize(Buffer::Iterator start) const
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
Dect2020ExtMacMultiplexingHeaderTypeC::Deserialize(Buffer::Iterator start)
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
Dect2020ExtMacMultiplexingHeaderTypeC::Print(std::ostream &os) const
{
    // TODO: Implement printing logic
    os << "Dect2020ExtMacMultiplexingHeaderTypeC";
}

} // namespace ns3
