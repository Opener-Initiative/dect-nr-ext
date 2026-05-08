#include "dect2020ext-association-request-message-ie.h"

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationRequestMessageIe);

Dect2020ExtAssociationRequestMessageIe::Dect2020ExtAssociationRequestMessageIe()
{
}

Dect2020ExtAssociationRequestMessageIe::~Dect2020ExtAssociationRequestMessageIe()
{
}

TypeId
Dect2020ExtAssociationRequestMessageIe::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationRequestMessageIe")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtAssociationRequestMessageIe>();
    return tid;
}

TypeId
Dect2020ExtAssociationRequestMessageIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtAssociationRequestMessageIe::GetSerializedSize() const
{
    uint32_t sizeInBits = 0;

    sizeInBits += 8; // Byte 0: Setup cause, Number of Flows, Power Const, FT mode
    sizeInBits += 8; // Byte 1: Current + reserved
    sizeInBits += 8; // Byte 2: HARQ Processes TX, MAX HARQ Re-TX
    sizeInBits += 8; // Byte 3: HARQ Processes RX, MAX HARQ Re-RX
    sizeInBits += 8; // Byte 4: Reserved + Flow ID

    if (m_ftMode)
    {
        sizeInBits += 8;  // Byte 5: Network/Cluster Beacon Period
        sizeInBits += 16; // Bytes 6-7: Next Cluster Channel (13 bits + 3 reserved)
        sizeInBits += 32; // Bytes 8-11: Time to Next
    }

    if (m_current)
    {
        sizeInBits += 16; // Bytes 12-13: Current Cluster Channel (13 bits + 3 reserved)
    }

    // Round up to full bytes
    uint32_t sizeInBytes = (sizeInBits + 7) / 8;
    return sizeInBytes;
}

void
Dect2020ExtAssociationRequestMessageIe::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = 0;
    byte0 |= (m_setupCause & 0x07) << 5;       // Bit 0-2
    byte0 |= (m_numberOfFlows & 0x07) << 2;    // Bit 3-5
    byte0 |= (m_powerConstraints & 0x01) << 1; // Bit 6
    byte0 |= (m_ftMode & 0x01);                // Bit 7
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = 0;
    byte1 |= (m_current & 0x01) << 7; // Bit 0
    // Bit 1-7 Reserved
    start.WriteU8(byte1);

    // byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_harqProcessesTx & 0x07) << 5; // Bit 0-2
    byte2 |= (m_maxHarqReTxDelay & 0x1F);     // Bit 3-7
    start.WriteU8(byte2);

    // byte 3
    uint8_t byte3 = 0;
    byte3 |= (m_harqProcessesRx & 0x07) << 5; // Bit 0-2
    byte3 |= (m_maxHarqReRxDelay & 0x1F);     // Bit 3-7
    start.WriteU8(byte3);

    // byte 4
    uint8_t byte4 = 0;
    // Bit 0-1 Reserved
    byte4 |= (m_flowId & 0x3F); // Bit 2-7
    start.WriteU8(byte4);

    if (m_ftMode)
    {
        // byte 5
        uint8_t byte5 = 0;
        byte5 |= (m_networkBeaconPeriod & 0x0F) << 4; // Bit 0-3
        byte5 |= (m_clusterBeaconPeriod & 0x0F);      // Bit 4-7
        start.WriteU8(byte5);

        // byte 6
        uint8_t byte6 = 0;
        // Bit 0-2 Reserved
        byte6 |= (m_nextClusterChannel >> 8) & 0x1F; // Bit 3-7
        start.WriteU8(byte6);

        // byte 7
        uint8_t byte7 = 0;
        byte7 |= (m_nextClusterChannel & 0xFF); // Bit 0-7
        start.WriteU8(byte7);

        // bytes 8-11: Time to Next
        start.WriteU32(m_timeToNext);
    }

    if (m_current)
    {
        // byte 12
        uint8_t byte12 = 0;
        // Bit 0-2 Reserved
        byte12 |= (m_currentClusterChannel >> 8) & 0x1F; // Bit 3-7
        start.WriteU8(byte12);

        // byte 13
        uint8_t byte13 = 0;
        byte13 |= (m_currentClusterChannel & 0xFF); // Bit 0-7
        start.WriteU8(byte13);
    }
}

uint32_t
Dect2020ExtAssociationRequestMessageIe::Deserialize(Buffer::Iterator start)
{
    uint32_t bytesRead = 0;

    // byte 0
    uint8_t byte0 = start.ReadU8();
    bytesRead++;
    m_setupCause = (byte0 >> 5) & 0x07;
    m_numberOfFlows = (byte0 >> 2) & 0x07;
    m_powerConstraints = (byte0 >> 1) & 0x01;
    m_ftMode = byte0 & 0x01;

    // byte 1
    uint8_t byte1 = start.ReadU8();
    bytesRead++;
    m_current = (byte1 >> 7) & 0x01;

    // byte 2
    uint8_t byte2 = start.ReadU8();
    bytesRead++;
    m_harqProcessesTx = (byte2 >> 5) & 0x07;
    m_maxHarqReTxDelay = byte2 & 0x1F;

    // byte 3
    uint8_t byte3 = start.ReadU8();
    bytesRead++;
    m_harqProcessesRx = (byte3 >> 5) & 0x07;
    m_maxHarqReRxDelay = byte3 & 0x1F;

    // byte 4
    uint8_t byte4 = start.ReadU8();
    bytesRead++;
    m_flowId = byte4 & 0x3F;

    if (m_ftMode)
    {
        // byte 5
        uint8_t byte5 = start.ReadU8();
        bytesRead++;
        m_networkBeaconPeriod = static_cast<NetworkBeaconPeriod>((byte5 >> 4) & 0x0F);
        m_clusterBeaconPeriod = static_cast<ClusterBeaconPeriod>(byte5 & 0x0F);

        // bytes 6-7: Next Cluster Channel (13 bits + 3 reserved)
        uint8_t byte6 = start.ReadU8();
        bytesRead++;
        uint8_t byte7 = start.ReadU8();
        bytesRead++;
        m_nextClusterChannel = ((byte6 & 0x1F) << 8) | byte7;

        // bytes 8-11: Time to Next (32 bits)
        m_timeToNext = start.ReadU32();
        bytesRead += 4;
    }

    if (m_current)
    {
        // bytes 12-13: Current Cluster Channel (13 bits + 3 reserved)
        uint8_t byte12 = start.ReadU8();
        bytesRead++;
        uint8_t byte13 = start.ReadU8();
        bytesRead++;
        m_currentClusterChannel = ((byte12 & 0x1F) << 8) | byte13;
    }

    return bytesRead;
}

void
Dect2020ExtAssociationRequestMessageIe::Print(std::ostream& os) const
{
    os << "Dect2020ExtAssociationRequestMessageIe" << "\n";
    os << "Setup Cause: " << static_cast<uint32_t>(m_setupCause) << "\n";
    os << "Number of Flows: " << static_cast<uint32_t>(m_numberOfFlows) << "\n";
    os << "Power Constraints: " << m_powerConstraints << "\n";
    os << "FT Mode: " << m_ftMode << "\n";
    os << "Current: " << m_current << "\n";
    os << "HARQ Processes TX: " << static_cast<uint32_t>(m_harqProcessesTx) << "\n";
    os << "Max HARQ ReTX Delay: " << static_cast<uint32_t>(m_maxHarqReTxDelay) << "\n";
    os << "HARQ Processes RX: " << static_cast<uint32_t>(m_harqProcessesRx) << "\n";
    os << "Max HARQ ReRX Delay: " << static_cast<uint32_t>(m_maxHarqReRxDelay) << "\n";
    os << "Flow ID: " << static_cast<uint32_t>(m_flowId) << "\n";
    os << "Network Beacon Period: " << static_cast<uint32_t>(m_networkBeaconPeriod) << "\n";
    os << "Cluster Beacon Period: " << static_cast<uint32_t>(m_clusterBeaconPeriod) << "\n";
    os << "Next Cluster Channel: " << m_nextClusterChannel << "\n";
    os << "Time to Next: " << m_timeToNext << "\n";
    os << "Current Cluster Channel: " << m_currentClusterChannel << "\n";   
}

// Setters and Getters

void
Dect2020ExtAssociationRequestMessageIe::SetSetupCause(uint8_t setupCause)
{
    m_setupCause = setupCause;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetSetupCause() const
{
    return m_setupCause;
}

void
Dect2020ExtAssociationRequestMessageIe::SetNumberOfFlows(uint8_t numberOfFlows)
{
    m_numberOfFlows = numberOfFlows;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetNumberOfFlows() const
{
    return m_numberOfFlows;
}

void
Dect2020ExtAssociationRequestMessageIe::SetPowerConstraints(bool powerConstraints)
{
    m_powerConstraints = powerConstraints;
}

bool
Dect2020ExtAssociationRequestMessageIe::GetPowerConstraints() const
{
    return m_powerConstraints;
}

void
Dect2020ExtAssociationRequestMessageIe::SetFtMode(bool ftMode)
{
    m_ftMode = ftMode;
}

bool
Dect2020ExtAssociationRequestMessageIe::GetFtMode() const
{
    return m_ftMode;
}

void
Dect2020ExtAssociationRequestMessageIe::SetCurrent(bool current)
{
    m_current = current;
}

bool
Dect2020ExtAssociationRequestMessageIe::GetCurrent() const
{
    return m_current;
}

void
Dect2020ExtAssociationRequestMessageIe::SetHarqProcessesTx(uint8_t harqProcessesTx)
{
    m_harqProcessesTx = harqProcessesTx;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetHarqProcessesTx() const
{
    return m_harqProcessesTx;
}

void
Dect2020ExtAssociationRequestMessageIe::SetMaxHarqReTxDelay(uint8_t maxHarqReTxDelay)
{
    m_maxHarqReTxDelay = maxHarqReTxDelay;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetMaxHarqReTxDelay() const
{
    return m_maxHarqReTxDelay;
}

void
Dect2020ExtAssociationRequestMessageIe::SetHarqProcessesRx(uint8_t harqProcessesRx)
{
    m_harqProcessesRx = harqProcessesRx;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetHarqProcessesRx() const
{
    return m_harqProcessesRx;
}

void
Dect2020ExtAssociationRequestMessageIe::SetMaxHarqReRxDelay(uint8_t maxHarqReRxDelay)
{
    m_maxHarqReRxDelay = maxHarqReRxDelay;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetMaxHarqReRxDelay() const
{
    return m_maxHarqReRxDelay;
}

void
Dect2020ExtAssociationRequestMessageIe::SetFlowId(uint8_t flowId)
{
    m_flowId = flowId;
}

uint8_t
Dect2020ExtAssociationRequestMessageIe::GetFlowId() const
{
    return m_flowId;
}

void
Dect2020ExtAssociationRequestMessageIe::SetNetworkBeaconPeriod(NetworkBeaconPeriod networkBeaconPeriod)
{
    m_networkBeaconPeriod = networkBeaconPeriod;
}

NetworkBeaconPeriod
Dect2020ExtAssociationRequestMessageIe::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

void
Dect2020ExtAssociationRequestMessageIe::SetClusterBeaconPeriod(ClusterBeaconPeriod clusterBeaconPeriod)
{
    m_clusterBeaconPeriod = clusterBeaconPeriod;
}

ClusterBeaconPeriod
Dect2020ExtAssociationRequestMessageIe::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

void
Dect2020ExtAssociationRequestMessageIe::SetNextClusterChannel(uint16_t nextClusterChannel)
{
    m_nextClusterChannel = nextClusterChannel;
}

uint16_t
Dect2020ExtAssociationRequestMessageIe::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

void
Dect2020ExtAssociationRequestMessageIe::SetTimeToNext(uint32_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ExtAssociationRequestMessageIe::GetTimeToNext() const
{
    return m_timeToNext;
}

void
Dect2020ExtAssociationRequestMessageIe::SetCurrentClusterChannel(uint16_t currentClusterChannel)
{
    m_currentClusterChannel = currentClusterChannel;
}

uint16_t
Dect2020ExtAssociationRequestMessageIe::GetCurrentClusterChannel() const
{
    return m_currentClusterChannel;
}

} // namespace ns3 