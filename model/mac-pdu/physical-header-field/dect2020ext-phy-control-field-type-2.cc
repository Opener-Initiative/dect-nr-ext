#include "dect2020ext-phy-control-field-type-2.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

#include "ns3/abort.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhyControlFieldType2");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhyControlFieldType2);

Dect2020ExtPhyControlFieldType2::Dect2020ExtPhyControlFieldType2()
{
}

Dect2020ExtPhyControlFieldType2::Dect2020ExtPhyControlFieldType2(PhyControlFieldType2Format headerFormat)
{
	m_headerFormat = headerFormat;
}

Dect2020ExtPhyControlFieldType2::~Dect2020ExtPhyControlFieldType2()
{
}

TypeId
Dect2020ExtPhyControlFieldType2::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhyControlFieldType2")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhyControlFieldType2>();
    return tid;
}

TypeId
Dect2020ExtPhyControlFieldType2::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtPhyControlFieldType2::Print(std::ostream& os) const
{
}

uint32_t
Dect2020ExtPhyControlFieldType2::GetSerializedSize() const
{
    // Note: both header formats are 10-byte long
    return 10;
}

void
Dect2020ExtPhyControlFieldType2::Serialize(Buffer::Iterator start) const
{
    uint8_t byte0 = 0; 
    byte0 |= m_headerFormat << 5;
    byte0 |= m_packetLengthType << 4;
    byte0 |= m_packetLength;

    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_shortNetworkId);

    // byte 2 and 3
    start.WriteU16(m_transmitterIdentity);

    // byte 4
    uint8_t byte4 = 0;
    byte4 |= m_transmitPower << 4;
    byte4 |= m_dfmcs;

    start.WriteU8(byte4);

    // byte 5 and 6
    start.WriteU16(m_receiverIdentity);

    if (m_headerFormat == FORMAT_000)
    {
        uint8_t byte7 = 0;
        byte7 |= m_numberOfSpatialStreams << 6;
        byte7 |= m_dfRedundancyVersion << 4;
        byte7 |= m_dfNewDataIndication << 3;
        byte7 |= m_dfHarqProcessNumber;

        start.WriteU8(byte7);

        uint8_t byte8 = 0;
        byte8 |= m_feedbackFormat << 4;
        byte8 |= m_feedbackInfo >> 8;

        start.WriteU8(byte8);

        uint8_t byte9 = 0;
        byte9 |= m_feedbackInfo & 0xFF;

        start.WriteU8(byte9);
    }
    else if (m_headerFormat == FORMAT_001)
    {
        uint8_t byte7 = 0;
        byte7 |= m_numberOfSpatialStreams << 6;

        start.WriteU8(byte7);

        uint8_t byte8 = 0;
        byte8 |= m_feedbackFormat << 4;
        byte8 |= m_feedbackInfo >> 8;

        start.WriteU8(byte8);

        uint8_t byte9 = 0;
        byte9 |= m_feedbackInfo & 0xFF;

        start.WriteU8(byte9);
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Unknown header format", static_cast<int>(m_headerFormat));
    }
}

uint32_t
Dect2020ExtPhyControlFieldType2::Deserialize(Buffer::Iterator start)
{
    uint8_t byte0 = start.ReadU8();
    m_headerFormat = static_cast<PhyControlFieldType2Format>(byte0 >> 5);
    m_packetLengthType = (byte0 >> 4) & 0x01;
    m_packetLength = byte0 & 0x0F;

    // byte 1
    m_shortNetworkId = start.ReadU8();

    // byte 2 and 3
    m_transmitterIdentity = start.ReadU16();

    // byte 4
    uint8_t byte4 = start.ReadU8();
    m_transmitPower = byte4 >> 4;
    m_dfmcs= byte4 & 0x0F;

    // byte 5 and 6
    m_receiverIdentity = start.ReadU16();

    if (m_headerFormat == FORMAT_000)
    {
        uint8_t byte7 = start.ReadU8();
        m_numberOfSpatialStreams = byte7 >> 6;
        m_dfRedundancyVersion = (byte7 >> 4) & 0x03;
        m_dfNewDataIndication = (byte7 >> 3) & 0x01;
        m_dfHarqProcessNumber = byte7 & 0x07;

        // byte 8 and 9
        uint8_t byte8 = start.ReadU8();
        m_feedbackFormat = byte8 >> 4;
        m_feedbackInfo = ((uint16_t)(byte8 & 0x0F) << 8) | start.ReadU8();
    }
    else if (m_headerFormat == FORMAT_001)
    {
        uint8_t byte7 = start.ReadU8();
        m_numberOfSpatialStreams = byte7 >> 6;

        // byte 8 and 9
        uint8_t byte8 = start.ReadU8();
        m_feedbackFormat = byte8 >> 4;
        m_feedbackInfo = ((uint16_t)(byte8 & 0x0F) << 8) | start.ReadU8();
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Unknown header format", m_headerFormat);
    }

    return GetSerializedSize();
}

void
Dect2020ExtPhyControlFieldType2::SetHeaderFormat(PhyControlFieldType2Format format)
{
    m_headerFormat = format;
}

Dect2020ExtPhyControlFieldType2::PhyControlFieldType2Format
Dect2020ExtPhyControlFieldType2::GetHeaderFormat() const
{
    return m_headerFormat;
}

void
Dect2020ExtPhyControlFieldType2::SetPacketLengthType(bool packetLengthType)
{
    m_packetLengthType = packetLengthType;
}

bool
Dect2020ExtPhyControlFieldType2::GetPacketLengthType() const
{
    return m_packetLengthType;
}

void
Dect2020ExtPhyControlFieldType2::SetPacketLength(uint8_t packetLength)
{
    if (packetLength >= 0 && packetLength <= 15)
    {
        m_packetLength = packetLength;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid packet length, must be between 0 and 15", packetLength);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetPacketLength() const
{
    return m_packetLength;
}

void
Dect2020ExtPhyControlFieldType2::SetShortNetworkId(uint8_t shortNetworkId)
{
    if (shortNetworkId >= 0 && shortNetworkId <= 0xFF)
    {
        m_shortNetworkId = shortNetworkId;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid short network ID, must be between 0 and 0xFF", shortNetworkId);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetShortNetworkId() const
{
    return m_shortNetworkId;
}

void
Dect2020ExtPhyControlFieldType2::SetTransmitterIdentity(uint16_t transmitterIdentity)
{
    if (transmitterIdentity >= 0 && transmitterIdentity <= 0xFFFF)
    {
        m_transmitterIdentity = transmitterIdentity;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid transmitter identity, must be between 0 and 0xFFFF", transmitterIdentity);
    }
}

uint16_t
Dect2020ExtPhyControlFieldType2::GetTransmitterIdentity() const
{
    return m_transmitterIdentity;
}

void
Dect2020ExtPhyControlFieldType2::SetTransmitPower(uint8_t transmitPower)
{
    if (transmitPower >= 0 && transmitPower <= 15)
    {
        m_transmitPower = transmitPower;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid transmit power, must be between 0 and 15", transmitPower);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetTransmitPower() const
{
    return m_transmitPower;
}

void
Dect2020ExtPhyControlFieldType2::SetDfmcs(uint8_t dfmcs)
{
    if (dfmcs >= 0 && dfmcs <= 15)
    {
        m_dfmcs= dfmcs;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid DF MCS, must be between 0 and 15", dfmcs);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetDfmcs() const
{
    return m_dfmcs;
}

void
Dect2020ExtPhyControlFieldType2::SetReceiverIdentity(uint16_t receiverIdentity)
{
    if (receiverIdentity >= 0 && receiverIdentity <= 0xFFFF)
    {
        m_receiverIdentity = receiverIdentity;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid receiver identity, must be between 0 and 0xFFFF", receiverIdentity);
    }
}

uint16_t
Dect2020ExtPhyControlFieldType2::GetReceiverIdentity() const
{
    return m_receiverIdentity;
}

void
Dect2020ExtPhyControlFieldType2::SetNumberOfSpatialStreams(uint8_t numberOfSpatialStreams)
{
    if (numberOfSpatialStreams >= 0 && numberOfSpatialStreams <= 3)
    {
        m_numberOfSpatialStreams = numberOfSpatialStreams;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid number of spatial streams, must be between 0 and 3", numberOfSpatialStreams);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetNumberOfSpatialStreams() const
{
    return m_numberOfSpatialStreams;
}

void
Dect2020ExtPhyControlFieldType2::SetDfRedundancyVersion(uint8_t dfRedundancyVersion)
{
    if (dfRedundancyVersion >= 0 && dfRedundancyVersion <= 3)
    {
        m_dfRedundancyVersion = dfRedundancyVersion;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid DF Redundancy Version, must be between 0 and 3", dfRedundancyVersion);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetDfRedundancyVersion() const
{
    return m_dfRedundancyVersion;
}

void
Dect2020ExtPhyControlFieldType2::SetDfNewDataIndication(bool dfNewDataIndication)
{
    m_dfNewDataIndication = dfNewDataIndication;
}

bool
Dect2020ExtPhyControlFieldType2::GetDfNewDataIndication() const
{
    return m_dfNewDataIndication;
}

void
Dect2020ExtPhyControlFieldType2::SetDfHarqProcessNumber(uint8_t dfHarqProcessNumber)
{
    if (dfHarqProcessNumber >= 0 && dfHarqProcessNumber <= 7)
    {
        m_dfHarqProcessNumber = dfHarqProcessNumber;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid DF HARQ Process Number, must be between 0 and 7", dfHarqProcessNumber);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetDfHarqProcessNumber() const
{
    return m_dfHarqProcessNumber;
}

void
Dect2020ExtPhyControlFieldType2::SetFeedbackFormat(uint8_t feedbackFormat)
{
    if (feedbackFormat >= 0 && feedbackFormat <= 15)
    {
        m_feedbackFormat = feedbackFormat;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid feedback format, must be between 0 and 15", feedbackFormat);
    }
}

uint8_t
Dect2020ExtPhyControlFieldType2::GetFeedbackFormat() const
{
    return m_feedbackFormat;
}

void
Dect2020ExtPhyControlFieldType2::SetFeedbackInfo(uint16_t feedbackInfo)
{
    if (feedbackInfo >= 0 && feedbackInfo <= 0xFFF)
    {
        m_feedbackInfo = feedbackInfo;
    }
    else
    {
        DECT2020EXT_MAC_PDU_FATAL("Invalid feedback info, must be between 0 and 0xFFF", feedbackInfo);
    }
}

uint16_t
Dect2020ExtPhyControlFieldType2::GetFeedbackInfo() const
{
    return m_feedbackInfo;
}

} // namespace ns3
