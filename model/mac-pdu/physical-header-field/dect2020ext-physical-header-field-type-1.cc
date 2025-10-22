#include "dect2020ext-physical-header-field-type-1.h"

#include "ns3/address-utils.h"
#include "ns3/log.h"

#include <cmath>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhysicalHeaderFieldType1");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhysicalHeaderFieldType1);

Dect2020ExtPhysicalHeaderFieldType1::Dect2020ExtPhysicalHeaderFieldType1()
{
}

Dect2020ExtPhysicalHeaderFieldType1::~Dect2020ExtPhysicalHeaderFieldType1()
{
}

TypeId
Dect2020ExtPhysicalHeaderFieldType1::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhysicalHeaderField")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhysicalHeaderFieldType1>();
    return tid;
}

TypeId
Dect2020ExtPhysicalHeaderFieldType1::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020ExtPhysicalHeaderFieldType1::Print(std::ostream& os) const
{
    // TODO: Implement
}

uint32_t
Dect2020ExtPhysicalHeaderFieldType1::GetSerializedSize(void) const
{
    return 5;
}

void
Dect2020ExtPhysicalHeaderFieldType1::Serialize(Buffer::Iterator start) const
{
    // ETSI 103 636 04: Figure 6.2.1-1

    uint8_t byte0 = 0;
    byte0 |= 0B000 << 5;
    byte0 |= m_packetLengthType << 4;
    byte0 |= m_packetLength;

    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_shortNetworkID);

    // byte 2 and 3
    start.WriteU16(m_transmitterIdentity);

    uint8_t byte4 = 0;
    byte4 |= m_transmitPower << 4;
    byte4 |= m_DFMCS;
    start.WriteU8(byte4);
}

uint32_t
Dect2020ExtPhysicalHeaderFieldType1::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    uint8_t byte0 = start.ReadU8();
    m_headerFormat = byte0 >> 5;
    m_packetLengthType = byte0 >> 4;
    m_packetLength = byte0 & 0x0F;
    bytesRead++;

    // byte 1
    m_shortNetworkID = start.ReadU8();
    bytesRead++;

    // byte 2 and 3
    m_transmitterIdentity = start.ReadU16();
    bytesRead += 2;

    uint8_t byte4 = start.ReadU8();
    m_transmitPower = byte4 >> 4;
    m_DFMCS = byte4 & 0x07;
    bytesRead++;

    return bytesRead;
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetHeaderFormat() const
{
    return m_headerFormat;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetPacketLengthType(uint8_t packetLengthType)
{
    if (packetLengthType == 0 || packetLengthType == 1)
    {
        m_packetLengthType = packetLengthType;
    }
    else
    {
        NS_LOG_WARN("Invalid Packet Length Type. Must be 0 or 1.");
    }
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetPacketLengthType() const
{
    return m_packetLengthType;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetPacketLength(uint8_t packetLength)
{
    if (packetLength >= 0 && packetLength <= 15)
    {
        m_packetLength = packetLength;
    }
    else
    {
        NS_LOG_WARN("Invalid Packet Length. Must be between 0 and 15");
    }
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetPacketLength() const
{
    return m_packetLength;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetShortNetworkID(uint8_t shortNetworkID)
{
    if (shortNetworkID >= 0 && shortNetworkID <= pow(2, 8) - 1)
    {
        m_shortNetworkID = shortNetworkID;
    }
    else
    {
        NS_LOG_WARN("Invalid Short Network ID. Must be between 0 and " << (pow(2, 8) - 1));
    }
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetShortNetworkID() const
{
    return m_shortNetworkID;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetTransmitterIdentity(uint16_t transmitterIdentity)
{
    if (transmitterIdentity >= 0 && transmitterIdentity <= pow(2, 16) - 1)
    {
        m_transmitterIdentity = transmitterIdentity;
    }
    else
    {
        NS_LOG_WARN("Invalid Transmitter Identity. Must be between 0 and " << (pow(2, 16) - 1));
    }
}

uint16_t
Dect2020ExtPhysicalHeaderFieldType1::GetTransmitterIdentity() const
{
    return m_transmitterIdentity;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetTransmitPower(uint8_t transmitPower)
{
    if (transmitPower >= 0 && transmitPower <= (pow(2, 4) - 1))
    {
        m_transmitPower = transmitPower;
    }
    else
    {
        NS_LOG_WARN("Invalid Transmit Power. Must be between 0 and " << (pow(2, 4) - 1));
    }
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetTransmitPower() const
{
    return m_transmitPower;
}

void
Dect2020ExtPhysicalHeaderFieldType1::SetDFMCS(uint8_t DFMCS)
{
    if (DFMCS >= 0 && DFMCS <= pow(2, 3) - 1)
    {
        m_DFMCS = DFMCS;
    }
    else
    {
        NS_LOG_WARN("Invalid DF MCS. Must be between 0 and " << pow(2, 3) - 1);
    }
}

uint8_t
Dect2020ExtPhysicalHeaderFieldType1::GetDFMCS() const
{
    return m_DFMCS;
}

} // namespace ns3
