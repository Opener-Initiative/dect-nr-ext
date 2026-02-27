#include "dect2020ext-association-control-ie.h"

namespace ns3
{

using DlDataReception = Dect2020ExtAssociationControlIE::DlDataReception;
using ULPeriod = Dect2020ExtAssociationControlIE::ULPeriod;

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
Dect2020ExtAssociationControlIE::SetDlDataReception(DlDataReception dlDataReception)
{
    m_dlDataReception = dlDataReception;
}

uint8_t
Dect2020ExtAssociationControlIE::GetDlDataReception() const
{
    return m_dlDataReception;
}

void
Dect2020ExtAssociationControlIE::SetUlPeriod(ULPeriod ulPeriod)
{
    m_ULPeriod = ulPeriod;
}

uint8_t
Dect2020ExtAssociationControlIE::GetUlPeriod() const
{
    return m_ULPeriod;
}

Time 
Dect2020ExtAssociationControlIE::DecodeDlDataReception() const
{
    switch (m_dlDataReception)
    {
        case MS_0:  return MilliSeconds(0);
        case MS_5:  return MilliSeconds(5);
        case MS_10: return MilliSeconds(10);
        case MS_20: return MilliSeconds(20);
        case MS_40: return MilliSeconds(40);
        case MS_80: return MilliSeconds(80);

        default:
            NS_LOG_WARN("m_dlDataReception was not initialized correctly");
            return MilliSeconds(0);
    }
}

Time
Dect2020ExtAssociationControlIE::DecodeULPeriod() const
{
    switch (m_ULPeriod)
    {
        case S_10:  return Seconds(10);
        case S_20:  return Seconds(20);
        case S_48:  return Seconds(48);
        case S_90:  return Seconds(90);
        case MIN_5: return Minutes(5);
        case MIN_10: return Minutes(10);
        case MIN_30: return Minutes(30);
        case H_1:   return Hours(1);
        case H_6:   return Hours(6);
        case H_12:  return Hours(12);
        case H_24:  return Hours(24);
        case H_48:  return Hours(48);
        default:
            NS_LOG_WARN("m_ULPeriod was not initialized correctly");
            return Seconds(10);
    }
}

} // namespace ns3