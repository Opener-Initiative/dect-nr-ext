#include "dect2020-mac-multiplexing-header.h"

namespace ns3
{

Dect2020MacMultiplexingHeader::Dect2020MacMultiplexingHeader()
{
}

Dect2020MacMultiplexingHeader::~Dect2020MacMultiplexingHeader()
{
}

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

} // namespace ns3 