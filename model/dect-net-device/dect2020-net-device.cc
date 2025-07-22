#include "dect2020-net-device.h"

namespace ns3
{

Dect2020NetDevice::Dect2020NetDevice()
{
    m_device = CreateObject<SimpleNetDevice>();
}

Dect2020NetDevice::~Dect2020NetDevice()
{
    m_device = nullptr;
}

TypeId
Dect2020NetDevice::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020NetDevice")
        .SetParent<Object>()
        .AddConstructor<Dect2020NetDevice>();

    return tid;
}

TypeId
Dect2020NetDevice::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

Ptr<SimpleNetDevice>
Dect2020NetDevice::GetDevice(void) const
{
    return m_device;
}

} // namespace ns3
