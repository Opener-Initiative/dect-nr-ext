#include "reconfiguration-response-message.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("ReconfigurationResponseMessage");

NS_OBJECT_ENSURE_REGISTERED(ReconfigurationResponseMessage);

ReconfigurationResponseMessage::ReconfigurationResponseMessage()
{
    NS_LOG_FUNCTION(this);
}

ReconfigurationResponseMessage::~ReconfigurationResponseMessage()
{
    NS_LOG_FUNCTION(this);
}

TypeId
ReconfigurationResponseMessage::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::ReconfigurationResponseMessage")
                            .SetParent<Header>()
                            .AddConstructor<ReconfigurationResponseMessage>();
    return tid;
}

TypeId
ReconfigurationResponseMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
ReconfigurationResponseMessage::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
ReconfigurationResponseMessage::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
ReconfigurationResponseMessage::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
ReconfigurationResponseMessage::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "ReconfigurationResponseMessage";
}

} // namespace ns3 