#include "ns3/dect2020-mac-pdu.h"

namespace ns3
{

Dect2020MacPdu::Dect2020MacPdu(MacHeaderType type, MacSecurity security)
{
    m_macHeaderType = type;
    m_macSecurity = security;
}

Dect2020MacPdu::~Dect2020MacPdu(){}

MacHeaderType
Dect2020MacPdu::GetMacHeaderType() const
{
    return m_macHeaderType;
}

void
Dect2020MacPdu::SetMacHeaderType(MacHeaderType type)
{
    m_macHeaderType = type;
}

MacSecurity
Dect2020MacPdu::GetMacSecurity() const
{
    return m_macSecurity;
}

void
Dect2020MacPdu::SetMacSecurity(MacSecurity security)
{
    m_macSecurity = security;
}

} // namespace ns3