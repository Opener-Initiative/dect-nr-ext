#include "dect2020ext-reconfiguration-response-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtReconfigurationResponseMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtReconfigurationResponseMessageIE);

Dect2020ExtReconfigurationResponseMessageIE::Dect2020ExtReconfigurationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtReconfigurationResponseMessageIE::~Dect2020ExtReconfigurationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtReconfigurationResponseMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtReconfigurationResponseMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtReconfigurationResponseMessageIE>();
    return tid;
}

TypeId
Dect2020ExtReconfigurationResponseMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtReconfigurationResponseMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020ExtReconfigurationResponseMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020ExtReconfigurationResponseMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020ExtReconfigurationResponseMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ExtReconfigurationResponseMessageIE";
}

} // namespace ns3 