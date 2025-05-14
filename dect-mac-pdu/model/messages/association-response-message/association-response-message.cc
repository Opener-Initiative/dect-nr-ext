#include "association-response-message.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("AssociationResponseMessage");

NS_OBJECT_ENSURE_REGISTERED(AssociationResponseMessage);

AssociationResponseMessage::AssociationResponseMessage()
{
    NS_LOG_FUNCTION(this);
}

AssociationResponseMessage::~AssociationResponseMessage()
{
    NS_LOG_FUNCTION(this);
}

TypeId
AssociationResponseMessage::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::AssociationResponseMessage")
                            .SetParent<Header>()
                            .AddConstructor<AssociationResponseMessage>();
    return tid;
}

TypeId
AssociationResponseMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
AssociationResponseMessage::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
AssociationResponseMessage::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
AssociationResponseMessage::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
AssociationResponseMessage::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "AssociationResponseMessage";
}

} // namespace ns3 