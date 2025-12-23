#include "dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtClusterBeaconMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtClusterBeaconMessageIE);

Dect2020ExtClusterBeaconMessageIE::Dect2020ExtClusterBeaconMessageIE()
    : m_SFN(0),
      m_txPowerIncluded(false),
      m_powerConstraints(false),
      m_frameOffsetIncluded(false),
      m_nextChannelIncluded(false),
      m_timeToNextFieldPresent(false),
      m_networkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS),
      m_clusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS),
      m_countToTrigger(0),
      m_relQuality(0),
      m_minQuality(0),
      m_clusterMaxTXPower(4),
      m_frameOffset(0),
      m_nextClusterChannel(0),
      m_timeToNext(0)
{
}

Dect2020ExtClusterBeaconMessageIE::~Dect2020ExtClusterBeaconMessageIE()
{
}

TypeId
Dect2020ExtClusterBeaconMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtClusterBeaconMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtClusterBeaconMessageIE>();
    return tid;
}

TypeId
Dect2020ExtClusterBeaconMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtClusterBeaconMessageIE::GetSerializedSize() const
{
    return 11; // Size in bytes
}

void
Dect2020ExtClusterBeaconMessageIE::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = m_SFN;
    start.WriteU8(byte0);

    // byte 1
    // 3 first bits are reserved
    uint8_t byte1 = (m_txPowerIncluded << 4);
    byte1 |= (m_powerConstraints << 3);
    byte1 |= (m_frameOffsetIncluded << 2);
    byte1 |= (m_nextChannelIncluded << 1);
    byte1 |= m_timeToNextFieldPresent;
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
    uint8_t byte4 = (m_clusterMaxTXPower << 4);
    byte4 |= (m_frameOffset & 0x0F);
    start.WriteU8(byte4);

    // byte 5
    // first 3 bits are reserved
    uint8_t byte5 = (m_nextClusterChannel >> 8);
    start.WriteU8(byte5);

    // byte 6
    uint8_t byte6 = (m_nextClusterChannel & 0x00FF);
    start.WriteU8(byte6);

    // Time To Next
    start.WriteU32(m_timeToNext);
}

uint32_t
Dect2020ExtClusterBeaconMessageIE::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    // byte 0
    m_SFN = start.ReadU8();
    bytesRead++;

    // byte1
    uint8_t byte1 = start.ReadU8();
    m_txPowerIncluded = (byte1 & 0x10);
    m_powerConstraints = (byte1 & 0x08);
    m_frameOffsetIncluded = (byte1 & 0x04);
    m_nextChannelIncluded = (byte1 & 0x02);
    m_timeToNextFieldPresent = (byte1 & 0x01);
    bytesRead++;

    // byte 2
    uint8_t byte2 = start.ReadU8();
    m_networkBeaconPeriod = static_cast<Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod>((byte2 & 0xF0) >> 4);
    m_clusterBeaconPeriod = static_cast<Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod>((byte2 & 0x0F));
    bytesRead++;

    // byte 3
    uint8_t byte3 = start.ReadU8();
    m_countToTrigger = (byte3 & 0xF0) >> 4;
    m_relQuality = (byte3 & 0x0C) >> 2;
    m_minQuality = (byte3 & 0x03);
    bytesRead++;

    // byte 4
    uint8_t byte4 = start.ReadU8();
    m_clusterMaxTXPower = (byte4 & 0xF0) >> 4;
    m_frameOffset = (byte4 & 0x0F);
    bytesRead++;

    // Next Cluster Channel
    uint8_t byte5 = start.ReadU8();
    m_nextClusterChannel = byte5;
    bytesRead++;

    uint8_t byte6 = start.ReadU8();
    m_nextClusterChannel = (m_nextClusterChannel << 8);
    m_nextClusterChannel |= byte6;
    bytesRead++;

    // Time To Next
    m_timeToNext = start.ReadU32();
    bytesRead += 4;

    return bytesRead; // Number of bytes consumed
}

void
Dect2020ExtClusterBeaconMessageIE::Print(std::ostream& os) const
{
    os << "[DECT2020 CLUSTER BEACON MESSAGE]" << "\n"
       << "SFN: 0x" << std::hex << (int)GetSFN() << "\n"
       << "Power Constraints: 0x" << std::hex << (int)GetPowerConstraints() << "\n"
       << "Frame Offset Included: 0x" << std::hex << (int)GetFrameOffsetIncluded() << "\n"
       << "Next Channel Included: 0x" << std::hex << (int)GetNextChannelIncluded() << "\n"
       << "Time To Next Field Present: 0x" << std::hex << (int)GetTimeToNextFieldPresent() << "\n"
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
Dect2020ExtClusterBeaconMessageIE::SetSFN(uint8_t sfn)
{
    m_SFN = sfn;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetSFN() const
{
    return m_SFN;
}

// TxPower
void
Dect2020ExtClusterBeaconMessageIE::SetTxPowerIncluded(bool txPower)
{
    m_txPowerIncluded = txPower;
}

bool
Dect2020ExtClusterBeaconMessageIE::GetTxPowerIncluded() const
{
    return m_txPowerIncluded;
}

// PowerConst
void
Dect2020ExtClusterBeaconMessageIE::SetPowerConstraints(bool powerConstraints)
{
    m_powerConstraints = powerConstraints;
}

bool
Dect2020ExtClusterBeaconMessageIE::GetPowerConstraints() const
{
    return m_powerConstraints;
}

// FO
void
Dect2020ExtClusterBeaconMessageIE::SetFrameOffsetIncluded(bool frameOffsetIncluded)
{
    m_frameOffsetIncluded = frameOffsetIncluded;
}

bool
Dect2020ExtClusterBeaconMessageIE::GetFrameOffsetIncluded() const
{
    return m_frameOffsetIncluded;
}

// NextChannel
void
Dect2020ExtClusterBeaconMessageIE::SetNextChannelIncluded(bool nextChannelIncluded)
{
    m_nextChannelIncluded = nextChannelIncluded;
}

bool
Dect2020ExtClusterBeaconMessageIE::GetNextChannelIncluded() const
{
    return m_nextChannelIncluded;
}

// TimeToNext (1-bit version)
void
Dect2020ExtClusterBeaconMessageIE::SetTimeToNextFieldPresent(bool timeToNextFieldPresent)
{
    m_timeToNextFieldPresent = timeToNextFieldPresent;
}

bool
Dect2020ExtClusterBeaconMessageIE::GetTimeToNextFieldPresent() const
{
    return m_timeToNextFieldPresent;
}

// NetworkBeaconPeriod
void
Dect2020ExtClusterBeaconMessageIE::SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod period)
{
    m_networkBeaconPeriod = period;
}

Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod
Dect2020ExtClusterBeaconMessageIE::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

// ClusterBeaconPeriod
void
Dect2020ExtClusterBeaconMessageIE::SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod period)
{
    m_clusterBeaconPeriod = period;
}

Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod
Dect2020ExtClusterBeaconMessageIE::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

// CountToTrigger
void
Dect2020ExtClusterBeaconMessageIE::SetCountToTrigger(uint8_t count)
{
    m_countToTrigger = count;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetCountToTrigger() const
{
    return m_countToTrigger;
}

// RelQuality
void
Dect2020ExtClusterBeaconMessageIE::SetRelQuality(uint8_t quality)
{
    m_relQuality = quality;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetRelQuality() const
{
    return m_relQuality;
}

// MinQuality
void
Dect2020ExtClusterBeaconMessageIE::SetMinQuality(uint8_t quality)
{
    m_minQuality = quality;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetMinQuality() const
{
    return m_minQuality;
}

// ClusterMaxTXPower
void
Dect2020ExtClusterBeaconMessageIE::SetClusterMaxTXPower(uint8_t power)
{
    m_clusterMaxTXPower = power;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetClusterMaxTXPower() const
{
    return m_clusterMaxTXPower;
}

// FrameOffset
void
Dect2020ExtClusterBeaconMessageIE::SetFrameOffset(uint8_t offset)
{
    m_frameOffset = offset;
}

uint8_t
Dect2020ExtClusterBeaconMessageIE::GetFrameOffset() const
{
    return m_frameOffset;
}

// NextClusterChannel
void
Dect2020ExtClusterBeaconMessageIE::SetNextClusterChannel(uint16_t channel)
{
    m_nextClusterChannel = channel;
}

uint16_t
Dect2020ExtClusterBeaconMessageIE::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

// TimeToNext (4-byte version)
void
Dect2020ExtClusterBeaconMessageIE::SetTimeToNext(uint32_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ExtClusterBeaconMessageIE::GetTimeToNext() const
{
    return m_timeToNext;
}

} // namespace ns3 