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
    byte0 |= (m_powerConst & 0x01) << 1;
    byte0 |= (m_ftMode & 0x01);
    start.WriteU8(byte0);

    // byte 1
    uint8_t byte1 = 0;
    byte1 |= (m_current & 0x01) << 7;
    start.WriteU8(byte1);

    // byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_harqProcessesTX & 0x07) << 4;
    byte2 |= (m_maxHarqReTX & 0x1F);
    start.WriteU8(byte2);

    // byte 3
    uint8_t byte3 = 0;
    byte3 |= (m_harqProcessesRX & 0x07) << 4;
    byte3 |= (m_maxHarqReRX & 0x1F);
    start.WriteU8(byte3);

    // byte 4
    uint8_t byte4 = 0;
    byte4 |= (m_flowID & 0x3F);
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
    m_setupCause = static_cast<SetupCause>((byte0 >> 5) & 0x07);
    m_numberOfFlows = (byte0 >> 2) & 0x07;
    m_powerConst = (byte0 >> 1) & 0x01;
    m_ftMode = byte0 & 0x01;

    // byte 1
    uint8_t byte1 = start.ReadU8();
    bytesRead++;
    m_current = (byte1 >> 7) & 0x01;

    // byte 2
    uint8_t byte2 = start.ReadU8();
    bytesRead++;
    m_harqProcessesTX = (byte2 >> 4) & 0x07;
    m_maxHarqReTX = byte2 & 0x1F;

    // byte 3
    uint8_t byte3 = start.ReadU8();
    bytesRead++;
    m_harqProcessesRX = (byte3 >> 4) & 0x07;
    m_maxHarqReRX = byte3 & 0x1F;

    // byte 4
    uint8_t byte4 = start.ReadU8();
    bytesRead++;
    m_flowID = byte4 & 0x3F;

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
    os << "Setup Cause: " << m_setupCause << "\n";
    os << "Number of Flows: " << m_numberOfFlows << "\n";
    os << "Power Const: " << m_powerConst << "\n";
    os << "FT Mode: " << m_ftMode << "\n";
    os << "Current: " << m_current << "\n";
    os << "HARQ Processes TX: " << m_harqProcessesTX << "\n";
    os << "Max HARQ ReTX TX: " << m_maxHarqReTX << "\n";
    os << "HARQ Processes RX: " << m_harqProcessesRX << "\n";
    os << "Max HARQ ReTX RX: " << m_maxHarqReRX << "\n";
    os << "Flow ID: " << m_flowID << "\n";
    os << "Network Beacon Period: " << m_networkBeaconPeriod << "\n";
    os << "Cluster Beacon Period: " << m_clusterBeaconPeriod << "\n";
    os << "Next Cluster Channel: " << m_nextClusterChannel << "\n";
    os << "Time to Next: " << m_timeToNext << "\n";
    os << "Current Cluster Channel: " << m_currentClusterChannel << "\n";   
}

} // namespace ns3 