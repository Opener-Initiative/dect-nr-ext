#include "dect2020-mac-multiplexing-header-type-b.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeB");
NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeaderTypeB);

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
}

uint32_t
Dect2020MacMultiplexingHeaderTypeB::Deserialize(Buffer::Iterator start)
{
}

void
Dect2020MacMultiplexingHeaderTypeB::Print(std::ostream &os) const
{
    os << "Dect2020MacMultiplexingHeaderTypeB";
}

} // namespace ns3