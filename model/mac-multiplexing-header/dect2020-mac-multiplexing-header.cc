#include "dect2020-mac-multiplexing-header.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeader");

NS_OBJECT_ENSURE_REGISTERED(Dect2020MacMultiplexingHeader);

Dect2020MacMultiplexingHeader::Dect2020MacMultiplexingHeader()
{
}

Dect2020MacMultiplexingHeader::~Dect2020MacMultiplexingHeader()
{
}

TypeId
Dect2020MacMultiplexingHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020MacMultiplexingHeader")
                            .SetParent<Header>()
                            .SetGroupName("DectMacPdu")
                            .AddConstructor<Dect2020MacMultiplexingHeader>();
    return tid;
}

TypeId
Dect2020MacMultiplexingHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020MacMultiplexingHeader::GetSerializedSize() const
{
    return m_type;
}

void
Dect2020MacMultiplexingHeader::Serialize(Buffer::Iterator start) const
{
    if(m_type == Type::A)
    {
        // byte 0
        uint8_t byte0 = (m_macExtension << 6);
        byte0 |= (m_length << 5);
        byte0 |= (m_IEType & 0x1F);
        start.WriteU8(byte0);
    }
}

uint32_t
Dect2020MacMultiplexingHeader::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;
    if(m_type == Type::A)
    {
        // byte 0
        uint8_t byte0 = start.ReadU8();
        m_macExtension = (byte0 >> 6) & 0x03;
        m_length = (byte0 >> 5) & 0x01;
        m_IEType = byte0 & 0x1F;
        bytesRead = 1;
        return bytesRead;
    }
    else
    {
        return 0;
    }
}

void
Dect2020MacMultiplexingHeader::Print(std::ostream &os) const
{
    os << "[Dect2020MacMultiplexingHeader]" << "\n"
       << "Type = " << m_type << "\n"
       << "MacExtension = " << m_macExtension << "\n"
       << "Length = " << m_length << "\n"
       << "IEType = " << m_IEType;
}

// Type setter and getter
void
Dect2020MacMultiplexingHeader::SetType(Type type)
{
    m_type = type;
}

Dect2020MacMultiplexingHeader::Type
Dect2020MacMultiplexingHeader::GetType() const
{
    return m_type;
}

// MAC Extension setter and getter
void
Dect2020MacMultiplexingHeader::SetMacExtension(uint8_t macExtension)
{
    m_macExtension = macExtension & 0x03; // Ensure only 2 bits are used
}

uint8_t
Dect2020MacMultiplexingHeader::GetMacExtension() const
{
    return m_macExtension;
}

// Length setter and getter
void
Dect2020MacMultiplexingHeader::SetLength(uint8_t length)
{
    m_length = length & 0x01; // Ensure only 1 bit is used
}

uint8_t
Dect2020MacMultiplexingHeader::GetLength() const
{
    return m_length;
}

// IE Type setter and getter
void
Dect2020MacMultiplexingHeader::SetIEType(uint8_t ieType)
{
    m_IEType = ieType & 0x1F; // Ensure only 5 bits are used
}

uint8_t
Dect2020MacMultiplexingHeader::GetIEType() const
{
    return m_IEType;
}

} // namespace ns3 