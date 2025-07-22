// Dect2020Channel - wrapper around ns3::SimpleChannel

#ifndef DECT2020_CHANNEL_H
#define DECT2020_CHANNEL_H

#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/pointer.h"

namespace ns3
{

class SimpleChannel;
class SimpleNetDevice;

class Dect2020Channel : public Object
{
    public:
        Dect2020Channel();
        virtual ~Dect2020Channel();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        void Add(Ptr<SimpleNetDevice> device);

        // Setters and Getters

        Ptr<SimpleChannel> GetWrappedChannelPointer(void) const;

    private:
        Ptr<SimpleChannel> m_simpleChannel;
        Time m_delay;
};

} // namespace ns3

#endif // DECT2020_CHANNEL_H