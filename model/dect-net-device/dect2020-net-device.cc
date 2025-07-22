#include "dect2020-net-device.h"

namespace ns3
{

Dect2020NetDevice::Dect2020NetDevice()
{
    m_simpleNetDevice = CreateObject<SimpleNetDevice>();
}

Dect2020NetDevice::~Dect2020NetDevice()
{
    m_simpleNetDevice = nullptr;
    m_channel = nullptr;
    m_linkUp = false;
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
    return m_simpleNetDevice;
}

void
Dect2020NetDevice::SetChannel(Ptr<Dect2020Channel> channel)
{
    //NS_LOG_FUNCTION(this << channel);
    m_channel = channel;
    m_channel->Add(m_simpleNetDevice);
    m_linkUp = true;
    //m_linkChangeCallbacks();
}

} // namespace ns3