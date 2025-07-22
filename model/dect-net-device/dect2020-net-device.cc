#include "dect2020-net-device.h"

namespace ns3
{

Dect2020NetDevice::Dect2020NetDevice()
{
    m_device = CreateObject<SimpleNetDevice>();
}

Dect2020NetDevice::~Dect2020NetDevice()
{
}

} // namespace ns3
