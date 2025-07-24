#include "dect2020-mac.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020Mac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020Mac);

Dect2020Mac::Dect2020Mac()
{
}

Dect2020Mac::~Dect2020Mac()
{
}

TypeId
Dect2020Mac::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020Mac")
        .SetParent<Object>()
        .AddConstructor<Dect2020Mac>();

    return tid;
}

TypeId
Dect2020Mac::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020Mac::ProcessPacket(Ptr<Packet> packet)
{
    NS_LOG_INFO("HELLO FROM MAC =)");
}

void
Dect2020Mac::SetNetDevice(Ptr<Dect2020NetDevice> netDevice)
{
    m_netDevice = netDevice;
}

} // namespace ns3