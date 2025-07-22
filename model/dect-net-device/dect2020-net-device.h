#ifndef DECT2020_NET_DEVICE_H
#define DECT2020_NET_DEVICE_H

#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/net-device.h"
#include "ns3/node.h"

namespace ns3
{

class Dect2020NetDevice : public NetDevice
{
    public:
        Dect2020NetDevice();
        ~Dect2020NetDevice();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        // Virtual methods from NetDevice

        void SetIfIndex(uint32_t index);
        uint32_t GetIfIndex() const;
        Ptr<Channel> GetChannel() const;
        void SetAddress(Address address);
        Address GetAddress() const;
        bool SetMtu(const uint16_t mtu);
        uint16_t GetMtu() const;
        bool IsLinkUp() const;
        void AddLinkChangeCallback(Callback<void> callback);
        bool IsBroadcast() const;
        Address GetBroadcast() const;
        bool IsMulticast() const;
        Address GetMulticast(Ipv4Address multicastGroup) const;
        Address GetMulticast(Ipv6Address multicastGroup) const;
        bool IsBridge() const;
        bool IsPointToPoint() const;
        bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
        bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
        Ptr<Node> GetNode() const;
        void SetNode(Ptr<Node> node);
        bool NeedsArp() const;
        void SetReceiveCallback(ReceiveCallback cb);
        void SetPromiscReceiveCallback(PromiscReceiveCallback cb);
        bool SupportsSendFrom() const;

        void SetChannel(Ptr<SimpleChannel> channel);

        // Setters and Getters

        Ptr<SimpleNetDevice> GetWrappedDevicePointer() const;

    private:
        Ptr<SimpleNetDevice> m_simpleNetDevice;
};

} // namespace ns3

#endif // DECT2020_NET_DEVICE_H