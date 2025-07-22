#include "dect2020-net-device-helper.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020NetDeviceHelper");
NS_OBJECT_ENSURE_REGISTERED(Dect2020NetDeviceHelper);

Dect2020NetDeviceHelper::Dect2020NetDeviceHelper()
{
    m_device = CreateObject<Dect2020NetDevice>();
}

Dect2020NetDeviceHelper::~Dect2020NetDeviceHelper()
{
    m_device = nullptr;
}

TypeId
Dect2020NetDeviceHelper::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020NetDeviceHelper")
        .SetParent<Object>()
        .AddConstructor<Dect2020NetDeviceHelper>();

    return tid;
}

TypeId
Dect2020NetDeviceHelper::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

} // namespace ns3
