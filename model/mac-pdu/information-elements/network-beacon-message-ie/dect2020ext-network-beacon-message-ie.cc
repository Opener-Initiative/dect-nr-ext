#include "dect2020ext-network-beacon-message-ie.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

#include <algorithm>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtNetworkBeaconMessageIe");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtNetworkBeaconMessageIe);

Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconMessageIe()
    : m_txPowerIncluded(false),
      m_powerConstraints(false),
      m_currentClusterChannelIncluded(false),
      m_networkBeaconChannels(0),
      m_networkBeaconPeriod(NETWORK_BEACON_PERIOD_100MS),
      m_clusterBeaconPeriod(CLUSTER_BEACON_PERIOD_100MS),
      m_nextClusterChannel(0),
      m_timeToNext(0),
      m_clusterMaxTxPower(0),
      m_currentClusterChannel(0)
{
}

Dect2020ExtNetworkBeaconMessageIe::~Dect2020ExtNetworkBeaconMessageIe()
{
}

TypeId
Dect2020ExtNetworkBeaconMessageIe::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtNetworkBeaconMessageIE")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtNetworkBeaconMessageIe>();
    return tid;
}

TypeId
Dect2020ExtNetworkBeaconMessageIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtNetworkBeaconMessageIe::Print(std::ostream& os) const
{
    os << "Dect2020ExtNetworkBeaconMessageIE" << std::endl;
    os << "Tx Power Included: " << m_txPowerIncluded << std::endl;
    os << "Power Constraints: " << m_powerConstraints << std::endl;
    os << "Current Cluster Channel Included: " << m_currentClusterChannelIncluded << std::endl;
    os << "Network Beacon Channels: " << m_networkBeaconChannels << std::endl;
    os << "Network Beacon Period: " << m_networkBeaconPeriod << std::endl;
    os << "Cluster Beacon Period: " << m_clusterBeaconPeriod << std::endl;
    os << "Next Cluster Channel: " << m_nextClusterChannel << std::endl;
    os << "Time To Next: " << m_timeToNext << std::endl;
    os << "Clusters Max Tx Power: " << m_clusterMaxTxPower << std::endl;
    os << "Current Cluster Channel: " << m_currentClusterChannel << std::endl;
    os << "Additional Network Beacon Channels: ";
    os << std::endl;
}

uint32_t
Dect2020ExtNetworkBeaconMessageIe::GetSerializedSize() const
{
    uint32_t sizeInBits = 0;

    sizeInBits += 64; // Byte 0 - Byte 7

    if (m_txPowerIncluded == 1)
    {
        sizeInBits += 4; // Reserved
        sizeInBits += 4; // Clusters Max TX Power Field
    }

    if (m_currentClusterChannelIncluded == 1)
    {
        sizeInBits += 3;  // Reserved
        sizeInBits += 13; // Current Cluster Channel Field
    }

    if (m_networkBeaconChannels > 0)
    {
        sizeInBits += m_networkBeaconChannels * 3;  // Reserved
        sizeInBits += m_networkBeaconChannels * 13; // Additional Network Beacon Channels
    }

    // Round up to the next multiple of 8.
    uint32_t sizeInBytes = (sizeInBits + 7) / 8;

    return sizeInBytes;
}

void
Dect2020ExtNetworkBeaconMessageIe::Serialize(Buffer::Iterator start) const
{
    // Byte 0
    uint8_t byte0 = 0;
    // Bit 0-2 Reserved
    byte0 |= (m_txPowerIncluded << 4);               // Bit 3
    byte0 |= (m_powerConstraints << 3);              // Bit 4
    byte0 |= (m_currentClusterChannelIncluded << 2); // Bit 5
    byte0 |= (m_networkBeaconChannels & 0x03);       // Bit 6-7

    start.WriteU8(byte0);

    // Byte 1
    uint8_t byte1 = 0;
    byte1 |= (m_networkBeaconPeriod & 0x0F) << 4; // Bit 0-3
    byte1 |= (m_clusterBeaconPeriod & 0x0F);      // Bit 4-7

    start.WriteU8(byte1);

    // Byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_nextClusterChannel >> 8) & 0x1F; // Bit 3-7

    start.WriteU8(byte2);

    // Byte 3
    uint8_t byte3 = 0;
    byte3 = m_nextClusterChannel & 0xFF; // Bit 0-7

    start.WriteU8(byte3);

    // Byte 4-7
    start.WriteU32(m_timeToNext);

    if (m_txPowerIncluded)
    {
        // Byte 8
        uint8_t byte8 = 0;
        // Bit 0-3 Reserved
        byte8 |= (m_clusterMaxTxPower & 0x0F); // Bit 4-7

        start.WriteU8(byte8);
    }

    if (m_currentClusterChannelIncluded)
    {
        // Byte 9
        uint8_t byte9 = 0;
        // Bit 0-2 Reserved
        byte9 |= (m_currentClusterChannel >> 8) & 0x1F; // Bit 3-7

        start.WriteU8(byte9);

        // Byte 10
        uint8_t byte10 = 0;
        byte10 |= (m_currentClusterChannel & 0xFF); // Bit 0-7

        start.WriteU8(byte10);
    }

    if (m_networkBeaconChannels > 0)
    {
        for (int idx = 0; idx < m_networkBeaconChannels; idx++)
        {
            start.WriteU16(m_additionalNetworkBeaconChannels[idx] & 0x1FFF);
        }
    }

}

uint32_t
Dect2020ExtNetworkBeaconMessageIe::Deserialize(Buffer::Iterator start)
{
    // See ETSI Doc. 103 636-4 Figure 6.4.2.2-1 for more Information

    uint32_t bytesRead = 0;

    // Byte 0
    uint8_t byte0 = 0;

    byte0 = start.ReadU8();
    bytesRead++;

    // Bits 0-2: Reserved
    m_txPowerIncluded = (byte0 & (1 << 4)) != 0;               // Bit 3
    m_powerConstraints = (byte0 & (1 << 3)) != 0;              // Bit 4
    m_currentClusterChannelIncluded = (byte0 & (1 << 2)) != 0; // Bit 5
    m_networkBeaconChannels = byte0 & 0x03;                    // Bit 5-6

    // Byte 1
    uint8_t byte1 = 0;
    byte1 = start.ReadU8();
    bytesRead++;

    m_networkBeaconPeriod = static_cast<Dect2020ExtNetworkBeaconPeriod>((byte1 >> 4) & 0x0F); // Bit 0-3
    m_clusterBeaconPeriod = static_cast<Dect2020ExtClusterBeaconPeriod>(byte1 & 0x0F);        // Bit 4-7

    // Byte 2
    uint8_t byte2 = 0;
    byte2 = start.ReadU8();
    bytesRead++;

    // Bit 0-2: Reserved
    // Bit 3-7: Next Cluster Channel
    uint16_t nextClusterChannelHigh = (byte2 & 0x1F) << 8;

    // Byte 3
    uint8_t byte3 = 0;
    byte3 = start.ReadU8();
    bytesRead++;

    uint16_t nextClusterChannelLow = byte3;
    m_nextClusterChannel = nextClusterChannelHigh | nextClusterChannelLow;

    // Byte 4-7: Time To next
    m_timeToNext = start.ReadU32();
    bytesRead += 4;

    // Optional Fields
    if (m_txPowerIncluded)
    {
        // Byte 8
        uint8_t byte8 = 0;
        byte8 = start.ReadU8();
        bytesRead++;

        // Bit 0-3: Reserved
        m_clusterMaxTxPower = byte8 & 0x0F;
    }

    if (m_currentClusterChannelIncluded)
    {
        // Byte 9
        uint8_t byte9 = 0;
        byte9 = start.ReadU8();
        bytesRead++;

        uint16_t currentClusterChannelHigh = (byte9 & 0x1F) << 8;

        // Byte 10
        uint8_t byte10 = 0;
        byte10 = start.ReadU8();
        bytesRead++;

        m_currentClusterChannel = currentClusterChannelHigh | byte10;
    }

    if (m_networkBeaconChannels > 0)
    {
        for (int idx = 0; idx < m_networkBeaconChannels; idx++)
        {
            m_additionalNetworkBeaconChannels[idx] = start.ReadU16() & 0x1FFF;
            bytesRead += 2;
        }
    }

    return bytesRead;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetTxPowerIncluded(bool txPowerIncluded)
{
    m_txPowerIncluded = txPowerIncluded;
}

bool
Dect2020ExtNetworkBeaconMessageIe::GetTxPowerIncluded() const
{
    return m_txPowerIncluded;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetPowerConstraints(bool powerConstraints)
{
    m_powerConstraints = powerConstraints;
}

bool
Dect2020ExtNetworkBeaconMessageIe::GetPowerConstraints() const
{
    return m_powerConstraints;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetCurrentClusterChannelIncluded(bool currentClusterChannelIncluded)
{
    m_currentClusterChannelIncluded = currentClusterChannelIncluded;
}

bool
Dect2020ExtNetworkBeaconMessageIe::GetCurrentClusterChannelIncluded() const
{
    return m_currentClusterChannelIncluded;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetNetworkBeaconChannels(uint8_t networkBeaconChannels)
{
    if ((networkBeaconChannels < 0) || (networkBeaconChannels > 3))
    {
        DECT2020EXT_MAC_PDU_FATAL("Network Beacon Channels must be between 0 and 3", networkBeaconChannels);
    }

    m_networkBeaconChannels = networkBeaconChannels;
}

uint8_t
Dect2020ExtNetworkBeaconMessageIe::GetNetworkBeaconChannels() const
{
    return m_networkBeaconChannels;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconPeriod networkBeaconPeriod)
{
    // Note: validating here against the biggest Network Beacon period since the rest of the values
    // are reserved by the standard
    if (networkBeaconPeriod > NETWORK_BEACON_PERIOD_4000MS)
    {
        DECT2020EXT_MAC_PDU_FATAL("Network Beacon period must be between 0 and 6", networkBeaconPeriod);
    }

    m_networkBeaconPeriod = networkBeaconPeriod;
}

Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod
Dect2020ExtNetworkBeaconMessageIe::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetClusterBeaconPeriod(Dect2020ExtClusterBeaconPeriod clusterBeaconPeriod)
{
    // Note: validating here against the biggest Cluster Beacon period since the rest of the values
    // are reserved by the standard
    if (clusterBeaconPeriod > 10)
    {
        DECT2020EXT_MAC_PDU_FATAL("Cluster Beacon period must be between 0 and 10", clusterBeaconPeriod);
    }

    m_clusterBeaconPeriod = clusterBeaconPeriod;
}

Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod
Dect2020ExtNetworkBeaconMessageIe::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetNextClusterChannel(uint16_t nextClusterChannel)
{
    if ((nextClusterChannel < 0) || (nextClusterChannel > 8191))
    {
        DECT2020EXT_MAC_PDU_FATAL("Next Cluster Channel must be between 0 and 8191", nextClusterChannel);
    }

    m_nextClusterChannel = nextClusterChannel;
}

uint16_t
Dect2020ExtNetworkBeaconMessageIe::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetTimeToNext(uint32_t timeToNext)
{
    if ((timeToNext < 0) || (timeToNext > UINT32_MAX))
    {
        DECT2020EXT_MAC_PDU_FATAL("Time to Next must be between 0 and 32 bits", timeToNext);
    }

    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ExtNetworkBeaconMessageIe::GetTimeToNext() const
{
    return m_timeToNext;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetClustersMaxTxPower(uint8_t clusterMaxTxPower)
{
    if (clusterMaxTxPower > 15)
    {
        DECT2020EXT_MAC_PDU_FATAL("Cluster Max Tx Power must be between 0 and 15", clusterMaxTxPower);
    }
    m_clusterMaxTxPower = clusterMaxTxPower;
}

uint8_t
Dect2020ExtNetworkBeaconMessageIe::GetClustersMaxTxPower() const
{
    return m_clusterMaxTxPower;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetCurrentClusterChannel(uint16_t currentClusterChannel)
{
    if (currentClusterChannel > 8191)
    {
        DECT2020EXT_MAC_PDU_FATAL("Current Cluster Channel must be between 0 and 8191", currentClusterChannel);
    }
    m_currentClusterChannel = currentClusterChannel;
}

uint16_t
Dect2020ExtNetworkBeaconMessageIe::GetCurrentClusterChannel() const
{
    return m_currentClusterChannel;
}

void
Dect2020ExtNetworkBeaconMessageIe::SetAdditionalNetworkBeaconChannels(uint16_t additionalNetworkBeaconChannels[3])
{
    std::copy(additionalNetworkBeaconChannels,
              additionalNetworkBeaconChannels + 3,
              m_additionalNetworkBeaconChannels);
}

uint16_t*
Dect2020ExtNetworkBeaconMessageIe::GetAdditionalNetworkBeaconChannels()
{
    return m_additionalNetworkBeaconChannels;
}

} // namespace ns3