#include "dect2020ext-mac-multiplexing-header-type-b.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacMultiplexingHeaderTypeB");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacMultiplexingHeaderTypeB);

Dect2020ExtMacMultiplexingHeaderTypeB::Dect2020ExtMacMultiplexingHeaderTypeB()
{
}

Dect2020ExtMacMultiplexingHeaderTypeB::~Dect2020ExtMacMultiplexingHeaderTypeB()
{
}

void
Dect2020ExtMacMultiplexingHeaderTypeB::SetIEType(OneBytePayloadIEType IEType)
{
    m_IEType = IEType;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeB::GetIEType() const
{
    return m_IEType;
}

// Insert a payload into the SDU
void
Dect2020ExtMacMultiplexingHeaderTypeB::InsertSDU(const uint8_t payload)
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
Dect2020ExtMacMultiplexingHeaderTypeB::RemoveSDU()
{
    uint8_t payload = m_buffer.Begin().ReadU8();
    m_buffer.RemoveAtStart(1);

    // TODO: Figure out if byte tag list is needed
    //m_byteTagList.Adjust(-deserialized);

    return payload;
}

uint8_t
Dect2020ExtMacMultiplexingHeaderTypeB::GetSDU() const
{
    return m_buffer.Begin().PeekU8();
}

TypeId
Dect2020ExtMacMultiplexingHeaderTypeB::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacMultiplexingHeaderTypeB")
        .SetParent<Header>()
        .AddConstructor<Dect2020ExtMacMultiplexingHeaderTypeB>();
    return tid;
}

TypeId
Dect2020ExtMacMultiplexingHeaderTypeB::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeB::GetSerializedSize() const
{
    return 2; // 1 byte for header, 1 byte for SDU
}

void
Dect2020ExtMacMultiplexingHeaderTypeB::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = (m_macExtension & 0x03) << 6;
    byte0 |= (m_length & 0x01) << 5;
    byte0 |= (m_type & 0x1F);
    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_buffer.Begin().ReadU8());
}

uint32_t
Dect2020ExtMacMultiplexingHeaderTypeB::Deserialize(Buffer::Iterator start)
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
Dect2020ExtMacMultiplexingHeaderTypeB::Print(std::ostream &os) const
{
    os << "[MAC MULTIPLEXING HEADER TYPE B]\n"
       << "MAC Extension: " << std::hex << (int)m_macExtension << "\n"
       << "Length: " << std::hex << (int)m_length << "\n"
       << "SDU: " << std::hex << GetSDU() << "\n"
       << "IE Type: " << std::hex << (int)m_IEType << "\n" << "\n";
}

} // namespace ns3