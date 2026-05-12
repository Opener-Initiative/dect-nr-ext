#include "dect2020ext-association-control-ie.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

using DlDataReception = Dect2020ExtAssociationControlIe::DlDataReception;
using ULPeriod = Dect2020ExtAssociationControlIe::UlPeriod;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtAssociationControlIe");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationControlIe);

Dect2020ExtAssociationControlIe::Dect2020ExtAssociationControlIe()
{
}

Dect2020ExtAssociationControlIe::~Dect2020ExtAssociationControlIe()
{
}

TypeId
Dect2020ExtAssociationControlIe::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationControlIe")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtAssociationControlIe>();
    return tid;
}

TypeId
Dect2020ExtAssociationControlIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtAssociationControlIe::Print(std::ostream& os) const
{
}

uint32_t
Dect2020ExtAssociationControlIe::GetSerializedSize() const
{
    return 1;
}

void
Dect2020ExtAssociationControlIe::Serialize(Buffer::Iterator start) const
{
    uint8_t byte0 = 0; 
    byte0 |= m_CB_M << 7;
    byte0 |= m_dlDataReception << 4;
    byte0 |= m_UlPeriod;

    start.WriteU8(byte0);
}

uint32_t
Dect2020ExtAssociationControlIe::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    uint8_t byte0 = start.ReadU8();
    m_CB_M = (byte0 >> 7);
    m_dlDataReception = (byte0 >> 4) & 7;
    m_UlPeriod = byte0 & 15;

    return bytesRead;
}

void
Dect2020ExtAssociationControlIe::SetCbM(uint8_t cbM)
{
    if (cbM <= 1)
    {
        m_CB_M = cbM;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid CB_M. Must be 0 or 1", cbM);
    }
}

uint8_t
Dect2020ExtAssociationControlIe::GetCbM() const
{
    return m_CB_M;
}

void
Dect2020ExtAssociationControlIe::SetDlDataReception(DlDataReception dlDataReception)
{
    m_dlDataReception = dlDataReception;
}

uint8_t
Dect2020ExtAssociationControlIe::GetDlDataReception() const
{
    return m_dlDataReception;
}

void
Dect2020ExtAssociationControlIe::SetUlPeriod(ULPeriod ulPeriod)
{
    m_UlPeriod = ulPeriod;
}

uint8_t
Dect2020ExtAssociationControlIe::GetUlPeriod() const
{
    return m_UlPeriod;
}

Time 
Dect2020ExtAssociationControlIe::DecodeDlDataReception() const
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
            NS_FATAL_ERROR("m_dlDataReception was not initialized correctly");
            return MilliSeconds(0);
    }
}

Time
Dect2020ExtAssociationControlIe::DecodeUlPeriod() const
{
    switch (m_UlPeriod)
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
            NS_FATAL_ERROR("m_UlPeriod was not initialized correctly");
            return Seconds(10);
    }
}

} // namespace ns3