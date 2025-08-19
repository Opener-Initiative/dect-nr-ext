#include "dect2020ext-beacon-header.h"

#include "ns3/address-utils.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtBeaconHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtBeaconHeader);

Dect2020ExtBeaconHeader::Dect2020ExtBeaconHeader()
    : m_networkId(0),
      m_transmitterAddress(0)
{
}

Dect2020ExtBeaconHeader::~Dect2020ExtBeaconHeader()
{
}

TypeId
Dect2020ExtBeaconHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtBeaconHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtBeaconHeader>();
    return tid;
}

TypeId
Dect2020ExtBeaconHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020ExtBeaconHeader::SetNetworkId(uint32_t networkId)
{
    m_networkId = networkId & 0xFFFFFF; // Nur die unteren 24 Bits speichern
}

uint32_t
Dect2020ExtBeaconHeader::GetNetworkId() const
{
    return m_networkId;
}

void
Dect2020ExtBeaconHeader::SetTransmitterAddress(uint32_t transmitterId)
{
    m_transmitterAddress = transmitterId;
}

uint32_t
Dect2020ExtBeaconHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

void
Dect2020ExtBeaconHeader::Print(std::ostream& os) const
{
    os << "Beacon Header:" << std::endl << "NetworkId = 0x" << std::hex << m_networkId << std::endl
       << "TransmitterAddress = 0x" << std::hex << m_transmitterAddress << std::endl;
}

uint32_t
Dect2020ExtBeaconHeader::GetSerializedSize(void) const
{
    return 7; // 24 Bits Network ID + 32 Bits Transmitter Address = 56 Bits => 7 Bytes
}

void
Dect2020ExtBeaconHeader::Serialize(Buffer::Iterator start) const
{
    // Network ID (24 Bits)
    start.WriteU8((m_networkId >> 16) & 0xFF);
    start.WriteU8((m_networkId >> 8) & 0xFF);
    start.WriteU8(m_networkId & 0xFF);

    // Transmitter Address (32 Bits)
    start.WriteHtonU32(m_transmitterAddress);
}

uint32_t
Dect2020ExtBeaconHeader::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;

    // Network ID (24 Bits)
    uint32_t networkId = 0;
    networkId |= (i.ReadU8() << 16);
    networkId |= (i.ReadU8() << 8);
    networkId |= i.ReadU8();
    m_networkId = networkId;

    // Transmitter Address (32 Bits)
    m_transmitterAddress = i.ReadNtohU32();

    return GetSerializedSize();
}

} // namespace ns3