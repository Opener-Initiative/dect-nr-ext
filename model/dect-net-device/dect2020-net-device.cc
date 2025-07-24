#include "dect2020-net-device.h"

namespace ns3
{

Dect2020NetDevice::Dect2020NetDevice()
{
    m_simpleNetDevice = CreateObject<SimpleNetDevice>();
    m_mac = nullptr;
}

Dect2020NetDevice::~Dect2020NetDevice()
{
    m_simpleNetDevice = nullptr;
    m_mac = nullptr;
}

TypeId
Dect2020NetDevice::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020NetDevice")
        .SetParent<NetDevice>()
        .AddConstructor<Dect2020NetDevice>();

    return tid;
}

TypeId
Dect2020NetDevice::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

// Virtual methods from NetDevice

void
Dect2020NetDevice::SetIfIndex(uint32_t index)
{
    m_simpleNetDevice->SetIfIndex(index);
}

uint32_t
Dect2020NetDevice::GetIfIndex() const
{
    return m_simpleNetDevice->GetIfIndex();
}

Ptr<Channel>
Dect2020NetDevice::GetChannel() const
{
    return m_simpleNetDevice->GetChannel();
}

void
Dect2020NetDevice::SetAddress(Address address)
{
    m_simpleNetDevice->SetAddress(address);
}

Address
Dect2020NetDevice::GetAddress() const
{
    return m_simpleNetDevice->GetAddress();
}  

bool
Dect2020NetDevice::SetMtu(const uint16_t mtu)
{
    return m_simpleNetDevice->SetMtu(mtu);
}

uint16_t
Dect2020NetDevice::GetMtu() const
{
    return m_simpleNetDevice->GetMtu();
}

bool
Dect2020NetDevice::IsLinkUp() const
{
    return m_simpleNetDevice->IsLinkUp();
}

void
Dect2020NetDevice::AddLinkChangeCallback(Callback<void> callback)
{
    m_simpleNetDevice->AddLinkChangeCallback(callback);
}

bool
Dect2020NetDevice::IsBroadcast() const
{
    return m_simpleNetDevice->IsBroadcast();
}

Address
Dect2020NetDevice::GetBroadcast() const
{
    return m_simpleNetDevice->GetBroadcast();
}

bool
Dect2020NetDevice::IsMulticast() const
{
    return m_simpleNetDevice->IsMulticast();
}

Address
Dect2020NetDevice::GetMulticast(Ipv4Address multicastGroup) const
{
    return m_simpleNetDevice->GetMulticast(multicastGroup);
}

Address
Dect2020NetDevice::GetMulticast(Ipv6Address multicastGroup) const
{
    return m_simpleNetDevice->GetMulticast(multicastGroup);
}

bool
Dect2020NetDevice::IsBridge() const
{
    return m_simpleNetDevice->IsBridge();
}

bool
Dect2020NetDevice::IsPointToPoint() const
{
    return m_simpleNetDevice->IsPointToPoint();
}

bool
Dect2020NetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
    if(m_mac != nullptr)
    {
        m_mac->ProcessPacket(packet);
    }
    return m_simpleNetDevice->Send(packet, dest, protocolNumber);
}

bool
Dect2020NetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
    return m_simpleNetDevice->SendFrom(packet, source, dest, protocolNumber);
}

Ptr<Node>
Dect2020NetDevice::GetNode() const
{
    return m_simpleNetDevice->GetNode();
}

void
Dect2020NetDevice::SetNode(Ptr<Node> node)
{
    m_simpleNetDevice->SetNode(node);
}

bool
Dect2020NetDevice::NeedsArp() const
{
    return m_simpleNetDevice->NeedsArp();
}

void
Dect2020NetDevice::SetReceiveCallback(ReceiveCallback cb)
{
    m_simpleNetDevice->SetReceiveCallback(cb);
}

void
Dect2020NetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback cb)
{
    m_simpleNetDevice->SetPromiscReceiveCallback(cb);
}

bool
Dect2020NetDevice::SupportsSendFrom() const
{
    return m_simpleNetDevice->SupportsSendFrom();
}

// Wrapped methods from SimpleNetDevice

void
Dect2020NetDevice::SetChannel(Ptr<SimpleChannel> channel)
{
    m_simpleNetDevice->SetChannel(channel);
}

Ptr<SimpleNetDevice>
Dect2020NetDevice::GetWrappedDevicePointer() const
{
    return m_simpleNetDevice;
}

void
Dect2020NetDevice::SetMac(const Ptr<Dect2020Mac> mac)
{
    m_mac = mac;
    m_mac->SetNetDevice(this);
}

} // namespace ns3