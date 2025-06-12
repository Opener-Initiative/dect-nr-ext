#include "dect2020-association-response-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020AssociationResponseMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020AssociationResponseMessageIE);

Dect2020AssociationResponseMessageIE::Dect2020AssociationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020AssociationResponseMessageIE::~Dect2020AssociationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020AssociationResponseMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020AssociationResponseMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020AssociationResponseMessageIE>();
    return tid;
}

TypeId
Dect2020AssociationResponseMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020AssociationResponseMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020AssociationResponseMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020AssociationResponseMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020AssociationResponseMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020AssociationResponseMessageIE";
}

} // namespace ns3 