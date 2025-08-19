#include "dect2020-keep-alive-ie.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Dect2020KeepAliveIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020KeepAliveIE);

Dect2020KeepAliveIE::Dect2020KeepAliveIE ()
{
  NS_LOG_FUNCTION (this);
}

Dect2020KeepAliveIE::~Dect2020KeepAliveIE ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
Dect2020KeepAliveIE::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Dect2020KeepAliveIE")
    .SetParent<Header> ()
    .AddConstructor<Dect2020KeepAliveIE> ()
  ;
  return tid;
}

TypeId
Dect2020KeepAliveIE::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
Dect2020KeepAliveIE::GetSerializedSize (void) const
{
  // Since this is a keep-alive IE, it might not need to carry any data
  // Return 0 for now, adjust if data needs to be added later
  return 0;
}

void
Dect2020KeepAliveIE::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  // No data to serialize for now
}

uint32_t
Dect2020KeepAliveIE::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  // No data to deserialize for now
  return 0;
}

void
Dect2020KeepAliveIE::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "Dect2020KeepAliveIE";
}

} // namespace ns3 