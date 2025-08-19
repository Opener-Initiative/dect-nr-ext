#include "dect2020ext-mac-multiplexing-header.h"

namespace ns3
{

Dect2020ExtMacMultiplexingHeader::Dect2020ExtMacMultiplexingHeader()
{
}

Dect2020ExtMacMultiplexingHeader::~Dect2020ExtMacMultiplexingHeader()
{
}

void
Dect2020ExtMacMultiplexingHeader::SetType(Type type)
{
    m_type = type;
}

Dect2020ExtMacMultiplexingHeader::Type
Dect2020ExtMacMultiplexingHeader::GetType() const
{
    return m_type;
}

void
Dect2020ExtMacMultiplexingHeader::SetMacExtension(uint8_t macExtension)
{
    m_macExtension = macExtension & 0x03; // Ensure only 2 bits are used
}

uint8_t
Dect2020ExtMacMultiplexingHeader::GetMacExtension() const
{
    return m_macExtension;
}

void
Dect2020ExtMacMultiplexingHeader::SetLength(uint8_t length)
{
    m_length = length & 0x01; // Ensure only 1 bit is used
}

uint8_t
Dect2020ExtMacMultiplexingHeader::GetLength() const
{
    return m_length;
}

} // namespace ns3 