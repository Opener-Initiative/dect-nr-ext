#include "dect2020ext-association-request-message-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtAssociationRequestMessageIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationRequestMessageIE);

Dect2020ExtAssociationRequestMessageIE::Dect2020ExtAssociationRequestMessageIE()
{
}

Dect2020ExtAssociationRequestMessageIE::~Dect2020ExtAssociationRequestMessageIE()
{
}

TypeId
Dect2020ExtAssociationRequestMessageIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationRequestMessageIE")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtAssociationRequestMessageIE>();
    return tid;
}

TypeId
Dect2020ExtAssociationRequestMessageIE::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtAssociationRequestMessageIE::GetSerializedSize() const
{
    return 14; // Size in bytes
}

void
Dect2020ExtAssociationRequestMessageIE::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = 0;
    byte0 |= (m_setupCause & 0x07) << 5;
    byte0 |= (m_numberOfFlows & 0x07) << 2;
    byte0 |= (m_powerConstraints & 0x01) << 1;
    byte0 |= (m_ftMode & 0x01);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = 0;
    byte1 |= (m_current & 0x01) << 7;
    start.WriteU8(byte1);

    // byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_harqProcessesTx & 0x07) << 4;
    byte2 |= (m_maxHarqReTxDelay & 0x1F);
    start.WriteU8(byte2);

    // byte 3
    uint8_t byte3 = 0;
    byte3 |= (m_harqProcessesRx & 0x07) << 4;
    byte3 |= (m_maxHarqReRxDelay & 0x1F);
    start.WriteU8(byte3);

    // byte 4
    uint8_t byte4 = 0;
    byte4 |= (m_flowId & 0x3F);
    start.WriteU8(byte4);

    // byte 5
    uint8_t byte5 = 0;
    byte5 |= (m_networkBeaconPeriod & 0x0F) << 4;
    byte5 |= (m_clusterBeaconPeriod & 0x0F);
    start.WriteU8(byte5);

    // byte 6
    uint8_t byte6 = 0;
    byte6 |= (m_nextClusterChannel >> 8);
    start.WriteU8(byte6);

    // byte 7
    uint8_t byte7 = 0;
    byte7 |= (m_nextClusterChannel & 0xFF);
    start.WriteU8(byte7);

    // bytes 8-11
    // Time to Next
    start.WriteU32(m_timeToNext);

    // byte 12
    uint8_t byte13 = 0;
    byte13 |= (m_currentClusterChannel >> 8);
    start.WriteU8(byte13);

    // byte 13
    uint8_t byte14 = 0;
    byte14 |= (m_currentClusterChannel & 0xFF);
    start.WriteU8(byte14);
}

uint32_t
Dect2020ExtAssociationRequestMessageIE::Deserialize(Buffer::Iterator start)
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
    m_harqProcessesTx = (byte2 >> 4) & 0x07;
    m_maxHarqReTxDelay = byte2 & 0x1F;

    // byte 3
    uint8_t byte3 = start.ReadU8();
    bytesRead++;
    m_harqProcessesRx = (byte3 >> 4) & 0x07;
    m_maxHarqReRxDelay = byte3 & 0x1F;

    // byte 4
    uint8_t byte4 = start.ReadU8();
    bytesRead++;
    m_flowId = byte4 & 0x3F;

    // byte 5
    uint8_t byte5 = start.ReadU8();
    bytesRead++;
    m_networkBeaconPeriod = static_cast<NetworkBeaconPeriod>((byte5 >> 4) & 0x0F);
    m_clusterBeaconPeriod = static_cast<ClusterBeaconPeriod>(byte5 & 0x0F);

    // bytes 6-7: nextClusterChannel (16 bits)
    uint8_t byte6 = start.ReadU8();
    bytesRead++;
    uint8_t byte7 = start.ReadU8();
    bytesRead++;
    m_nextClusterChannel = (byte6 << 8) | byte7;

    // bytes 8-11: timeToNext (32 bits)
    m_timeToNext = start.ReadU32();
    bytesRead += 4;

    // bytes 12-13: currentClusterChannel (16 bits)
    uint8_t byte12 = start.ReadU8();
    bytesRead++;
    uint8_t byte13 = start.ReadU8();
    bytesRead++;
    m_currentClusterChannel = (byte12 << 8) | byte13;

    return bytesRead;
}

void
Dect2020ExtAssociationRequestMessageIE::Print(std::ostream& os) const
{
    os << "Dect2020ExtAssociationRequestMessageIE" << "\n";
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
Dect2020ExtAssociationRequestMessageIE::SetSetupCause(uint8_t setupCause)
{
    m_setupCause = setupCause;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetSetupCause() const
{
    return m_setupCause;
}

void
Dect2020ExtAssociationRequestMessageIE::SetNumberOfFlows(uint8_t numberOfFlows)
{
    m_numberOfFlows = numberOfFlows;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetNumberOfFlows() const
{
    return m_numberOfFlows;
}

void
Dect2020ExtAssociationRequestMessageIE::SetPowerConstraints(bool powerConstraints)
{
    m_powerConstraints = powerConstraints;
}

bool
Dect2020ExtAssociationRequestMessageIE::GetPowerConstraints() const
{
    return m_powerConstraints;
}

void
Dect2020ExtAssociationRequestMessageIE::SetFtMode(bool ftMode)
{
    m_ftMode = ftMode;
}

bool
Dect2020ExtAssociationRequestMessageIE::GetFtMode() const
{
    return m_ftMode;
}

void
Dect2020ExtAssociationRequestMessageIE::SetCurrent(bool current)
{
    m_current = current;
}

bool
Dect2020ExtAssociationRequestMessageIE::GetCurrent() const
{
    return m_current;
}

void
Dect2020ExtAssociationRequestMessageIE::SetHarqProcessesTx(uint8_t harqProcessesTx)
{
    m_harqProcessesTx = harqProcessesTx;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetHarqProcessesTx() const
{
    return m_harqProcessesTx;
}

void
Dect2020ExtAssociationRequestMessageIE::SetMaxHarqReTxDelay(uint8_t maxHarqReTxDelay)
{
    m_maxHarqReTxDelay = maxHarqReTxDelay;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetMaxHarqReTxDelay() const
{
    return m_maxHarqReTxDelay;
}

void
Dect2020ExtAssociationRequestMessageIE::SetHarqProcessesRx(uint8_t harqProcessesRx)
{
    m_harqProcessesRx = harqProcessesRx;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetHarqProcessesRx() const
{
    return m_harqProcessesRx;
}

void
Dect2020ExtAssociationRequestMessageIE::SetMaxHarqReRxDelay(uint8_t maxHarqReRxDelay)
{
    m_maxHarqReRxDelay = maxHarqReRxDelay;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetMaxHarqReRxDelay() const
{
    return m_maxHarqReRxDelay;
}

void
Dect2020ExtAssociationRequestMessageIE::SetFlowId(uint8_t flowId)
{
    m_flowId = flowId;
}

uint8_t
Dect2020ExtAssociationRequestMessageIE::GetFlowId() const
{
    return m_flowId;
}

void
Dect2020ExtAssociationRequestMessageIE::SetNetworkBeaconPeriod(NetworkBeaconPeriod networkBeaconPeriod)
{
    m_networkBeaconPeriod = networkBeaconPeriod;
}

NetworkBeaconPeriod
Dect2020ExtAssociationRequestMessageIE::GetNetworkBeaconPeriod() const
{
    return m_networkBeaconPeriod;
}

void
Dect2020ExtAssociationRequestMessageIE::SetClusterBeaconPeriod(ClusterBeaconPeriod clusterBeaconPeriod)
{
    m_clusterBeaconPeriod = clusterBeaconPeriod;
}

ClusterBeaconPeriod
Dect2020ExtAssociationRequestMessageIE::GetClusterBeaconPeriod() const
{
    return m_clusterBeaconPeriod;
}

void
Dect2020ExtAssociationRequestMessageIE::SetNextClusterChannel(uint16_t nextClusterChannel)
{
    m_nextClusterChannel = nextClusterChannel;
}

uint16_t
Dect2020ExtAssociationRequestMessageIE::GetNextClusterChannel() const
{
    return m_nextClusterChannel;
}

void
Dect2020ExtAssociationRequestMessageIE::SetTimeToNext(uint32_t timeToNext)
{
    m_timeToNext = timeToNext;
}

uint32_t
Dect2020ExtAssociationRequestMessageIE::GetTimeToNext() const
{
    return m_timeToNext;
}

void
Dect2020ExtAssociationRequestMessageIE::SetCurrentClusterChannel(uint16_t currentClusterChannel)
{
    m_currentClusterChannel = currentClusterChannel;
}

uint16_t
Dect2020ExtAssociationRequestMessageIE::GetCurrentClusterChannel() const
{
    return m_currentClusterChannel;
}

} // namespace ns3 