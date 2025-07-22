// Dect2020Channel - wrapper around ns3::SimpleChannel

#ifndef DECT2020_CHANNEL_H
#define DECT2020_CHANNEL_H

#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"

namespace ns3
{

class SimpleChannel;
class SimpleNetDevice;
class Channel;

class Dect2020Channel : public Channel
{
    public:
        Dect2020Channel();
        virtual ~Dect2020Channel();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        // Virtual methods from Channel

        virtual std::size_t GetNDevices() const;
        virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

        // Wrapped methods from SimpleChannel

        void Add(Ptr<SimpleNetDevice> device);
        void SetAttribute(std::string name, const AttributeValue &value);

        // Dect2020Channel Setters and Getters

        Ptr<SimpleChannel> GetWrappedChannel(void) const;

    private:
        Ptr<SimpleChannel> m_simpleChannel;
        Time m_delay;
};

} // namespace ns3

#endif // DECT2020_CHANNEL_H