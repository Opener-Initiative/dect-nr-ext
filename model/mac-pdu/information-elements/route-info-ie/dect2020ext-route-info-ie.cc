#include "dect2020-route-info-ie.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020RouteInfoIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020RouteInfoIE);

Dect2020RouteInfoIE::Dect2020RouteInfoIE()
{
}

Dect2020RouteInfoIE::~Dect2020RouteInfoIE()
{
}

TypeId
Dect2020RouteInfoIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020RouteInfoIE")
                        .SetParent<Header>()
                        .SetGroupName("Dect2020")
                        .AddConstructor<Dect2020RouteInfoIE>();
    return tid;
}

TypeId
Dect2020RouteInfoIE::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020RouteInfoIE::SetSinkAddress(uint32_t sinkAddress)
{
    m_sinkAddress = sinkAddress;
}

uint32_t
Dect2020RouteInfoIE::GetSinkAddress() const
{
    return m_sinkAddress;
}

void
Dect2020RouteInfoIE::SetRouteCost(uint8_t routeCost)
{
    m_routeCost = routeCost;
}

uint8_t
Dect2020RouteInfoIE::GetRouteCost() const
{
    return m_routeCost;
}

void
Dect2020RouteInfoIE::SetApplicationSequenceNumber(uint8_t applicationSequenceNumber)
{
    m_applicationSequenceNumber = applicationSequenceNumber;
}

uint8_t
Dect2020RouteInfoIE::GetApplicationSequenceNumber() const
{
    return m_applicationSequenceNumber;
}

void
Dect2020RouteInfoIE::Print(std::ostream& os) const
{
    os << "Route Info IE:" << std::endl << "SinkAddress = 0x" << std::hex << m_sinkAddress << std::endl
        << "RouteCost = " << m_routeCost << std::endl
        << "ApplicationSequenceNumber = " << m_applicationSequenceNumber << std::endl;
}

uint32_t
Dect2020RouteInfoIE::GetSerializedSize(void) const
{
    return 6;
}

void
Dect2020RouteInfoIE::Serialize(Buffer::Iterator start) const
{
    start.WriteHtonU32(m_sinkAddress);
    start.WriteU8(m_routeCost);
    start.WriteU8(m_applicationSequenceNumber);
}

uint32_t
Dect2020RouteInfoIE::Deserialize(Buffer::Iterator start)
{
    m_sinkAddress = start.ReadNtohU32();
    m_routeCost = start.ReadU8();
    m_applicationSequenceNumber = start.ReadU8();

    return GetSerializedSize();
}

} // namespace ns3
