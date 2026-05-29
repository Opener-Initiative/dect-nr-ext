#include "dect2020ext-keep-alive-ie.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020ExtKeepAliveIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtKeepAliveIE);

Dect2020ExtKeepAliveIE::Dect2020ExtKeepAliveIE ()
{
  NS_LOG_FUNCTION (this);
}

Dect2020ExtKeepAliveIE::~Dect2020ExtKeepAliveIE ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
Dect2020ExtKeepAliveIE::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Dect2020ExtKeepAliveIE")
    .SetParent<Header> ()
    .AddConstructor<Dect2020ExtKeepAliveIE> ()
  ;
  return tid;
}

TypeId
Dect2020ExtKeepAliveIE::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
Dect2020ExtKeepAliveIE::GetSerializedSize () const
{
  // Since this is a keep-alive IE, it might not need to carry any data
  // Return 0 for now, adjust if data needs to be added later
  return 0;
}

void
Dect2020ExtKeepAliveIE::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  // No data to serialize for now
}

uint32_t
Dect2020ExtKeepAliveIE::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  // No data to deserialize for now
  return 0;
}

void
Dect2020ExtKeepAliveIE::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "Dect2020ExtKeepAliveIE";
}

} // namespace ns3 