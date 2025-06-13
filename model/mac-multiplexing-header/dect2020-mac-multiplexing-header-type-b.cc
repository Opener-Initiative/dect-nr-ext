#include "dect2020-mac-multiplexing-header-type-b.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeB");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeB);

// Insert an IE into the SDU
void
Dect2020MacMultiplexingHeaderTypeB::InsertSDU(const Header& header)
{
    uint32_t size = header.GetSerializedSize();
    m_SDU.AddAtStart(size);
    //m_byteTagList.Adjust(size);
    //m_byteTagList.AddAtStart(size);
    header.Serialize(m_SDU.Begin());
}

// Remove an IE from the SDU
uint32_t
Dect2020MacMultiplexingHeaderTypeB::RemoveSDU(Header& header)
{
    uint32_t deserialized = header.Deserialize(m_SDU.Begin());
    m_SDU.RemoveAtStart(deserialized);
    //m_byteTagList.Adjust(-deserialized);
    return deserialized;
}

TypeId
Dect2020MacMultiplexingHeaderTypeB::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeaderTypeB");
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
    return 2;
}

void
Dect2020MacMultiplexingHeaderTypeB::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = m_macExtension << 6;
    byte0 |= m_length << 5;
    byte0 |= m_IEType;
    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_SDU.Begin().ReadU8());
}

uint32_t
Dect2020MacMultiplexingHeaderTypeB::Deserialize(Buffer::Iterator start)
{
    // Read first byte and extract fields
    uint8_t byte0 = start.ReadU8();
    m_macExtension = (byte0 >> 6) & 0x03;  // Extract 2 bits
    m_length = (byte0 >> 5) & 0x01;        // Extract 1 bit
    m_IEType = byte0 & 0x1F;               // Extract 5 bits

    // Read the 1-byte SDU
    m_SDU = Buffer(1);
    m_SDU.Begin().WriteU8(start.ReadU8());

    return 2;  // Return total bytes read (1 byte header + 1 byte SDU){
}

void
Dect2020MacMultiplexingHeaderTypeB::Print(std::ostream &os) const
{
    os << "Dect2020MacMultiplexingHeaderTypeB";
}

} // namespace ns3