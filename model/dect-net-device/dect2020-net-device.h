#ifndef DECT2020_NET_DEVICE_H
#define DECT2020_NET_DEVICE_H

#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/pointer.h"
#include "ns3/dect2020-channel.h"

namespace ns3
{

class SimpleNetDevice;
class SimpleChannel;
class Dect2020Channel;

class Dect2020NetDevice : public Object
{
    public:
        Dect2020NetDevice();
        ~Dect2020NetDevice();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        void SetChannel(Ptr<Dect2020Channel> channel);

        // Setters and Getters

        Ptr<SimpleNetDevice> GetDevice(void) const;

    private:
        Ptr<SimpleNetDevice> m_simpleNetDevice;
        Ptr<Dect2020Channel> m_channel;
        bool m_linkUp;
};

} // namespace ns3

#endif // DECT2020_NET_DEVICE_H