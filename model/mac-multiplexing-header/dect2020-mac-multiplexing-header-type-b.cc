#include "dect2020-mac-multiplexing-header-type-b.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeB");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeB);

Dect2020MacMultiplexingHeaderTypeB::Dect2020MacMultiplexingHeaderTypeB()
{
}

Dect2020MacMultiplexingHeaderTypeB::~Dect2020MacMultiplexingHeaderTypeB()
{
}

// Insert a payload into the SDU
void
Dect2020MacMultiplexingHeaderTypeB::InsertSDU(const uint8_t payload)
{
    uint32_t size = 1;
    m_buffer.AddAtStart(size);

    // TODO: Figure out if byte tag list is needed
    //m_byteTagList.Adjust(size);
    //m_byteTagList.AddAtStart(size);

    m_buffer.Begin().WriteU8(payload);
}

// Remove the payload from the SDU
uint8_t
Dect2020MacMultiplexingHeaderTypeB::RemoveSDU()
{
    uint8_t payload = m_buffer.Begin().ReadU8();
    m_buffer.RemoveAtStart(1);

    // TODO: Figure out if byte tag list is needed
    //m_byteTagList.Adjust(-deserialized);

    return payload;
}

TypeId
Dect2020MacMultiplexingHeaderTypeB::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeaderTypeB")
        .SetParent<Header>()
        .AddConstructor<Dect2020MacMultiplexingHeaderTypeB>();
    return tid;
}

TypeId
Dect2020MacMultiplexingHeaderTypeB::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020MacMultiplexingHeaderTypeB::GetSerializedSize() const
{
    return 2; // 1 byte for header, 1 byte for SDU
}

void
Dect2020MacMultiplexingHeaderTypeB::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = (m_macExtension & 0x03) << 6;
    byte0 |= (m_length & 0x01) << 5;
    byte0 |= (m_IEType & 0x1F);
    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_buffer.Begin().ReadU8());
}

uint32_t
Dect2020MacMultiplexingHeaderTypeB::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    // byte 0
    uint8_t byte0 = start.ReadU8();
    m_macExtension = (byte0 >> 6) & 0x03;
    m_length = (byte0 >> 5) & 0x01;
    m_IEType = byte0 & 0x1F;
    bytesRead++;

    // byte 1
    uint8_t byte1 = start.ReadU8();
    m_buffer.AddAtStart(1);
    m_buffer.Begin().WriteU8(byte1);
    bytesRead++;

    return bytesRead;
}

void
Dect2020MacMultiplexingHeaderTypeB::Print(std::ostream &os) const
{
    os << "MAC Extension: " << (int)m_macExtension
       << ", Length: " << (int)m_length
       << ", IE Type: " << (int)m_IEType;
}

} // namespace ns3