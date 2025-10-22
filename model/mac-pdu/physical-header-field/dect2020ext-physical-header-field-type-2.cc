#include "dect2020ext-physical-header-field-type-1.h"

#include "ns3/address-utils.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhysicalHeaderField");

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhysicalHeaderField);

Dect2020ExtPhysicalHeaderField::Dect2020ExtPhysicalHeaderField()
{
}

Dect2020ExtPhysicalHeaderField::~Dect2020ExtPhysicalHeaderField()
{
}

TypeId
Dect2020ExtPhysicalHeaderField::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhysicalHeaderField")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhysicalHeaderField>();
    return tid;
}

TypeId
Dect2020ExtPhysicalHeaderField::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
Dect2020ExtPhysicalHeaderField::Print(std::ostream& os) const
{
    os << "Physical Header Field:" << std::endl << "Header Format: " << static_cast<int>(m_headerFormat) << std::endl
       << "Packet Length: " << static_cast<int>(m_packetLength) << std::endl
       << "Short Network ID: 0x" << std::hex << static_cast<int>(m_shortNetworkID) << std::endl
       << "Transmitter Identity: 0x" << std::hex << static_cast<int>(m_transmitterIdentity)
       << std::endl
       << "Transmit power: " << static_cast<int>(m_transmitPower) << std::endl
       << "DF MCS: " << static_cast<int>(m_DFMCS) << std::endl;
}

uint32_t
Dect2020ExtPhysicalHeaderField::GetSerializedSize(void) const
{
    if(m_type != Dect2020ExtPhysicalHeaderFieldType::TYPE1 && m_type != Dect2020ExtPhysicalHeaderFieldType::TYPE2)
    {
        throw std::runtime_error("[ERROR] Physical Layer Control Field type was not selected");
    }
    else
    {
        if(m_type == Dect2020ExtPhysicalHeaderFieldType::TYPE1)
        {
            return 5;
        }
        else
        {
            return 10;
        }
    }
}

void
Dect2020ExtPhysicalHeaderField::Serialize(Buffer::Iterator start) const
{
    // ETSI 103 636 04: Figure 6.2.1-1

    if(m_type == Type::TYPE1)
    {
        // Byte 0
        uint8_t byte0 = 0;
        // Bit 0-2 Header Format: Set to 000
        byte0 |= (m_packetLengthType << 4);
        byte0 |= (m_packetLength & 0x0F);

        start.WriteU8(byte0);

        // Byte 1
        uint8_t byte1 = (m_shortNetworkID & 0xFF);

        start.WriteU8(byte1);

        // Byte 2
        uint8_t byte2 = (m_transmitterIdentity >> 8) & 0xFF;

        start.WriteU8(byte2);

        // Byte 3
        uint8_t byte3 = (m_transmitterIdentity) & 0xFF;

        start.WriteU8(byte3);

        // Byte 4
        uint8_t byte4 = (m_transmitPower & 0x0F) << 4;
        // Bit 4 reserved
        byte4 |= (m_dFMCS & 0x3);

        start.WriteU8(byte4);
    }
    else
    {
        // Byte 0
        uint8_t byte0 = 0;
        // Bit 0-2 Header Format: Set to 000
        byte0 |= (m_packetLengthType << 4);
        byte0 |= (m_packetLength & 0x0F);

        start.WriteU8(byte0);

        // Byte 1
        uint8_t byte1 = (m_shortNetworkID & 0xFF);

        start.WriteU8(byte1);

        // Byte 2
        uint8_t byte2 = (m_transmitterIdentity >> 8) & 0xFF;

        start.WriteU8(byte2);

        // Byte 3
        uint8_t byte3 = (m_transmitterIdentity) & 0xFF;

        start.WriteU8(byte3);

        // Byte 4
        uint8_t byte4 = (m_transmitPower & 0x0F) << 4;
        // Bit 4 reserved
        byte4 |= (m_dFMCS & 0x3);

        start.WriteU8(byte4);

        // byte 5
        uint8_t byte5 = (m_receiverIdentity >> 8) & 0xFF;

        start.WriteU8(byte5);

        // byte 6
        uint8_t byte6 = (m_receiverIdentity) & 0xFF;

        start.WriteU8(byte6);

        // byte 7
        uint8_t byte7;
        byte7 |= (m_spatialStreams << 5);
        byte7 |= (m_DFRedundancyVersion << 4);
        byte7 |= (m_DFNewDataIndication << 3);
        byte7 |= m_DFHarqProcessNr;

        start.WriteU8(byte7);

        // byte 8
        uint8_t byte8;
        byte8 |= (m_feedbackFormat << 4);
        byte8 |= (m_feedbackInfo >> 8);

        start.WriteU8(byte8);

        // byte 9
        uint8_t byte9;
        byte9 |= (m_feedbackInfo & 0x0FF);

        start.WriteU8(byte9);
    }
}

uint32_t
Dect2020ExtPhysicalHeaderField::Deserialize(Buffer::Iterator start)
{
    if(m_type == Type::TYPE1)
    {

        uint32_t bytesRead = 0;
        uint8_t byte0 = start.ReadU8();
        bytesRead++;

        // Bit 0-2 set to 0 (Header format)
        m_packetLengthType = (byte0 & (1 << 4)) != 0;
        m_packetLength = (byte0 & 0x0F);

        // Byte 1
        uint8_t byte1 = start.ReadU8();
        bytesRead++;
        m_shortNetworkID = (byte1 & 0xFF);

        // Byte 2-3
        uint8_t byte2 = start.ReadU8();
        uint8_t byte3 = start.ReadU8();

        m_transmitterIdentity = (byte2 << 8) | byte3;
        bytesRead += 2;

        // Byte 4
        uint8_t byte4 = start.ReadU8();
        bytesRead++;
        m_transmitPower = (byte4 & 0xF0) >> 4;
        m_dFMCS = (byte4 & 0x03);

        return bytesRead;
    }
    else
    {
        uint32_t bytesRead = 0;
        uint8_t byte0 = start.ReadU8();
        bytesRead++;

        // Bit 0-2 set to 0 (Header format)
        m_packetLengthType = (byte0 & (1 << 4)) != 0;
        m_packetLength = (byte0 & 0x0F);

        // Byte 1
        uint8_t byte1 = start.ReadU8();
        bytesRead++;
        m_shortNetworkID = (byte1 & 0xFF);

        // Byte 2-3
        uint8_t byte2 = start.ReadU8();
        uint8_t byte3 = start.ReadU8();

        m_transmitterIdentity = (byte2 << 8) | byte3;
        bytesRead += 2;

        // Byte 4
        uint8_t byte4 = start.ReadU8();
        bytesRead++;
        m_transmitPower = (byte4 & 0xF0) >> 4;
        m_dFMCS = (byte4 & 0x03);

        // byte 5 and byte 6
        uint16_t byte5and6 = start.ReadU16();
        bytesRead += 2;

        // byte 7
        uint8_t byte7 = start.ReadU8();
        bytesRead++;

        m_spatialStreams = (byte7 >> 0x6);
        m_DFRedundancyVersion = ((byte7 >> 4) & 0x3);
        m_DFNewDataIndication = ((byte7 >> 3) & 0x1);
        m_DFHarqProcessNr =  (byte7 & 0x7);

        // byte 8
        uint8_t byte8 = start.ReadU8();
        bytesRead++;

        m_feedbackFormat = (byte8 >> 4);
        m_feedbackInfo = (byte8 & 0xF);

        // byte 9
        uint8_t byte9 = start.ReadU8();
        bytesRead++;

        m_feedbackInfo |= (m_feedbackInfo << 8);
        m_feedbackInfo |= byte9;

        return bytesRead;
    }
}

void
Dect2020ExtPhysicalHeaderField::SetPacketLengthType(uint8_t packetLengthType)
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
Dect2020ExtPhysicalHeaderField::GetPacketLengthType()
{
    return m_packetLengthType;
}

void
Dect2020ExtPhysicalHeaderField::SetPacketLength(uint8_t packetLength)
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
Dect2020ExtPhysicalHeaderField::GetPacketLength()
{
    return m_packetLength;
}

void
Dect2020ExtPhysicalHeaderField::SetShortNetworkID(uint8_t shortNetworkID)
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
Dect2020ExtPhysicalHeaderField::GetShortNetworkID()
{
    return m_shortNetworkID;
}

void
Dect2020ExtPhysicalHeaderField::SetTransmitterIdentity(uint16_t transmitterIdentity)
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
Dect2020ExtPhysicalHeaderField::GetTransmitterIdentity()
{
    return m_transmitterIdentity;
}

void
Dect2020ExtPhysicalHeaderField::SetTransmitPower(uint8_t transmitPower)
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
Dect2020ExtPhysicalHeaderField::GetTransmitPower()
{
    return m_transmitPower;
}

void
Dect2020ExtPhysicalHeaderField::SetDFMCS(uint8_t dFMCS)
{
    if (dFMCS >= 0 && dFMCS <= pow(2, 3) - 1)
    {
        m_dFMCS = dFMCS;
    }
    else
    {
        NS_LOG_WARN("Invalid DF MCS. Must be between 0 and " << pow(2, 3) - 1);
    }
}

uint8_t
Dect2020ExtPhysicalHeaderField::GetDFMCS()
{
    return m_dFMCS;
}

void
Dect2020ExtPhysicalHeaderField::SetType(Dect2020ExtPhysicalHeaderField::Type type)
{
    m_type = type;
}

Dect2020ExtPhysicalHeaderField::Type
Dect2020ExtPhysicalHeaderField::GetType() const
{
    return m_type;
}

} // namespace ns3
