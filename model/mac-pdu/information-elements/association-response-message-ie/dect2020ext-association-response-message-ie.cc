#include "dect2020ext-association-response-message-ie.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtAssociationResponseMessageIe");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtAssociationResponseMessageIe);

Dect2020ExtAssociationResponseMessageIe::Dect2020ExtAssociationResponseMessageIe()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtAssociationResponseMessageIe::~Dect2020ExtAssociationResponseMessageIe()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtAssociationResponseMessageIe::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtAssociationResponseMessageIe")
                            .SetParent<Header>()
                            .AddConstructor<Dect2020ExtAssociationResponseMessageIe>();
    return tid;
}

TypeId
Dect2020ExtAssociationResponseMessageIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtAssociationResponseMessageIe::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    uint32_t size = 1; // Byte 0

    if (!m_associationAccepted)
    {
        size += 1; // Byte 1: Reject Cause + Timer
    }

    if (m_harqMod)
    {
        size += 2; // Byte 2–3: HARQ RX + TX
    }

    size += 1; // Byte 4: Flow ID

    if (m_groupIdAndResourceTagIncluded)
    {
        size += 2; // Byte 5–6: Group ID + Resource Tag
    }

    return size;
}

void
Dect2020ExtAssociationResponseMessageIe::Serialize(Buffer::Iterator start) const
{
    // Byte 0
    uint8_t byte0 = 0;
    byte0 |= (m_associationAccepted << 7); // Bit 0
    // Bit 1 Reserved
    byte0 |= (m_harqMod << 5);                       // Bit 2
    byte0 |= (m_numberOfFlows & 0x07) << 2;          // Bit 3-5
    byte0 |= (m_groupIdAndResourceTagIncluded << 1); // Bit 6
    // Bit 7 Reserved

    start.WriteU8(byte0);

    if (m_associationAccepted == 0) // Association rejected
    {
        // Byte 1
        uint8_t byte1 = 0;
        byte1 |= (m_rejectCause & 0x0F) << 4; // Bit 0-3
        byte1 |= (m_rejectTimer & 0x0F);      // Bit 4-7

        start.WriteU8(byte1);

        NS_LOG_INFO("Serialize: byte1 = 0x" << std::hex << static_cast<uint32_t>(byte1));
    }

    if (m_harqMod == 1) // HARQ configuration present
    {
        // Byte 2
        uint8_t byte2 = 0;
        byte2 |= (m_harqProcessesRx & 0x07) << 5; // Bit 0-2
        byte2 |= m_maxHarqReRxDelay & 0x1F;       // Bit 3-7

        start.WriteU8(byte2);

        // Byte 3
        uint8_t byte3 = 0;
        byte3 |= (m_harqProcessesTx & 0x07) << 5; // Bit 0-2
        byte3 |= m_maxHarqReTxDelay & 0x1F;       // Bit 3-7

        start.WriteU8(byte3);
    }

    // Byte 4
    uint8_t byte4 = 0;
    byte4 |= (m_flowId & 0x3F); // Bit 2-7

    start.WriteU8(byte4);

    if (m_groupIdAndResourceTagIncluded == 1)
    {
        // Byte 5
        uint8_t byte5 = 0;
        byte5 |= (m_groupId & 0x7F); // Bit 1-7

        start.WriteU8(byte5);

        // Byte 6
        uint8_t byte6 = 0;
        byte6 |= (m_resourceTag & 0x7F); // Bit 1-7

        start.WriteU8(byte6);
    }
}

uint32_t
Dect2020ExtAssociationResponseMessageIe::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    Buffer::Iterator i = start;

    // Byte 0
    uint8_t byte0 = i.ReadU8();
    SetAssociationAccepted((byte0 >> 7) & 0x01); // Bit 0
    // Bit 1 Reserved
    SetHarqMod((byte0 >> 5) & 0x01);                       // Bit 2
    SetNumberOfFlows((byte0 >> 2) & 0x07);                 // Bit 3-5
    SetGroupIdAndResourceTagIncluded((byte0 >> 1) & 0x01); // Bit 6
    // Bit 7 Reserved

    if (m_associationAccepted == 0) // Association rejected
    {
        // Byte 1
        uint8_t byte1 = i.ReadU8();

        NS_LOG_INFO("Deserialize: byte1 = 0x" << std::hex << static_cast<uint32_t>(byte1));

        SetRejectCause((byte1 >> 4) & 0x0F); // Bit 0-3
        SetRejectTimer(byte1 & 0x0F);        // Bit 4-7
    }

    if (m_harqMod == 1) // HARQ configuration present
    {
        // Byte 2
        uint8_t byte2 = i.ReadU8();
        SetHarqProcessesRx((byte2 >> 5) & 0x07); // Bit 0-2
        SetMaxHarqReRxDelay(byte2 & 0x1F);       // Bit 3-7

        // Byte 3
        uint8_t byte3 = i.ReadU8();
        SetHarqProcessesTx((byte3 >> 5) & 0x07); // Bit 0-2
        SetMaxHarqReTxDelay(byte3 & 0x1F);       // Bit 3-7
    }

    // Byte 4
    uint8_t byte4 = i.ReadU8();
    SetFlowId(byte4 & 0x3F); // Bit 2-7

    if (m_groupIdAndResourceTagIncluded == 1)
    {
        // Byte 5
        uint8_t byte5 = i.ReadU8();
        SetGroupId(byte5 & 0x7F); // Bit 1-7

        // Byte 6
        uint8_t byte6 = i.ReadU8();
        SetResourceTag(byte6 & 0x7F); // Bit 1-7
    }

    return i.GetDistanceFrom(start);
}

void
Dect2020ExtAssociationResponseMessageIe::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020ExtAssociationResponseMessageIe";
}

void
Dect2020ExtAssociationResponseMessageIe::SetAssociationAccepted(bool accepted)
{
    m_associationAccepted = accepted;
}

bool
Dect2020ExtAssociationResponseMessageIe::GetAssociationAccepted() const
{
    return m_associationAccepted;
}

void
Dect2020ExtAssociationResponseMessageIe::SetHarqMod(bool harqMod)
{
    m_harqMod = harqMod;
}

bool
Dect2020ExtAssociationResponseMessageIe::GetHarqMod() const
{
    return m_harqMod;
}

void
Dect2020ExtAssociationResponseMessageIe::SetNumberOfFlows(uint8_t numberOfFlows)
{
    m_numberOfFlows = numberOfFlows & 0x07; // 3 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetNumberOfFlows() const
{
    return m_numberOfFlows;
}

void
Dect2020ExtAssociationResponseMessageIe::SetGroupIdAndResourceTagIncluded(bool included)
{
    m_groupIdAndResourceTagIncluded = included;
}

bool
Dect2020ExtAssociationResponseMessageIe::GetGroupIdAndResourceTagIncluded() const
{
    return m_groupIdAndResourceTagIncluded;
}

void
Dect2020ExtAssociationResponseMessageIe::SetRejectCause(uint8_t cause)
{
    m_rejectCause = cause & 0x0F; // 4 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetRejectCause() const
{
    return m_rejectCause;
}

void
Dect2020ExtAssociationResponseMessageIe::SetRejectTimer(uint8_t timer)
{
    m_rejectTimer = timer & 0x0F; // 4 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetRejectTimer() const
{
    return m_rejectTimer;
}

void
Dect2020ExtAssociationResponseMessageIe::SetHarqProcessesRx(uint8_t value)
{
    m_harqProcessesRx = value & 0x07; // 3 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetHarqProcessesRx() const
{
    return m_harqProcessesRx;
}

void
Dect2020ExtAssociationResponseMessageIe::SetMaxHarqReRxDelay(uint8_t delay)
{
    m_maxHarqReRxDelay = delay & 0x1F; // 5 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetMaxHarqReRxDelay() const
{
    return m_maxHarqReRxDelay;
}

void
Dect2020ExtAssociationResponseMessageIe::SetHarqProcessesTx(uint8_t value)
{
    m_harqProcessesTx = value & 0x07; // 3 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetHarqProcessesTx() const
{
    return m_harqProcessesTx;
}

void
Dect2020ExtAssociationResponseMessageIe::SetMaxHarqReTxDelay(uint8_t delay)
{
    m_maxHarqReTxDelay = delay & 0x1F; // 5 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetMaxHarqReTxDelay() const
{
    return m_maxHarqReTxDelay;
}

void
Dect2020ExtAssociationResponseMessageIe::SetFlowId(uint8_t flowId)
{
    m_flowId = flowId & 0x3F; // 6 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetFlowId() const
{
    return m_flowId;
}

void
Dect2020ExtAssociationResponseMessageIe::SetGroupId(uint8_t groupId)
{
    m_groupId = groupId & 0x7F; // 7 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetGroupId() const
{
    return m_groupId;
}

void
Dect2020ExtAssociationResponseMessageIe::SetResourceTag(uint8_t resourceTag)
{
    m_resourceTag = resourceTag & 0x7F; // 7 Bit
}

uint8_t
Dect2020ExtAssociationResponseMessageIe::GetResourceTag() const
{
    return m_resourceTag;
}

} // namespace ns3 