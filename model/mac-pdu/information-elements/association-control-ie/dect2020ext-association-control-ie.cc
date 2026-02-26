#include "dect2020ext-association-control-ie.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtAssociationControlIE");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationControlIE);

Dect2020ExtAssociationControlIE::Dect2020ExtAssociationControlIE()
{
}

Dect2020ExtAssociationControlIE::~Dect2020ExtAssociationControlIE()
{
}

TypeId
Dect2020ExtAssociationControlIE::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationControlIE")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtAssociationControlIE>();
    return tid;
}

TypeId
Dect2020ExtAssociationControlIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtAssociationControlIE::Print(std::ostream& os) const
{
}

uint32_t
Dect2020ExtAssociationControlIE::GetSerializedSize() const
{
    return 1;
}

void
Dect2020ExtAssociationControlIE::Serialize(Buffer::Iterator start) const
{
    uint8_t byte0 = 0; 
    byte0 |= m_CB_M << 7;
    byte0 |= m_dlDataReception << 4;
    byte0 |= m_ULPeriod;

    start.WriteU8(byte0);
}

uint32_t
Dect2020ExtAssociationControlIE::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    uint8_t byte0 = start.ReadU8();
    m_CB_M = (byte0 >> 7);
    m_dlDataReception = (byte0 >> 4) & 7;
    m_ULPeriod = byte0 & 15;

    return bytesRead;
}

void
Dect2020ExtAssociationControlIE::SetCbM(uint8_t cbM)
{
    if (cbM <= 1)
    {
        m_CB_M = cbM;
    }
    else
    {
        NS_LOG_WARN("Invalid CB_M. Must be 0 or 1");
    }
}

uint8_t
Dect2020ExtAssociationControlIE::GetCbM() const
{
    return m_CB_M;
}

void
Dect2020ExtAssociationControlIE::SetDlDataReception(uint8_t dlDataReception)
{
    if (dlDataReception <= 5)
    {
        m_dlDataReception = dlDataReception;
    }
    else
    {
        NS_LOG_WARN("Invalid DLdataReception. Must be between 0 and 5");
    }
}

uint8_t
Dect2020ExtAssociationControlIE::GetDlDataReception() const
{
    return m_dlDataReception;
}

void
Dect2020ExtAssociationControlIE::SetUlPeriod(uint8_t ulPeriod)
{
    if (ulPeriod <= 11)
    {
        m_ULPeriod = ulPeriod;
    }
    else
    {
        NS_LOG_WARN("Invalid ULPeriod. Must be between 0 and 11");
    }
}

uint8_t
Dect2020ExtAssociationControlIE::GetUlPeriod() const
{
    return m_ULPeriod;
}

} // namespace ns3