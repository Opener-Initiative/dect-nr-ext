#include "dect2020ext-cluster-beacon-message-ie.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtClusterBeaconMessageIe");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtClusterBeaconMessageIe);

Dect2020ExtClusterBeaconMessageIe::Dect2020ExtClusterBeaconMessageIe()
    : m_SFN(0),
        m_txPowerIncluded(false),
        m_powerConstraints(false),
        m_frameOffsetIncluded(false),
        m_nextChannelIncluded(false),
        m_timeToNextFieldPresent(false),
        m_networkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS),
        m_clusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS),
        m_countToTrigger(0),
        m_relQuality(0),
        m_minQuality(0),
        m_clusterMaxTxPower(4),
        m_frameOffset(0),
        m_nextClusterChannel(0),
        m_timeToNext(0)
{
}

Dect2020ExtClusterBeaconMessageIe::~Dect2020ExtClusterBeaconMessageIe()
{
}

TypeId
Dect2020ExtClusterBeaconMessageIe::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtClusterBeaconMessageIe")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtClusterBeaconMessageIe>();
    return tid;
}

TypeId
Dect2020ExtClusterBeaconMessageIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtClusterBeaconMessageIe::GetSerializedSize() const
{
    uint32_t sizeInBits = 0;

    sizeInBits += 8; // Byte 0: SFN
    sizeInBits += 8; // Byte 1: Flags
    sizeInBits += 8; // Byte 2: Network/Cluster Beacon Period
    sizeInBits += 8; // Byte 3: CountToTrigger, RelQuality, MinQuality

    if (m_txPowerIncluded)
    {
        sizeInBits += 8; // Byte 4: Cluster Max TX Power + reserved
    }

    if (m_frameOffsetIncluded)
    {
        sizeInBits += 8; // Byte 5: FrameOffset (8 Bit variant)
    }

    if (m_nextChannelIncluded)
    {
        sizeInBits += 16; // Bytes 6-7: Next Cluster Channel (13 bits + reserved)
    }

    if (m_timeToNextFieldPresent)
    {
        sizeInBits += 32; // Bytes 8-11: Time to Next (32 bits)
    }

    // Round up to full bytes
    uint32_t sizeInBytes = (sizeInBits + 7) / 8;
    return sizeInBytes;
}

void
Dect2020ExtClusterBeaconMessageIe::Serialize(Buffer::Iterator start) const
{
        // Byte 0
    uint8_t byte0 = 0;
    byte0 = m_SFN; // Bit 0-7

    start.WriteU8(byte0);

    // Byte 1
    uint8_t byte1 = 0;
    // Bit 0-2 Reserved
    byte1 |= (m_txPowerIncluded << 4);       // Bit 3
    byte1 |= (m_powerConstraints << 3);      // Bit 4
    byte1 |= (m_frameOffsetIncluded << 2);   // Bit 5
    byte1 |= (m_nextChannelIncluded << 1);   // Bit 6
    byte1 |= (m_timeToNextFieldPresent);     // Bit 7

    start.WriteU8(byte1);

    // Byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_networkBeaconPeriod & 0x0F) << 4; // Bit 0-3
    byte2 |= (m_clusterBeaconPeriod & 0x0F);      // Bit 4-7

    start.WriteU8(byte2);

    // Byte 3
    uint8_t byte3 = 0;
    byte3 |= (m_countToTrigger & 0x0F) << 4; // Bit 0-3
    byte3 |= (m_relQuality & 0x03) << 2;     // Bit 4-5
    byte3 |= (m_minQuality & 0x03);          // Bit 6-7

    start.WriteU8(byte3);

    if (m_txPowerIncluded)
    {
        // Byte 4
        uint8_t byte4 = 0;
        // Bit 0-3 Reserved
        byte4 |= (m_clusterMaxTxPower & 0x0F); // Bit 4-7

        start.WriteU8(byte4);
    }

    if (m_frameOffsetIncluded) // Note: Only the 8 bit variant is implemented. For subcarrier scaling factor >= 4,
              // Frame Offset is 16 Bit
    {
        // Byte 5
        uint8_t byte5 = 0;
        byte5 = m_frameOffset; // Bit 0-7

        start.WriteU8(byte5);
    }

    if (m_nextChannelIncluded)
    {
        // Byte 6
        uint8_t byte6 = 0;
        // Bit 0-2 Reserved
        byte6 |= (m_nextClusterChannel >> 8) & 0x1F; // Bit 3-7

        start.WriteU8(byte6);

        // Byte 7
        uint8_t byte7 = 0;
        byte7 |= (m_nextClusterChannel & 0xFF); // Bit 0-7

        start.WriteU8(byte7);
    }

    if (m_timeToNextFieldPresent)
    {
        start.WriteU32(m_timeToNext);
    }
}

uint32_t
Dect2020ExtClusterBeaconMessageIe::Deserialize(Buffer::Iterator start)
{
        uint32_t bytesRead = 0;

    // Byte 0
    uint8_t byte0 = start.ReadU8();
    bytesRead++;
    m_SFN = byte0;

    // Byte 1
    uint8_t byte1 = start.ReadU8();
    bytesRead++;
    m_txPowerIncluded = (byte1 >> 4) & 0x01;
    m_powerConstraints = (byte1 >> 3) & 0x01;
    m_frameOffsetIncluded = (byte1 >> 2) & 0x01;
    m_nextChannelIncluded = (byte1 >> 1) & 0x01;
    m_timeToNextFieldPresent = (byte1 >> 0) & 0x01;

    // Byte 2
    uint8_t byte2 = start.ReadU8();
    bytesRead++;
    m_networkBeaconPeriod = static_cast<Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod>((byte2 >> 4) & 0x0F);
    m_clusterBeaconPeriod = static_cast<Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod>(byte2 & 0x0F);

    // Byte 3
    uint8_t byte3 = start.ReadU8();
    bytesRead++;
    m_countToTrigger = (byte3 >> 4) & 0x0F;
    m_relQuality = (byte3 >> 2) & 0x03;
    m_minQuality = byte3 & 0x03;

    // Byte 4 (optional)
    if (m_txPowerIncluded)
    {
        uint8_t byte4 = start.ReadU8();
        bytesRead++;
        m_clusterMaxTxPower = byte4 & 0x0F;
    }

    // Byte 5 (optional)
    if (m_frameOffsetIncluded)
    {
        uint8_t byte5 = start.ReadU8();
        bytesRead++;
        m_frameOffset = byte5; // only 8 Bit variant implemented
    }

    // Bytes 6-7 (optional)
    if (m_nextChannelIncluded)
    {
        uint8_t byte6 = start.ReadU8();
        bytesRead++;
        uint8_t byte7 = start.ReadU8();
        bytesRead++;
        m_nextClusterChannel = ((byte6 & 0x1F) << 8) | byte7;
    }

    // Bytes 8-11 (optional)
    if (m_timeToNextFieldPresent)
    {
        m_timeToNext = start.ReadU32();
        bytesRead += 4;
    }

    return bytesRead;
}

void
Dect2020ExtClusterBeaconMessageIe::Print(std::ostream& os) const
{
    os << "[DECT2020 CLUSTER BEACON MESSAGE]" << "\n"
       << "SFN: 0x" << std::hex << (int)GetSFN() << "\n"
       << "Tx Power Included: 0x" << std::hex << (int)GetTxPowerIncluded() << "\n"
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

void
Dect2020ExtClusterBeaconMessageIe::SetSFN(uint8_t sfn)
{
    m_SFN = sfn;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetSFN() const
{
    return m_SFN;
}

void
Dect2020ExtClusterBeaconMessageIe::SetTxPowerIncluded(bool txPower)
{
    m_txPowerIncluded = txPower;
}

bool
Dect2020ExtClusterBeaconMessageIe::GetTxPowerIncluded() const
{
    return m_txPowerIncluded;
}

void
Dect2020ExtClusterBeaconMessageIe::SetPowerConstraints(bool powerConstraints)
{
    m_powerConstraints = powerConstraints;
}

bool
Dect2020ExtClusterBeaconMessageIe::GetPowerConstraints() const
{
    return m_powerConstraints;
}

void
Dect2020ExtClusterBeaconMessageIe::SetFrameOffsetIncluded(bool frameOffsetIncluded)
{
    m_frameOffsetIncluded = frameOffsetIncluded;
}

bool
Dect2020ExtClusterBeaconMessageIe::GetFrameOffsetIncluded() const
{
    return m_frameOffsetIncluded;
}

void
Dect2020ExtClusterBeaconMessageIe::SetNextChannelIncluded(bool nextChannelIncluded)
{
    m_nextChannelIncluded = nextChannelIncluded;
}

bool
Dect2020ExtClusterBeaconMessageIe::GetNextChannelIncluded() const
{
    return m_nextChannelIncluded;
}

void
Dect2020ExtClusterBeaconMessageIe::SetTimeToNextFieldPresent(bool timeToNextFieldPresent)
{
    m_timeToNextFieldPresent = timeToNextFieldPresent;
}

bool
Dect2020ExtClusterBeaconMessageIe::GetTimeToNextFieldPresent() const
{
    return m_timeToNextFieldPresent;
}

void
Dect2020ExtClusterBeaconMessageIe::SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod period)
{
    m_networkBeaconPeriod = period;
}

Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod
Dect2020ExtClusterBeaconMessageIe::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

void
Dect2020ExtClusterBeaconMessageIe::SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod period)
{
    m_clusterBeaconPeriod = period;
}

Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod
Dect2020ExtClusterBeaconMessageIe::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

void
Dect2020ExtClusterBeaconMessageIe::SetCountToTrigger(uint8_t count)
{
    m_countToTrigger = count;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetCountToTrigger() const
{
    return m_countToTrigger;
}

void
Dect2020ExtClusterBeaconMessageIe::SetRelQuality(uint8_t quality)
{
    m_relQuality = quality;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetRelQuality() const
{
    return m_relQuality;
}

void
Dect2020ExtClusterBeaconMessageIe::SetMinQuality(uint8_t quality)
{
    m_minQuality = quality;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetMinQuality() const
{
    return m_minQuality;
}

void
Dect2020ExtClusterBeaconMessageIe::SetClusterMaxTXPower(uint8_t power)
{
    m_clusterMaxTxPower = power;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetClusterMaxTXPower() const
{
    return m_clusterMaxTxPower;
}

void
Dect2020ExtClusterBeaconMessageIe::SetFrameOffset(uint8_t offset)
{
    m_frameOffset = offset;
}

uint8_t
Dect2020ExtClusterBeaconMessageIe::GetFrameOffset() const
{
    return m_frameOffset;
}

void
Dect2020ExtClusterBeaconMessageIe::SetNextClusterChannel(uint16_t channel)
{
    m_nextClusterChannel = channel;
}

uint16_t
Dect2020ExtClusterBeaconMessageIe::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

void
Dect2020ExtClusterBeaconMessageIe::SetTimeToNext(uint32_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ExtClusterBeaconMessageIe::GetTimeToNext() const
{
    return m_timeToNext;
}

} // namespace ns3 