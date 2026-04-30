#include "dect2020ext-phy-control-field-type-1.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhyControlFieldType1");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhyControlFieldType1);

Dect2020ExtPhyControlFieldType1::Dect2020ExtPhyControlFieldType1()
{
}

Dect2020ExtPhyControlFieldType1::~Dect2020ExtPhyControlFieldType1()
{
}

TypeId
Dect2020ExtPhyControlFieldType1::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhyControlFieldType1")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhyControlFieldType1>();
    return tid;
}

TypeId
Dect2020ExtPhyControlFieldType1::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtPhyControlFieldType1::Print(std::ostream& os) const
{
    // TODO: Implement
}

uint32_t
Dect2020ExtPhyControlFieldType1::GetSerializedSize() const
{
    return 5;
}

void
Dect2020ExtPhyControlFieldType1::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = 0;
    byte0 |= (static_cast<uint8_t>(m_packetLengthType)) << 4;
    byte0 |= m_packetLength;

    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_shortNetworkId);

    // byte 2 and 3
    start.WriteHtonU16(m_transmitterIdentity);

    uint8_t byte4 = 0;
    byte4 |= m_transmitPower << 4;
    byte4 |= m_DFMCS;
    start.WriteU8(byte4);
}

uint32_t
Dect2020ExtPhyControlFieldType1::Deserialize(Buffer::Iterator start)
{
    uint8_t byte0 = start.ReadU8();
    m_headerFormat = byte0 >> 5;
    m_packetLengthType = byte0 >> 4;
    m_packetLength = byte0 & 0x0F;

    // byte 1
    m_shortNetworkId = start.ReadU8();

    // byte 2 and 3
    m_transmitterIdentity = start.ReadNtohU16();

    uint8_t byte4 = start.ReadU8();
    m_transmitPower = byte4 >> 4;
    m_DFMCS = byte4 & 0x07;

    return GetSerializedSize();
}

uint8_t
Dect2020ExtPhyControlFieldType1::GetHeaderFormat() const
{
    return m_headerFormat;
}

void
Dect2020ExtPhyControlFieldType1::SetPacketLengthType(bool packetLengthType)
{
    m_packetLengthType = packetLengthType;
}

bool
Dect2020ExtPhyControlFieldType1::GetPacketLengthType() const
{
    return m_packetLengthType;
}

void
Dect2020ExtPhyControlFieldType1::SetPacketLength(uint8_t packetLength)
{
    if (packetLength >= 0 && packetLength <= 15)
    {
        m_packetLength = packetLength;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid packet length, must be between 0 and 15", static_cast<uint32_t>(packetLength));
    }
}

uint8_t
Dect2020ExtPhyControlFieldType1::GetPacketLength() const
{
    return m_packetLength;
}

void
Dect2020ExtPhyControlFieldType1::SetShortNetworkId(uint8_t shortNetworkId)
{
    if (shortNetworkId >= 0 && shortNetworkId <= 0xFF)
    {
        m_shortNetworkId = shortNetworkId;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid Short Network ID, must be between 0 and 0xFF", static_cast<uint32_t>(shortNetworkId));
    }
}

uint8_t
Dect2020ExtPhyControlFieldType1::GetShortNetworkId() const
{
    return m_shortNetworkId;
}

void
Dect2020ExtPhyControlFieldType1::SetTransmitterIdentity(uint16_t transmitterIdentity)
{
    if (transmitterIdentity >= 0 && transmitterIdentity <= 0xFFFF)
    {
        m_transmitterIdentity = transmitterIdentity;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid Transmitter Identity, must be between 0 and 0xFFFF", transmitterIdentity);
    }
}

uint16_t
Dect2020ExtPhyControlFieldType1::GetTransmitterIdentity() const
{
    return m_transmitterIdentity;
}

void
Dect2020ExtPhyControlFieldType1::SetTransmitPower(uint8_t transmitPower)
{
    if (transmitPower >= 0 && transmitPower <= 15)
    {
        m_transmitPower = transmitPower;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid transmit power, must be between 0 and 15", static_cast<uint32_t>(transmitPower));
    }
}

uint8_t
Dect2020ExtPhyControlFieldType1::GetTransmitPower() const
{
    return m_transmitPower;
}

void
Dect2020ExtPhyControlFieldType1::SetDFMCS(uint8_t DFMCS)
{
    if (DFMCS >= 0 && DFMCS <= 7)
    {
        m_DFMCS = DFMCS;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid DF MCS, must be between 0 and 7", static_cast<uint32_t>(DFMCS));
    }
}

uint8_t
Dect2020ExtPhyControlFieldType1::GetDFMCS() const
{
    return m_DFMCS;
}

} // namespace ns3