#include "dect2020-mesh-header.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("Dect2020MeshHeader");

    NS_OBJECT_ENSURE_REGISTERED(Dect2020MeshHeader);

    Dect2020MeshHeader::Dect2020MeshHeader()
        : m_sinkAddress(0),
          m_routeCost(0),
          m_applicationSequenceNumber(0)
    {
    }

    Dect2020MeshHeader::~Dect2020MeshHeader()
    {
    }

    TypeId
    Dect2020MeshHeader::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Dect2020MeshHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020")
                            .AddConstructor<Dect2020MeshHeader>();
        return tid;
    }

    TypeId
    Dect2020MeshHeader::GetInstanceTypeId(void) const
    {
        return GetTypeId();
    }

    void
    Dect2020MeshHeader::SetSinkAddress(uint32_t sinkAddress)
    {
        m_sinkAddress = sinkAddress;
    }

    uint32_t
    Dect2020MeshHeader::GetSinkAddress() const
    {
        return m_sinkAddress;
    }

    void
    Dect2020MeshHeader::SetRouteCost(uint32_t routeCost)
    {
        m_routeCost = routeCost;
    }

    uint32_t
    Dect2020MeshHeader::GetRouteCost() const
    {
        return m_routeCost;
    }

    void
    Dect2020MeshHeader::SetApplicationSequenceNumber(uint32_t applicationSequenceNumber)
    {
        m_applicationSequenceNumber = applicationSequenceNumber;
    }

    uint32_t
    Dect2020MeshHeader::GetApplicationSequenceNumber() const
    {
        return m_applicationSequenceNumber;
    }

    void
    Dect2020MeshHeader::Print(std::ostream& os) const
    {
        os << "Mesh Header:" << std::endl << "SinkAddress = 0x" << std::hex << m_sinkAddress << std::endl
           << "RouteCost = " << m_routeCost << std::endl
           << "ApplicationSequenceNumber = " << m_applicationSequenceNumber << std::endl;
    }

    uint32_t
    Dect2020MeshHeader::GetSerializedSize(void) const
    {
        return 0;
    }

    void
    Dect2020MeshHeader::Serialize(Buffer::Iterator start) const
    {
    }

    uint32_t
    Dect2020MeshHeader::Deserialize(Buffer::Iterator start)
    {
        return 0;
    }

} // namespace ns3
