#include "dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeC");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeC);

TypeId
Dect2020MacMultiplexingHeaderTypeC::GetTypeId(void)
{
    TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeaderTypeC")
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
    // No need to delete m_payload as it's owned by the caller
    m_payload = nullptr;
}

TypeId
Dect2020MacMultiplexingHeaderTypeC::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020MacMultiplexingHeaderTypeC::InsertSDU(const Header &header)
{
    uint32_t size = header.GetSerializedSize();
    m_buffer.AddAtStart(size);
    header.Serialize(m_buffer.Begin());
}

uint8_t
Dect2020MacMultiplexingHeaderTypeC::RemoveSDU(Header &header)
{
    uint32_t deserialized = header.Deserialize(m_buffer.Begin());
    m_buffer.RemoveAtStart(deserialized);

    return deserialized;
}

uint8_t
Dect2020MacMultiplexingHeaderTypeC::GetSDU() const
{
    // TODO: Implement SDU retrieval logic
    return 0;
}

void
Dect2020MacMultiplexingHeaderTypeC::SetIEType(ExtendedIEType IEType)
{
    m_IEType = IEType;
}

uint8_t
Dect2020MacMultiplexingHeaderTypeC::GetIEType() const
{
    return m_IEType;
}

uint32_t
Dect2020MacMultiplexingHeaderTypeC::GetSerializedSize() const
{
    return m_buffer.GetSize() + 1;
}

void
Dect2020MacMultiplexingHeaderTypeC::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = (m_macExtension & 0x03) << 6;
    byte0 |= (m_IEType & 0x3F);
    start.WriteU8(byte0);

    // SDU

}

uint32_t
Dect2020MacMultiplexingHeaderTypeC::Deserialize(Buffer::Iterator start)
{
    // TODO: Implement deserialization logic
    return 0;
}

void
Dect2020MacMultiplexingHeaderTypeC::Print(std::ostream &os) const
{
    // TODO: Implement printing logic
    os << "Dect2020MacMultiplexingHeaderTypeC";
}

} // namespace ns3
