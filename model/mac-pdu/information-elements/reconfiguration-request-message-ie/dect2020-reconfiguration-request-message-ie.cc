#include "dect2020-reconfiguration-request-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ReconfigurationRequestMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ReconfigurationRequestMessageIE);

Dect2020ReconfigurationRequestMessageIE::Dect2020ReconfigurationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ReconfigurationRequestMessageIE::~Dect2020ReconfigurationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ReconfigurationRequestMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ReconfigurationRequestMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ReconfigurationRequestMessageIE>();
    return tid;
}

TypeId
Dect2020ReconfigurationRequestMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ReconfigurationRequestMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020ReconfigurationRequestMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020ReconfigurationRequestMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020ReconfigurationRequestMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ReconfigurationRequestMessageIE";
}

} // namespace ns3 