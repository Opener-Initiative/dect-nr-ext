#include "association-request-message.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("AssociationRequestMessage");

NS_OBJECT_ENSURE_REGISTERED(AssociationRequestMessage);

AssociationRequestMessage::AssociationRequestMessage()
{
    NS_LOG_FUNCTION(this);
}

AssociationRequestMessage::~AssociationRequestMessage()
{
    NS_LOG_FUNCTION(this);
}

TypeId
AssociationRequestMessage::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::AssociationRequestMessage")
                            .SetParent<Header>()
                            .AddConstructor<AssociationRequestMessage>();
    return tid;
}

TypeId
AssociationRequestMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
AssociationRequestMessage::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
AssociationRequestMessage::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
AssociationRequestMessage::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
AssociationRequestMessage::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "AssociationRequestMessage";
}

} // namespace ns3 