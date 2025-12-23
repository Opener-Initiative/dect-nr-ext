#include "dect2020ext-rd-capability-ie.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRdCapabilityIE");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtRdCapabilityIE);

Dect2020ExtRdCapabilityIE::Dect2020ExtRdCapabilityIE()
{
    m_numOfPhyCapabilities = 0;
}

Dect2020ExtRdCapabilityIE::~Dect2020ExtRdCapabilityIE()
{
}

TypeId
Dect2020ExtRdCapabilityIE::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtRdCapabilityIE")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtRdCapabilityIE>();
    return tid;
}

TypeId
Dect2020ExtRdCapabilityIE::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtRdCapabilityIE::GetSerializedSize() const
{
    return 7;
}

void
Dect2020ExtRdCapabilityIE::Print(std::ostream& os) const
{
    os << "RD Capability IE:"
       << " Number of PHY capabilities=" << static_cast<uint32_t>(m_numOfPhyCapabilities) << std::endl 
       << ", Release=" << static_cast<uint32_t>(m_Release)<< std::endl 
       << ", GroupAssign=" << m_groupAssignment<< std::endl 
       << ", Paging=" << m_paging<< std::endl 
       << ", OperatingModes=" << static_cast<uint32_t>(m_operatingModes)<< std::endl 
       << ", Mesh=" << m_mesh<< std::endl 
       << ", ScheduledAccessDataTransfer=" << m_scheduledAccessDataTransfer<< std::endl 
       << ", MACSec=" << static_cast<uint32_t>(m_macSecurity)<< std::endl 
       << ", DLC=" << static_cast<uint32_t>(m_dlcServiceType)<< std::endl 
       << ", PowerClass=" << static_cast<uint32_t>(m_rdPowerClass)<< std::endl 
       << ", NSSFoRx=" << static_cast<uint32_t>(m_maxNssFoRx)<< std::endl 
       << ", RxDiv=" << static_cast<uint32_t>(m_rxForTxDiversity)<< std::endl 
       << ", RxGain=" << static_cast<uint32_t>(m_rxGain)<< std::endl 
       << ", MaxMCS=" << static_cast<uint32_t>(m_maxMcs)<< std::endl 
       << ", Soft-Buffer=" << static_cast<uint32_t>(m_softBufferSize)<< std::endl 
       << ", HARQProc=" << static_cast<uint32_t>(m_numOfHarqProcesses)<< std::endl 
       << ", HARQDelay=" << static_cast<uint32_t>(m_harqFeedbackDelay)<< std::endl 
       << ", DDelay=" << m_dDelay<< std::endl 
       << ", HalfDULP=" << m_halfDulp << std::endl;
}


void
Dect2020ExtRdCapabilityIE::Serialize(Buffer::Iterator start) const
{
    // Byte 0
    uint8_t byte0 = 0;
    byte0 |= (m_numOfPhyCapabilities & 0x07) << 5; // Bits 0-2
    byte0 |= (m_Release & 0x1F) << 0;              // Bits 3-7

    start.WriteU8(byte0);

    // Byte 1
    uint8_t byte1 = 0;
    // Bit 0-1 Reserved
    byte1 |= (m_groupAssignment ? 1 : 0) << 5;             // Bit 2
    byte1 |= (m_paging ? 1 : 0) << 4;                      // Bit 3
    byte1 |= (m_operatingModes & 0x03) << 2;               // Bit 4-5
    byte1 |= (m_mesh ? 1 : 0) << 1;                        // Bit 6
    byte1 |= (m_scheduledAccessDataTransfer ? 1 : 0) << 0; // Bit 7

    start.WriteU8(byte1);

    // Byte 2
    uint8_t byte2 = 0;
    byte2 |= (m_macSecurity & 0x07) << 5;    // Bit 0-2
    byte2 |= (m_dlcServiceType & 0x07) << 2; // Bit 3-5
    // Bit 6-7 Reserved

    start.WriteU8(byte2);

    // Byte 3
    uint8_t byte3 = 0;
    // Bit 0 Reserved
    byte3 |= (m_rdPowerClass & 0x07) << 4;     // Bit 1-3
    byte3 |= (m_maxNssFoRx & 0x03) << 2;       // Bit 4-5
    byte3 |= (m_rxForTxDiversity & 0x03) << 0; // Bit 6-7

    start.WriteU8(byte3);

    // Byte 4
    uint8_t byte4 = 0;
    byte4 |= (m_rxGain & 0x0F) << 4; // Bit 0-3
    byte4 |= (m_maxMcs & 0x0F) << 0; // Bit 4-7

    start.WriteU8(byte4);

    // Byte 5
    uint8_t byte5 = 0;
    byte5 |= (m_softBufferSize & 0x0F) << 4;     // Bit 0-3
    byte5 |= (m_numOfHarqProcesses & 0x03) << 2; // Bit 4-5
    // Bit 6-7 Reserved

    start.WriteU8(byte5);

    // Byte 6
    uint8_t byte6 = 0;
    byte6 |= (m_harqFeedbackDelay & 0x0F) << 4; // Bit 0-3
    byte6 |= (m_dDelay ? 1 : 0) << 3;           // Bit 4
    byte6 |= (m_halfDulp ? 1 : 0) << 2;         // Bit 5
    // Bit 6-7 Reserved

    start.WriteU8(byte6);
}

uint32_t
Dect2020ExtRdCapabilityIE::Deserialize(Buffer::Iterator start)
{
    // Byte 0
    uint8_t byte0 = start.ReadU8();

    m_numOfPhyCapabilities = (byte0 >> 5) & 0x07; // Bit 0-2
    m_Release = byte0 & 0x1F;                     // Bit 3-7

    // Byte 1
    uint8_t byte1 = start.ReadU8();

    m_groupAssignment = ((byte1 >> 5) & 0x01);
    m_paging = ((byte1 >> 4) & 0x01);
    m_operatingModes = (byte1 >> 2) & 0x03;
    m_mesh = ((byte1 >> 1) & 0x01);
    m_scheduledAccessDataTransfer = (byte1 & 0x01);

    // Byte 2
    uint8_t byte2 = start.ReadU8();

    m_macSecurity = (byte2 >> 5) & 0x07;
    m_dlcServiceType = (byte2 >> 2) & 0x07;
    // Bit 6-7 reserved

    // Byte 3
    uint8_t byte3 = start.ReadU8();

    m_rdPowerClass = (byte3 >> 4) & 0x07;
    m_maxNssFoRx = (byte3 >> 2) & 0x03;
    m_rxForTxDiversity = byte3 & 0x03;

    // Byte 4
    uint8_t byte4 = start.ReadU8();

    m_rxGain = (byte4 >> 4) & 0x0F;
    m_maxMcs = byte4 & 0x0F;

    // Byte 5
    uint8_t byte5 = start.ReadU8();

    m_softBufferSize = (byte5 >> 4) & 0x0F;
    m_numOfHarqProcesses = (byte5 >> 2) & 0x03;
    // Bit 6–7 reserved

    // Byte 6
    uint8_t byte6 = start.ReadU8();

    m_harqFeedbackDelay = (byte6 >> 4) & 0x0F;
    m_dDelay = ((byte6 >> 3) & 0x01);
    m_halfDulp = ((byte6 >> 2) & 0x01);
    // Bit 6–7 reserved

    return GetSerializedSize();
}

void
Dect2020ExtRdCapabilityIE::SetNumOfPhyCapabilities(uint8_t val)
{
    m_numOfPhyCapabilities = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetNumOfPhyCapabilities() const
{
    return m_numOfPhyCapabilities;
}

void
Dect2020ExtRdCapabilityIE::SetRelease(uint8_t val)
{
    m_Release = val & 0x1F;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetRelease() const
{
    return m_Release;
}

void
Dect2020ExtRdCapabilityIE::SetGroupAssignment(bool enabled)
{
    m_groupAssignment = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetGroupAssignment() const
{
    return m_groupAssignment;
}

void
Dect2020ExtRdCapabilityIE::SetPaging(bool enabled)
{
    m_paging = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetPaging() const
{
    return m_paging;
}

void
Dect2020ExtRdCapabilityIE::SetOperatingModes(uint8_t val)
{
    m_operatingModes = val & 0x03;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetOperatingModes() const
{
    return m_operatingModes;
}

void
Dect2020ExtRdCapabilityIE::SetMesh(bool enabled)
{
    m_mesh = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetMesh() const
{
    return m_mesh;
}

void
Dect2020ExtRdCapabilityIE::SetScheduledAccessDataTransfer(bool enabled)
{
    m_scheduledAccessDataTransfer = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetScheduledAccessDataTransfer() const
{
    return m_scheduledAccessDataTransfer;
}

void
Dect2020ExtRdCapabilityIE::SetMacSecurity(uint8_t val)
{
    m_macSecurity = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetMacSecurity() const
{
    return m_macSecurity;
}

void
Dect2020ExtRdCapabilityIE::SetDlcServiceType(uint8_t val)
{
    m_dlcServiceType = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetDlcServiceType() const
{
    return m_dlcServiceType;
}

void
Dect2020ExtRdCapabilityIE::SetRdPowerClass(uint8_t val)
{
    m_rdPowerClass = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetRdPowerClass() const
{
    return m_rdPowerClass;
}

void
Dect2020ExtRdCapabilityIE::SetMaxNssFoRx(uint8_t val)
{
    m_maxNssFoRx = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetMaxNssFoRx() const
{
    return m_maxNssFoRx;
}

void
Dect2020ExtRdCapabilityIE::SetRxForTxDiversity(uint8_t val)
{
    m_rxForTxDiversity = val & 0x07;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetRxForTxDiversity() const
{
    return m_rxForTxDiversity;
}

void
Dect2020ExtRdCapabilityIE::SetRxGain(uint8_t val)
{
    m_rxGain = val & 0x0F;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetRxGain() const
{
    return m_rxGain;
}

void
Dect2020ExtRdCapabilityIE::SetMaxMcs(uint8_t val)
{
    m_maxMcs = val & 0x0F;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetMaxMcs() const
{
    return m_maxMcs;
}

void
Dect2020ExtRdCapabilityIE::SetSoftBufferSize(uint8_t val)
{
    m_softBufferSize = val & 0x0F;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetSoftBufferSize() const
{
    return m_softBufferSize;
}

void
Dect2020ExtRdCapabilityIE::SetNumOfHarqProcesses(uint8_t val)
{
    m_numOfHarqProcesses = val & 0x03;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetNumOfHarqProcesses() const
{
    return m_numOfHarqProcesses;
}

void
Dect2020ExtRdCapabilityIE::SetHarqFeedbackDelay(uint8_t val)
{
    m_harqFeedbackDelay = val & 0x0F;
}

uint8_t
Dect2020ExtRdCapabilityIE::GetHarqFeedbackDelay() const
{
    return m_harqFeedbackDelay;
}

void
Dect2020ExtRdCapabilityIE::SetDDelay(bool enabled)
{
    m_dDelay = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetDDelay() const
{
    return m_dDelay;
}

void
Dect2020ExtRdCapabilityIE::SetHalfDulp(bool enabled)
{
    m_halfDulp = enabled;
}

bool
Dect2020ExtRdCapabilityIE::GetHalfDulp() const
{
    return m_halfDulp;
}

} // namespace ns3 