#include "dect2020ext-net-device.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/node.h"
#include "ns3/channel.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtNetDevice");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtNetDevice);

TypeId
Dect2020ExtNetDevice::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtNetDevice")
                            .SetParent<NetDevice>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtNetDevice>()

                            .AddTraceSource("Tx",
                                            "Trace source for packets received by this device",
                                            MakeTraceSourceAccessor(&Dect2020ExtNetDevice::m_txTrace),
                                            "ns3::Packet::TracedCallback")

                            .AddTraceSource("Rx",
                                            "Trace source for packets received by this device",
                                            MakeTraceSourceAccessor(&Dect2020ExtNetDevice::m_rxTrace),
                                            "ns3::Packet::TracedCallback");
    return tid;
}

Dect2020ExtNetDevice::Dect2020ExtNetDevice()
    :m_node(nullptr),
    m_ifIndex(0),
    m_mtu(1500),
    m_linkUp(false),
    m_channel(nullptr)
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtNetDevice::~Dect2020ExtNetDevice()
{
    NS_LOG_FUNCTION(this);
}

/**
 * Assigns the MAC to the net device
 * Assigns the net device to the MAC
 */
void
Dect2020ExtNetDevice::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
    m_mac->SetNetDevice(this);
}

Ptr<Dect2020ExtMac>
Dect2020ExtNetDevice::GetMac() const
{
    return m_mac;
}


/**
 * Assigns the PHY to the net device
 * Assigns the net device to the PHY
 */
void
Dect2020ExtNetDevice::SetPhy(Ptr<Dect2020ExtPhy> phy)
{
    m_phy = phy;
    m_phy->SetDevice(this);
}

Ptr<Dect2020ExtPhy>
Dect2020ExtNetDevice::GetPhy() const
{
    return m_phy;
}

void
Dect2020ExtNetDevice::SetBandNumber(uint8_t bandNumber)
{
    m_bandNumber = bandNumber;
}

uint8_t
Dect2020ExtNetDevice::GetBandNumber() const
{
    return m_bandNumber;
}

void
Dect2020ExtNetDevice::SetTerminationPointType(Dect2020ExtTerminationPointType terminationPointType)
{
    m_terminationPointType = terminationPointType;
    SetBootstrappingVariables((terminationPointType == FT) ? true : false);
}

Dect2020ExtNetDevice::Dect2020ExtTerminationPointType
Dect2020ExtNetDevice::GetTerminationPointType() const
{
    return m_terminationPointType;
}

void
Dect2020ExtNetDevice::SetBootstrappingVariables(bool isFT)
{
    m_numOfPHYCapabilities = 0;
    m_release = 2;
    m_supportGroupAssignment = false;
    m_supportPaging = false;
    m_operatingModes = (isFT ? 1 : 0);
    m_mesh = false;
    m_scheduledAccessDataTransfer = false;
    m_macSecurity = false;
    m_dlcServiceType = 0;
    m_rdPowerClass = 0;
    m_maxNssFoRx = 0;
    m_rxForTxDiversity = 0;
    m_rxGain = 5; // 0 dB
    m_maxMcs = 11;
    m_softBufferSize = 0;
    m_numOfHarqProcesses = 1;
    m_harqFeedbackDelay = 2;
    m_dDelay = true;
    m_halfDulp = false;
}

void
Dect2020ExtNetDevice::SetIfIndex(const uint32_t index)
{
    m_ifIndex = index;
}

uint32_t
Dect2020ExtNetDevice::GetIfIndex() const
{
    return m_ifIndex;
}

Ptr<Channel>
Dect2020ExtNetDevice::GetChannel() const
{
    return m_channel;
}

void
Dect2020ExtNetDevice::SetAddress(Address address)
{
    m_address = address;
}

Address
Dect2020ExtNetDevice::GetAddress() const
{
    return m_address;
}

bool
Dect2020ExtNetDevice::SetMtu(const uint16_t mtu)
{
    m_mtu = mtu;
    return true;
}

uint16_t
Dect2020ExtNetDevice::GetMtu() const
{
    return m_mtu;
}

bool
Dect2020ExtNetDevice::IsLinkUp() const
{
    return m_linkUp;
}

void
Dect2020ExtNetDevice::AddLinkChangeCallback(Callback<void> callback)
{
    m_linkChanges.ConnectWithoutContext(callback);
}

bool
Dect2020ExtNetDevice::IsBroadcast() const
{
    return true;
}

Address
Dect2020ExtNetDevice::GetBroadcast() const
{
    return Mac48Address("ff:ff:ff:ff:ff:ff");
}

bool
Dect2020ExtNetDevice::IsMulticast() const
{
    return true;
}

Address
Dect2020ExtNetDevice::GetMulticast(Ipv4Address multicastGroup) const
{
    return Mac48Address::GetMulticast(multicastGroup);
}

Address
Dect2020ExtNetDevice::GetMulticast(Ipv6Address addr) const
{
    return Mac48Address::GetMulticast(addr);
}

bool
Dect2020ExtNetDevice::IsBridge() const
{
    return false;
}

bool
Dect2020ExtNetDevice::IsPointToPoint() const
{
    return false;
}

bool
Dect2020ExtNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
    NS_LOG_FUNCTION(this << packet << dest << protocolNumber);
    return true;
}

bool
Dect2020ExtNetDevice::SendFrom(Ptr<Packet> packet,
                                const Address& source,
                                const Address& dest,
                                uint16_t protocolNumber)
{
    NS_LOG_FUNCTION(this << packet << source << dest << protocolNumber);
    return true;
}

Ptr<Node>
Dect2020ExtNetDevice::GetNode() const
{
    return m_node;
}

void
Dect2020ExtNetDevice::SetNode(Ptr<Node> node)
{
    m_node = node;
}

bool
Dect2020ExtNetDevice::NeedsArp() const
{
    return false;
}

void
Dect2020ExtNetDevice::SetReceiveCallback(ReceiveCallback cb)
{
    m_receiveCallback = cb;
}

void
Dect2020ExtNetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback cb)
{
    m_promiscCallback = cb;
}

bool
Dect2020ExtNetDevice::SupportsSendFrom() const
{
    return true;
}

void
Dect2020ExtNetDevice::Receive(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
    NS_LOG_INFO("Received packet from PHY on NetDevice " << this);
}

} // namespace ns3
