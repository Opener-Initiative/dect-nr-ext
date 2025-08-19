#include "dect2020ext-route-info-ie.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRouteInfoIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtRouteInfoIE);

Dect2020ExtRouteInfoIE::Dect2020ExtRouteInfoIE()
{
}

Dect2020ExtRouteInfoIE::~Dect2020ExtRouteInfoIE()
{
}

TypeId
Dect2020ExtRouteInfoIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtRouteInfoIE")
                        .SetParent<Header>()
                        .SetGroupName("Dect2020Ext")
                        .AddConstructor<Dect2020ExtRouteInfoIE>();
    return tid;
}

TypeId
Dect2020ExtRouteInfoIE::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020ExtRouteInfoIE::SetSinkAddress(uint32_t sinkAddress)
{
    m_sinkAddress = sinkAddress;
}

uint32_t
Dect2020ExtRouteInfoIE::GetSinkAddress() const
{
    return m_sinkAddress;
}

void
Dect2020ExtRouteInfoIE::SetRouteCost(uint8_t routeCost)
{
    m_routeCost = routeCost;
}

uint8_t
Dect2020ExtRouteInfoIE::GetRouteCost() const
{
    return m_routeCost;
}

void
Dect2020ExtRouteInfoIE::SetApplicationSequenceNumber(uint8_t applicationSequenceNumber)
{
    m_applicationSequenceNumber = applicationSequenceNumber;
}

uint8_t
Dect2020ExtRouteInfoIE::GetApplicationSequenceNumber() const
{
    return m_applicationSequenceNumber;
}

void
Dect2020ExtRouteInfoIE::Print(std::ostream& os) const
{
    os << "Route Info IE:" << std::endl << "SinkAddress = 0x" << std::hex << m_sinkAddress << std::endl
        << "RouteCost = " << m_routeCost << std::endl
        << "ApplicationSequenceNumber = " << m_applicationSequenceNumber << std::endl;
}

uint32_t
Dect2020ExtRouteInfoIE::GetSerializedSize(void) const
{
    return 6;
}

void
Dect2020ExtRouteInfoIE::Serialize(Buffer::Iterator start) const
{
    start.WriteHtonU32(m_sinkAddress);
    start.WriteU8(m_routeCost);
    start.WriteU8(m_applicationSequenceNumber);
}

uint32_t
Dect2020ExtRouteInfoIE::Deserialize(Buffer::Iterator start)
{
    m_sinkAddress = start.ReadNtohU32();
    m_routeCost = start.ReadU8();
    m_applicationSequenceNumber = start.ReadU8();

    return GetSerializedSize();
}

} // namespace ns3
