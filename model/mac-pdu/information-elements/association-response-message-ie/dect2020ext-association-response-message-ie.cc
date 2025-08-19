#include "dect2020ext-association-response-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtAssociationResponseMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationResponseMessageIE);

Dect2020ExtAssociationResponseMessageIE::Dect2020ExtAssociationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtAssociationResponseMessageIE::~Dect2020ExtAssociationResponseMessageIE()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtAssociationResponseMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationResponseMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtAssociationResponseMessageIE>();
    return tid;
}

TypeId
Dect2020ExtAssociationResponseMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtAssociationResponseMessageIE::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    return 0; // Size in bytes
}

void
Dect2020ExtAssociationResponseMessageIE::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
}

uint32_t
Dect2020ExtAssociationResponseMessageIE::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    return 0; // Number of bytes consumed
}

void
Dect2020ExtAssociationResponseMessageIE::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ExtAssociationResponseMessageIE";
}

} // namespace ns3 