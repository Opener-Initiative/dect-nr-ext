#include "cluster-beacon-message.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("ClusterBeaconMessage");

NS_OBJECT_ENSURE_REGISTERED(ClusterBeaconMessage);

ClusterBeaconMessage::ClusterBeaconMessage()
{
    NS_LOG_FUNCTION(this);
}

ClusterBeaconMessage::~ClusterBeaconMessage()
{
    NS_LOG_FUNCTION(this);
}

TypeId
ClusterBeaconMessage::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::ClusterBeaconMessage")
                            .SetParent<Header>()
                            .AddConstructor<ClusterBeaconMessage>();
    return tid;
}

TypeId
ClusterBeaconMessage::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
ClusterBeaconMessage::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
ClusterBeaconMessage::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
ClusterBeaconMessage::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
ClusterBeaconMessage::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "ClusterBeaconMessage";
}

} // namespace ns3 