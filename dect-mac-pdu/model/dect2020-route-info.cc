#include "dect2020-route-info.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("Dect2020RouteInfo");

    NS_OBJECT_ENSURE_REGISTERED(Dect2020RouteInfo);

    Dect2020RouteInfo::Dect2020RouteInfo()
        : m_sinkAddress(0),
          m_routeCost(0),
          m_applicationSequenceNumber(0)
    {
    }

    Dect2020RouteInfo::~Dect2020RouteInfo()
    {
    }

    TypeId
    Dect2020RouteInfo::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Dect2020RouteInfo")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020")
                            .AddConstructor<Dect2020RouteInfo>();
        return tid;
    }

    TypeId
    Dect2020RouteInfo::GetInstanceTypeId(void) const
    {
        return GetTypeId();
    }

    void
    Dect2020RouteInfo::SetSinkAddress(uint32_t sinkAddress)
    {
        m_sinkAddress = sinkAddress;
    }

    uint32_t
    Dect2020RouteInfo::GetSinkAddress() const
    {
        return m_sinkAddress;
    }

    void
    Dect2020RouteInfo::SetRouteCost(uint32_t routeCost)
    {
        m_routeCost = routeCost;
    }

    uint32_t
    Dect2020RouteInfo::GetRouteCost() const
    {
        return m_routeCost;
    }

    void
    Dect2020RouteInfo::SetApplicationSequenceNumber(uint32_t applicationSequenceNumber)
    {
        m_applicationSequenceNumber = applicationSequenceNumber;
    }

    uint32_t
    Dect2020RouteInfo::GetApplicationSequenceNumber() const
    {
        return m_applicationSequenceNumber;
    }

    void
    Dect2020RouteInfo::Print(std::ostream& os) const
    {
        os << "Mesh Header:" << std::endl << "SinkAddress = 0x" << std::hex << m_sinkAddress << std::endl
           << "RouteCost = " << m_routeCost << std::endl
           << "ApplicationSequenceNumber = " << m_applicationSequenceNumber << std::endl;
    }

    uint32_t
    Dect2020RouteInfo::GetSerializedSize(void) const
    {
        return 6;
    }

    void
    Dect2020RouteInfo::Serialize(Buffer::Iterator start) const
    {
        start.WriteHtonU32(m_sinkAddress);
        start.WriteU8(m_routeCost);
        start.WriteU8(m_applicationSequenceNumber);
    }

    uint32_t
    Dect2020RouteInfo::Deserialize(Buffer::Iterator start)
    {
        m_sinkAddress = start.ReadNtohU32();
        m_routeCost = start.ReadU8();
        m_applicationSequenceNumber = start.ReadU8();

        return GetSerializedSize();
    }

} // namespace ns3
