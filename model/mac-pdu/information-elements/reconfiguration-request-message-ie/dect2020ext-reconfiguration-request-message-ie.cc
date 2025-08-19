#include "dect2020ext-reconfiguration-request-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtReconfigurationRequestMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtReconfigurationRequestMessageIE);

Dect2020ExtReconfigurationRequestMessageIE::Dect2020ExtReconfigurationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtReconfigurationRequestMessageIE::~Dect2020ExtReconfigurationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtReconfigurationRequestMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtReconfigurationRequestMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtReconfigurationRequestMessageIE>();
    return tid;
}

TypeId
Dect2020ExtReconfigurationRequestMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtReconfigurationRequestMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020ExtReconfigurationRequestMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020ExtReconfigurationRequestMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020ExtReconfigurationRequestMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ExtReconfigurationRequestMessageIE";
}

} // namespace ns3 