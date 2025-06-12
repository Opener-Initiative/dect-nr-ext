#include "dect2020-mac-multiplexing-header.h"

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

// Dect2020MacMultiplexingHeader Setters and Getters

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