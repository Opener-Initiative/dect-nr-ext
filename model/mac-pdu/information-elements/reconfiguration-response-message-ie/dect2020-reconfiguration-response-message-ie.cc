#include "dect2020-reconfiguration-response-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ReconfigurationResponseMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ReconfigurationResponseMessageIE);

Dect2020ReconfigurationResponseMessageIE::Dect2020ReconfigurationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ReconfigurationResponseMessageIE::~Dect2020ReconfigurationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ReconfigurationResponseMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ReconfigurationResponseMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ReconfigurationResponseMessageIE>();
    return tid;
}

TypeId
Dect2020ReconfigurationResponseMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ReconfigurationResponseMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020ReconfigurationResponseMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020ReconfigurationResponseMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020ReconfigurationResponseMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ReconfigurationResponseMessageIE";
}

} // namespace ns3 