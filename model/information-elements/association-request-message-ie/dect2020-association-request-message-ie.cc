#include "dect2020-association-request-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020AssociationRequestMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020AssociationRequestMessageIE);

Dect2020AssociationRequestMessageIE::Dect2020AssociationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020AssociationRequestMessageIE::~Dect2020AssociationRequestMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020AssociationRequestMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020AssociationRequestMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020AssociationRequestMessageIE>();
    return tid;
}

TypeId
Dect2020AssociationRequestMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020AssociationRequestMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020AssociationRequestMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020AssociationRequestMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020AssociationRequestMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020AssociationRequestMessageIE";
}

} // namespace ns3 