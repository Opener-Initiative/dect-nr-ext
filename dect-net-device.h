#ifndef DECT_NET_DEVICE_H
#define DECT_NET_DEVICE_H

#include "ns3/address.h"
#include "ns3/packet.h"
#include "ns3/net-device.h"

#include "ns3/callback.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/channel.h"

#include <stdint.h>

using namespace ns3;

class DectNetDevice : public NetDevice
{
  public:
    static TypeId GetTypeId();
    DectNetDevice();
    ~DectNetDevice();
    void SetIfIndex(const uint32_t index) override;
    uint32_t GetIfIndex() const override;
    Ptr<Channel> GetChannel() const override;
    void SetAddress(Address address) override;
    Address GetAddress() const override;
    bool SetMtu(const uint16_t mtu) override;
    uint16_t GetMtu() const override;
    bool IsLinkUp() const override;
    typedef void (*LinkChangeTracedCallback)();
    void AddLinkChangeCallback(Callback<void> callback) override;
    bool IsBroadcast() const override;
    Address GetBroadcast() const override;
    bool IsMulticast() const override;
    Address GetMulticast(Ipv4Address multicastGroup) const override;
    Address GetMulticast(Ipv6Address addr) const override;
    bool IsBridge() const override;
    bool IsPointToPoint() const override;
    bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;
    bool SendFrom(Ptr<Packet> packet,
                const Address& source,
                const Address& dest,
                uint16_t protocolNumber) override;
    Ptr<Node> GetNode() const override;
    void SetNode(Ptr<Node> node) override;
    bool NeedsArp() const override;
    enum PacketType
    {
        PACKET_HOST = 1, //!< Packet addressed to us
        NS3_PACKET_HOST = PACKET_HOST,
        PACKET_BROADCAST, //!< Packet addressed to all
        NS3_PACKET_BROADCAST = PACKET_BROADCAST,
        PACKET_MULTICAST, //!< Packet addressed to multicast group
        NS3_PACKET_MULTICAST = PACKET_MULTICAST,
        PACKET_OTHERHOST, //!< Packet addressed to someone else
        NS3_PACKET_OTHERHOST = PACKET_OTHERHOST,
    };
    typedef Callback<bool, Ptr<NetDevice>, Ptr<const Packet>, uint16_t, const Address&>
        ReceiveCallback;
    void SetReceiveCallback(ReceiveCallback cb) override;

    typedef Callback<bool,
                    Ptr<NetDevice>,
                    Ptr<const Packet>,
                    uint16_t,
                    const Address&,
                    const Address&,
                    PacketType>
        PromiscReceiveCallback;

    void SetPromiscReceiveCallback(NetDevice::PromiscReceiveCallback cb) override;
    bool SupportsSendFrom() const override;
};

#endif /* DECT_NET_DEVICE_H */