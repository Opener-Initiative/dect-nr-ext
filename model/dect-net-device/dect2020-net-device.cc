#include "dect2020-net-device.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/drop-tail-queue.h"

namespace ns3
{

class Dect2020Tag : public Tag
{
  public:
    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const override;

    uint32_t GetSerializedSize() const override;
    void Serialize(TagBuffer i) const override;
    void Deserialize(TagBuffer i) override;

    void SetSrc(Mac48Address src);
    Mac48Address GetSrc() const;
    void SetDst(Mac48Address dst);
    Mac48Address GetDst() const;
    void SetProto(uint16_t proto);
    uint16_t GetProto() const;

    void Print(std::ostream& os) const override;

  private:
    Mac48Address m_src;        //!< source address
    Mac48Address m_dst;        //!< destination address
    uint16_t m_protocolNumber; //!< protocol number
};

NS_OBJECT_ENSURE_REGISTERED(Dect2020Tag);

TypeId
Dect2020Tag::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020Tag")
                            .SetParent<Tag>()
                            .AddConstructor<Dect2020Tag>();
    return tid;
}

TypeId
Dect2020Tag::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020Tag::GetSerializedSize() const
{
    return 8 + 8 + 2;
}

void
Dect2020Tag::Serialize(TagBuffer i) const
{
    uint8_t mac[6];
    m_src.CopyTo(mac);
    i.Write(mac, 6);
    m_dst.CopyTo(mac);
    i.Write(mac, 6);
    i.WriteU16(m_protocolNumber);
}

void
Dect2020Tag::Deserialize(TagBuffer i)
{
    uint8_t mac[6];
    i.Read(mac, 6);
    m_src.CopyFrom(mac);
    i.Read(mac, 6);
    m_dst.CopyFrom(mac);
    m_protocolNumber = i.ReadU16();
}

void
Dect2020Tag::SetSrc(Mac48Address src)
{
    m_src = src;
}

Mac48Address
Dect2020Tag::GetSrc() const
{
    return m_src;
}

void
Dect2020Tag::SetDst(Mac48Address dst)
{
    m_dst = dst;
}

Mac48Address
Dect2020Tag::GetDst() const
{
    return m_dst;
}

void
Dect2020Tag::SetProto(uint16_t proto)
{
    m_protocolNumber = proto;
}

uint16_t
Dect2020Tag::GetProto() const
{
    return m_protocolNumber;
}

void
Dect2020Tag::Print(std::ostream& os) const
{
    os << "src=" << m_src << " dst=" << m_dst << " proto=" << m_protocolNumber;
}

NS_LOG_COMPONENT_DEFINE("Dect2020NetDevice");
NS_OBJECT_ENSURE_REGISTERED(Dect2020NetDevice);

Dect2020NetDevice::Dect2020NetDevice()
{
    m_mac = nullptr;
    m_queue = CreateObject<DropTailQueue<Packet>>(); // <-- This is critical!
    m_bps = DataRate("0b/s");
    m_node = nullptr;
    m_mtu = 1500;
    m_linkUp = false;
    m_ifIndex = 0;
}

Dect2020NetDevice::~Dect2020NetDevice()
{
    m_mac = nullptr;
    m_queue = CreateObject<DropTailQueue<Packet>>();
}

TypeId
Dect2020NetDevice::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020NetDevice")
        .SetParent<NetDevice>()
        .AddConstructor<Dect2020NetDevice>()
        .AddAttribute("TxQueue",
                        "A queue to use as the transmit queue in the device.",
                        StringValue("ns3::DropTailQueue<Packet>"),
                        MakePointerAccessor(&Dect2020NetDevice::m_queue),
                        MakePointerChecker<Queue<Packet>>());

    return tid;
}

TypeId
Dect2020NetDevice::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020NetDevice::StartTransmission()
{
    NS_LOG_FUNCTION(this);
    if (m_queue->GetNPackets() == 0)
    {
        return;
    }
    
    NS_ASSERT_MSG(!FinishTransmissionEvent.IsPending(),
                  "Tried to transmit a packet while another transmission was in progress");
    Ptr<Packet> packet = m_queue->Dequeue();

    Time txTime = Time(0);
    if (m_bps > DataRate(0))
    {
        txTime = m_bps.CalculateBytesTxTime(packet->GetSize());
    }
    FinishTransmissionEvent =
        Simulator::Schedule(txTime, &Dect2020NetDevice::FinishTransmission, this, packet);
}

void
Dect2020NetDevice::FinishTransmission(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this);

    Dect2020Tag tag;
    packet->RemovePacketTag(tag);

    Mac48Address src = tag.GetSrc();
    Mac48Address dst = tag.GetDst();
    uint16_t proto = tag.GetProto();

    m_channel->Send(packet, proto, dst, src, this);

    StartTransmission();
}

void
Dect2020NetDevice::Receive(Ptr<Packet> packet, uint16_t protocol, Mac48Address to, Mac48Address from)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_FUNCTION(this << packet << protocol << to << from);
    NetDevice::PacketType packetType;

    // if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt(packet))
    // {
    //     m_phyRxDropTrace(packet);
    //     return;
    // }

    if (to == m_address)
    {
        packetType = NetDevice::PACKET_HOST;
    }
    else if (to.IsBroadcast())
    {
        packetType = NetDevice::PACKET_BROADCAST;
    }
    else if (to.IsGroup())
    {
        packetType = NetDevice::PACKET_MULTICAST;
    }
    else
    {
        packetType = NetDevice::PACKET_OTHERHOST;
    }

    if (packetType != NetDevice::PACKET_OTHERHOST)
    {
        m_rxCallback(this, packet, protocol, from);
    }

    // if (!m_promiscCallback.IsNull())
    // {
    //     m_promiscCallback(this, packet, protocol, from, to, packetType);
    // }
}

void
Dect2020NetDevice::SetChannel(Ptr<Dect2020Channel> channel)
{
    NS_LOG_FUNCTION(this);
    m_channel = channel;
    m_channel->Add(this);
}

// Virtual methods from NetDevice

void
Dect2020NetDevice::SetIfIndex(uint32_t index)
{
    m_ifIndex = index;
}

uint32_t
Dect2020NetDevice::GetIfIndex() const
{
    return m_ifIndex;
}

Ptr<Channel>
Dect2020NetDevice::GetChannel() const
{
    return m_channel;
}

void
Dect2020NetDevice::SetAddress(Address address)
{
    m_address = Mac48Address::ConvertFrom(address);
}

Address
Dect2020NetDevice::GetAddress() const
{
    return m_address;
}  

bool
Dect2020NetDevice::SetMtu(const uint16_t mtu)
{
    m_mtu = mtu;
    return true;
}

uint16_t
Dect2020NetDevice::GetMtu() const
{
    return m_mtu;
}

bool
Dect2020NetDevice::IsLinkUp() const
{
    return m_linkUp;
}

void
Dect2020NetDevice::AddLinkChangeCallback(Callback<void> callback)
{
    m_linkChangeCallbacks.ConnectWithoutContext(callback);
}

bool
Dect2020NetDevice::IsBroadcast() const
{
    return true;
}

Address
Dect2020NetDevice::GetBroadcast() const
{
    return m_address;
}

bool
Dect2020NetDevice::IsMulticast() const
{
    return true;
}

Address
Dect2020NetDevice::GetMulticast(Ipv4Address multicastGroup) const
{
    return Mac48Address::GetMulticast(multicastGroup);
}

Address
Dect2020NetDevice::GetMulticast(Ipv6Address multicastGroup) const
{
    return Mac48Address::GetMulticast(multicastGroup);
}

bool
Dect2020NetDevice::IsBridge() const
{
    return false;
}

bool
Dect2020NetDevice::IsPointToPoint() const
{
    return false;
}

bool
Dect2020NetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
    if(m_mac != nullptr)
    {
        m_mac->ProcessPacket(packet);
    }
    return true;
}

bool
Dect2020NetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
    NS_LOG_FUNCTION(this);
    if(m_mac != nullptr)
    {
        m_mac->ProcessPacket(packet);
    }

    NS_LOG_FUNCTION(this << packet << source << dest << protocolNumber);
    if (packet->GetSize() > m_mtu)
    {
        return false;
    }

    Mac48Address to = Mac48Address::ConvertFrom(dest);
    Mac48Address from = Mac48Address::ConvertFrom(source);

    // Simple option >>>>>>>>>>>>>>>>>
    m_channel->Send(packet, protocolNumber, to, from, this);
    return true;
    // Simple option >>>>>>>>>>>>>>>>>

    // Complex option >>>>>>>>>>>>>>>>>
    // Dect2020Tag tag;
    // tag.SetSrc(from);
    // tag.SetDst(to);
    // tag.SetProto(protocolNumber);

    // packet->AddPacketTag(tag);

    // if (m_queue->Enqueue(packet))
    // {
    //     if (m_queue->GetNPackets() == 1 && !FinishTransmissionEvent.IsPending())
    //     {
    //         StartTransmission();
    //     }
    //     return true;
    // }

    // return false;
    // Complex option >>>>>>>>>>>>>>>>>
}


Ptr<Node>
Dect2020NetDevice::GetNode() const
{
    return m_node;
}

void
Dect2020NetDevice::SetNode(Ptr<Node> node)
{
    m_node = node;
}

bool
Dect2020NetDevice::NeedsArp() const
{
    return false;
}

void
Dect2020NetDevice::SetReceiveCallback(ReceiveCallback cb)
{
    m_rxCallback = cb;
}

void
Dect2020NetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback cb)
{
    m_promiscRxCallback = cb;
}

bool
Dect2020NetDevice::SupportsSendFrom() const
{
    return true;
}

// Dect2020NetDevice Setters and Getters

void
Dect2020NetDevice::SetMac(const Ptr<Dect2020Mac> mac)
{
    m_mac = mac;
    m_mac->SetNetDevice(this);
}

Ptr<Dect2020Mac>
Dect2020NetDevice::GetMac() const
{
    return m_mac;
}

void
Dect2020NetDevice::SetRdMode(RDMode rdMode)
{
    m_rdMode = rdMode;
}

Dect2020NetDevice::RDMode
Dect2020NetDevice::GetRdMode() const
{
    return m_rdMode;
}

} // namespace ns3