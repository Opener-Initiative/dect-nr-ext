#include "dect2020-cluster-beacon-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ClusterBeaconMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ClusterBeaconMessageIE);

Dect2020ClusterBeaconMessageIE::Dect2020ClusterBeaconMessageIE()
{
}

Dect2020ClusterBeaconMessageIE::~Dect2020ClusterBeaconMessageIE()
{
}

TypeId
Dect2020ClusterBeaconMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ClusterBeaconMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ClusterBeaconMessageIE>();
    return tid;
}

TypeId
Dect2020ClusterBeaconMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ClusterBeaconMessageIE::GetSerializedSize() const
{
    return 12; // Size in bytes
}

void
Dect2020ClusterBeaconMessageIE::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = m_SFN;
    start.WriteU8(byte0);

    // byte 1
    // 4 first bits are reserved
    uint8_t byte1 = (m_powerConst << 3);
    byte1 |= (m_FO << 2);
    byte1 |= (m_nextChannel << 1);
    byte1 |= m_timeToNext;
    start.WriteU8(byte1);

    // byte 2
    uint8_t byte2 = (m_networkBeaconPeriod << 4);
    byte2 |= m_clusterBeaconPeriod;
    start.WriteU8(byte2);

    // byte 3
    uint8_t byte3 = (m_countToTrigger << 4);
    byte3 |= (m_relQuality << 2);
    byte3 |= m_minQuality;
    start.WriteU8(byte3);

    // byte 4
    start.WriteU8(m_frameOffset);

    // byte 5
    // first 3 bits are reserved
    uint8_t byte5 = (m_nextClusterChannel >> 8);
    start.WriteU8(byte5);

    // byte 6
    uint8_t byte6 = (m_nextClusterChannel && 0x00FF);
    start.WriteU8(byte6);

    // Time To Next
    start.WriteU32(m_timeToNext);
}

uint32_t
Dect2020ClusterBeaconMessageIE::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    // byte 0
    m_SFN = start.ReadU8();
    bytesRead++;

    // byte1
    uint8_t byte1 = start.ReadU8();
    m_powerConst = (byte1 & 0x08);
    m_FO = (byte1 & 0x04);
    m_nextChannel = (byte1 & 0x02);
    m_timeToNext = (byte1 & 0x01);
    bytesRead++;

    // byte 2
    uint8_t byte2 = start.ReadU8();
    m_networkBeaconPeriod = (byte2 & 0xF0);
    m_clusterBeaconPeriod = (byte2 & 0x0F);
    bytesRead++;

    // byte 3
    uint8_t byte3 = start.ReadU8();
    m_countToTrigger = (byte3 & 0xF0);
    m_relQuality = (byte3 & 0x0C);
    m_minQuality = (byte3 & 0x03);
    bytesRead++;

    // byte 4
    m_frameOffset = start.ReadU8();
    bytesRead++;

    // Next Cluster Channel
    uint8_t byte5 = start.ReadU8();
    m_nextClusterChannel = byte5;
    bytesRead++;

    uint8_t byte6 = start.ReadU8();
    m_nextClusterChannel = (m_nextClusterChannel << 8);
    m_nextClusterChannel = (m_nextClusterChannel & byte6);
    bytesRead++;

    // Time To Next
    m_timeToNext = start.ReadU32();
    bytesRead += 4;

    return bytesRead; // Number of bytes consumed
}

void
Dect2020ClusterBeaconMessageIE::Print(std::ostream& os) const
{
    os << "[DECT2020 CLUSTER BEACON MESSAGE]" << "\n"
       << "SFN: 0x" << std::hex << (int)GetSFN() << "\n"
       << "Power Const: 0x" << std::hex << (int)GetPowerConst() << "\n"
       << "FO: 0x" << std::hex << (int)GetFO() << "\n"
       << "Next Channel: 0x" << std::hex << (int)GetNextChannel() << "\n"
       << "Time To Next (1-bit): 0x" << std::hex << (int)GetTimeToNextBit() << "\n"
       << "Network Beacon Period: 0x" << std::hex << (int)GetNetworkBeaconPeriod() << "\n"
       << "Cluster Beacon Period: 0x" << std::hex << (int)GetClusterBeaconPeriod() << "\n"
       << "Count To Trigger: 0x" << std::hex << (int)GetCountToTrigger() << "\n"
       << "Relative Quality: 0x" << std::hex << (int)GetRelQuality() << "\n"
       << "Minimum Quality: 0x" << std::hex << (int)GetMinQuality() << "\n"
       << "Frame Offset: 0x" << std::hex << (int)GetFrameOffset() << "\n"
       << "Next Cluster Channel: 0x" << std::hex << (int)GetNextClusterChannel() << "\n"
       << "Time To Next (4-byte): 0x" << std::hex << (int)GetTimeToNext() << "\n";
}

// SFN
void
Dect2020ClusterBeaconMessageIE::SetSFN(uint8_t sfn)
{
    m_SFN = sfn;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetSFN() const
{
    return m_SFN;
}

// PowerConst
void
Dect2020ClusterBeaconMessageIE::SetPowerConst(uint8_t powerConst)
{
    m_powerConst = powerConst;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetPowerConst() const
{
    return m_powerConst;
}

// FO
void
Dect2020ClusterBeaconMessageIE::SetFO(uint8_t fo)
{
    m_FO = fo;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetFO() const
{
    return m_FO;
}

// NextChannel
void
Dect2020ClusterBeaconMessageIE::SetNextChannel(uint8_t nextChannel)
{
    m_nextChannel = nextChannel;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetNextChannel() const
{
    return m_nextChannel;
}

// TimeToNext (1-bit version)
void
Dect2020ClusterBeaconMessageIE::SetTimeToNextBit(uint8_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetTimeToNextBit() const
{
    return m_timeToNext;
}

// NetworkBeaconPeriod
void
Dect2020ClusterBeaconMessageIE::SetNetworkBeaconPeriod(uint8_t period)
{
    m_networkBeaconPeriod = period;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

// ClusterBeaconPeriod
void
Dect2020ClusterBeaconMessageIE::SetClusterBeaconPeriod(uint8_t period)
{
    m_clusterBeaconPeriod = period;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

// CountToTrigger
void
Dect2020ClusterBeaconMessageIE::SetCountToTrigger(uint8_t count)
{
    m_countToTrigger = count;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetCountToTrigger() const
{
    return m_countToTrigger;
}

// RelQuality
void
Dect2020ClusterBeaconMessageIE::SetRelQuality(uint8_t quality)
{
    m_relQuality = quality;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetRelQuality() const
{
    return m_relQuality;
}

// MinQuality
void
Dect2020ClusterBeaconMessageIE::SetMinQuality(uint8_t quality)
{
    m_minQuality = quality;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetMinQuality() const
{
    return m_minQuality;
}

// ClusterMaxTXPower
void
Dect2020ClusterBeaconMessageIE::SetClusterMaxTXPower(uint8_t power)
{
    m_clusterMaxTXPower = power;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetClusterMaxTXPower() const
{
    return m_clusterMaxTXPower;
}

// FrameOffset
void
Dect2020ClusterBeaconMessageIE::SetFrameOffset(uint8_t offset)
{
    m_frameOffset = offset;
}

uint8_t
Dect2020ClusterBeaconMessageIE::GetFrameOffset() const
{
    return m_frameOffset;
}

// NextClusterChannel
void
Dect2020ClusterBeaconMessageIE::SetNextClusterChannel(uint16_t channel)
{
    m_nextClusterChannel = channel;
}

uint16_t
Dect2020ClusterBeaconMessageIE::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

// TimeToNext (4-byte version)
void
Dect2020ClusterBeaconMessageIE::SetTimeToNext(uint32_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ClusterBeaconMessageIE::GetTimeToNext() const
{
    return m_timeToNext;
}

} // namespace ns3 