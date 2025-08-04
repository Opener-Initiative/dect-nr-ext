// Dect2020Channel - wrapper around ns3::SimpleChannel

#ifndef DECT2020_CHANNEL_H
#define DECT2020_CHANNEL_H

#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "ns3/dect2020-net-device.h"

namespace ns3
{

class Channel;
class Dect2020NetDevice;

class Dect2020Channel : public Channel
{
    public:
        Dect2020Channel();
        virtual ~Dect2020Channel();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        void Add(Ptr<Dect2020NetDevice> device);
        void Send(Ptr<Packet> packet, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<Dect2020NetDevice> sender);
        Time GetDelay(void) const;

        // Virtual methods from Channel

        virtual std::size_t GetNDevices() const;
        virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

    private:
        Time m_delay;
        std::vector<Ptr<Dect2020NetDevice>> m_devices;
};

} // namespace ns3

#endif // DECT2020_CHANNEL_H