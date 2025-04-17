#include "dect-net-device.h"

DectNetDevice::DectNetDevice(){}

DectNetDevice::~DectNetDevice(){}

TypeId DectNetDevice::GetTypeId()
{
    static TypeId tid = TypeId("ns3::NetDevice").SetParent<Object>().SetGroupName("Network");
    return tid;
}

void DectNetDevice::SetIfIndex(const uint32_t index) {}

uint32_t DectNetDevice::GetIfIndex() const { return 0; }

Ptr<Channel> DectNetDevice::GetChannel() const { return nullptr; }

void DectNetDevice::SetAddress(Address address) {}

Address DectNetDevice::GetAddress() const { return Address(); }

bool DectNetDevice::SetMtu(const uint16_t mtu) { return false; }

uint16_t DectNetDevice::GetMtu() const { return 0; }

bool DectNetDevice::IsLinkUp() const { return false; }

void DectNetDevice::AddLinkChangeCallback(Callback<void> callback) {}

bool DectNetDevice::IsBroadcast() const { return false; }

Address DectNetDevice::GetBroadcast() const { return Address(); }

bool DectNetDevice::IsMulticast() const { return false; }

Address DectNetDevice::GetMulticast(Ipv4Address multicastGroup) const { return Address(); }

Address DectNetDevice::GetMulticast(Ipv6Address addr) const { return Address(); }

bool DectNetDevice::IsBridge() const { return false; }

bool DectNetDevice::IsPointToPoint() const { return false; }

bool DectNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) { return false; }

bool DectNetDevice::SendFrom(Ptr<Packet> packet,
                              const Address& source,
                              const Address& dest,
                              uint16_t protocolNumber) { return false; }

Ptr<Node> DectNetDevice::GetNode() const { return nullptr; }

void DectNetDevice::SetNode(Ptr<Node> node) {}

bool DectNetDevice::NeedsArp() const { return false; }

void DectNetDevice::SetReceiveCallback(ReceiveCallback cb) {}

void DectNetDevice::SetPromiscReceiveCallback(NetDevice::PromiscReceiveCallback cb) {}

bool DectNetDevice::SupportsSendFrom() const { return false; }
