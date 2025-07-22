#ifndef DECT2020_NET_DEVICE_HELPER_H
#define DECT2020_NET_DEVICE_HELPER_H

#include "ns3/dect2020-net-device.h"

namespace ns3
{

class Dect2020NetDevice;

class Dect2020NetDeviceHelper
{
    public:
        Dect2020NetDeviceHelper();
        ~Dect2020NetDeviceHelper();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

    private:
        Ptr<Dect2020NetDevice> m_device;
};

} // namespace ns3

#endif // DECT2020_NET_DEVICE_HELPER_H