#include "dect2020ext-beacon-header.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtBeaconHeader");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtBeaconHeader);

Dect2020ExtBeaconHeader::Dect2020ExtBeaconHeader()
{
}

Dect2020ExtBeaconHeader::~Dect2020ExtBeaconHeader()
{
}

TypeId
Dect2020ExtBeaconHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtBeaconHeader")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtBeaconHeader>();
    return tid;
}

TypeId
Dect2020ExtBeaconHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtBeaconHeader::Print(std::ostream& os) const
{
    os << "[DECT2020 BEACON HEADER]" << "\n"
       << "Network ID: 0x" << std::hex << m_networkId << "\n"
       << "Transmitter Address: 0x" << std::hex << m_transmitterAddress << std::dec;
}

uint32_t
Dect2020ExtBeaconHeader::GetSerializedSize() const
{
    return 7;
}

void
Dect2020ExtBeaconHeader::Serialize(Buffer::Iterator start) const
{
    // 3 bytes
    start.WriteU8((m_networkId >> 16) & 0xFF);
    start.WriteU8((m_networkId >> 8) & 0xFF);
    start.WriteU8(m_networkId & 0xFF);

    // 4 bytes
    start.WriteHtonU32(m_transmitterAddress);
}

uint32_t
Dect2020ExtBeaconHeader::Deserialize(Buffer::Iterator start)
{
    // 3 bytes
    uint32_t networkId = 0;
    networkId |= (start.ReadU8() << 16);
    networkId |= (start.ReadU8() << 8);
    networkId |= start.ReadU8();
    m_networkId = networkId;

    // 4 bytes
    m_transmitterAddress = start.ReadNtohU32();

    return GetSerializedSize();
}

void
Dect2020ExtBeaconHeader::SetNetworkId(uint32_t networkId)
{
    m_networkId = networkId & 0xFFFFFF;
    if ((networkId & 0xFFFFFF) > 0xFFFFFF)
    {
        DECT2020EXT_MAC_PDU_FATAL("Network ID must be at most 24 bits long", networkId);
    }
}

uint32_t
Dect2020ExtBeaconHeader::GetNetworkId() const
{
    return m_networkId;
}

void
Dect2020ExtBeaconHeader::SetTransmitterAddress(uint32_t transmitterAddress)
{
    m_transmitterAddress = transmitterAddress;
}

uint32_t
Dect2020ExtBeaconHeader::GetTransmitterAddress() const
{
    return m_transmitterAddress;
}

} // namespace ns3
