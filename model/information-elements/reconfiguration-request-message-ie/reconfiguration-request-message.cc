#include "reconfiguration-request-message.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("ReconfigurationRequestMessage");

NS_OBJECT_ENSURE_REGISTERED(ReconfigurationRequestMessage);

ReconfigurationRequestMessage::ReconfigurationRequestMessage()
{
    NS_LOG_FUNCTION(this);
}

ReconfigurationRequestMessage::~ReconfigurationRequestMessage()
{
    NS_LOG_FUNCTION(this);
}

TypeId
ReconfigurationRequestMessage::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::ReconfigurationRequestMessage")
                            .SetParent<Header>()
                            .AddConstructor<ReconfigurationRequestMessage>();
    return tid;
}

TypeId
ReconfigurationRequestMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
ReconfigurationRequestMessage::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
ReconfigurationRequestMessage::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
ReconfigurationRequestMessage::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
ReconfigurationRequestMessage::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "ReconfigurationRequestMessage";
}

} // namespace ns3 