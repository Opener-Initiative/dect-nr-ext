#ifndef DECT2020_NET_DEVICE_H
#define DECT2020_NET_DEVICE_H

#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/dect2020-mac.h"

namespace ns3
{

class Dect2020Mac;

class Dect2020NetDevice : public NetDevice
{
    public:
        Dect2020NetDevice();
        ~Dect2020NetDevice();

        static TypeId GetTypeId(void);
        TypeId GetInstanceTypeId(void) const;

        // Virtual methods from NetDevice

        void SetIfIndex(uint32_t index) override;
        uint32_t GetIfIndex() const override;
        Ptr<Channel> GetChannel() const override;
        void SetAddress(Address address) override;
        Address GetAddress() const override;
        bool SetMtu(const uint16_t mtu) override;
        uint16_t GetMtu() const override;
        bool IsLinkUp() const override;
        void AddLinkChangeCallback(Callback<void> callback) override;
        bool IsBroadcast() const override;
        Address GetBroadcast() const override;
        bool IsMulticast() const override;
        Address GetMulticast(Ipv4Address multicastGroup) const override;
        Address GetMulticast(Ipv6Address multicastGroup) const override;
        bool IsBridge() const override;
        bool IsPointToPoint() const override;
        bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;
        bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber) override;
        Ptr<Node> GetNode() const override;
        void SetNode(Ptr<Node> node) override;
        bool NeedsArp() const override;
        void SetReceiveCallback(ReceiveCallback cb) override;
        void SetPromiscReceiveCallback(PromiscReceiveCallback cb) override;
        bool SupportsSendFrom() const override;

        // SimpleNetDevice wrapped methods

        void SetChannel(Ptr<SimpleChannel> channel);

        // Dect2020NetDevice Setters and Getters

        Ptr<SimpleNetDevice> GetWrappedDevicePointer() const;
        void SetMac(const Ptr<Dect2020Mac> mac);
        Ptr<Dect2020Mac> GetMac() const;

    private:
        Ptr<SimpleNetDevice> m_simpleNetDevice;
        Ptr<Dect2020Mac> m_mac;
};

} // namespace ns3

#endif // DECT2020_NET_DEVICE_H