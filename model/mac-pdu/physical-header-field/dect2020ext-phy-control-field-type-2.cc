#include "dect2020ext-phy-control-field-type-2.h"

#include <cmath>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPHYControlFieldType2");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPHYControlFieldType2);

Dect2020ExtPHYControlFieldType2::Dect2020ExtPHYControlFieldType2(Dect2020ExtHeaderFormat headerFormat)
{
	m_headerFormat = headerFormat;
}

Dect2020ExtPHYControlFieldType2::~Dect2020ExtPHYControlFieldType2()
{
}

TypeId
Dect2020ExtPHYControlFieldType2::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPHYControlFieldType2")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPHYControlFieldType2>();
    return tid;
}

TypeId
Dect2020ExtPHYControlFieldType2::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtPHYControlFieldType2::Print(std::ostream& os) const
{
}

uint32_t
Dect2020ExtPHYControlFieldType2::GetSerializedSize() const
{
    return 10;
}

void
Dect2020ExtPHYControlFieldType2::Serialize(Buffer::Iterator start) const
{
    uint8_t byte0 = 0; 
    byte0 |= m_headerFormat << 5;
    byte0 |= m_packetLengthType << 4;
    byte0 |= m_packetLength;

    start.WriteU8(byte0);

    // byte 1
    start.WriteU8(m_shortNetworkID);

    // byte 2 and 3
    start.WriteU16(m_transmitterIdentity);

    // byte 4
    uint8_t byte4 = 0;
    byte4 |= m_transmitPower << 4;
    byte4 |= m_DFMCS;

    start.WriteU8(byte4);

    // byte 5 and 6
    start.WriteU16(m_receiverIdentity);

    if (m_headerFormat == FORMAT_000)
    {
        uint8_t byte7 = 0;
        byte7 |= m_numberOfSpatialStreams << 6;
        byte7 |= m_DFRedundancyVersion << 4;
        byte7 |= m_DFNewDataIndication << 3;
        byte7 |= m_DFHARQProcessNumber;

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
}

uint32_t
Dect2020ExtPHYControlFieldType2::Deserialize(Buffer::Iterator start)
{
    int bytesRead = 0;

    uint8_t byte0 = start.ReadU8();
    m_headerFormat = byte0 >> 5;
    m_packetLengthType = (byte0 >> 4) & 0x01;
    m_packetLength = byte0 & 0x0F;

    bytesRead++;

    // byte 1
    m_shortNetworkID = start.ReadU8();

    bytesRead++;

    // byte 2 and 3
    m_transmitterIdentity = start.ReadU16();

    uint8_t byte4 = start.ReadU8();
    m_transmitPower = byte4 >> 4;
    m_DFMCS = byte4 & 0x0F;

    bytesRead++;

    // byte 5 and 6
    m_receiverIdentity = start.ReadU16();

    bytesRead += 2;

    if (m_headerFormat == FORMAT_000)
    {
        uint8_t byte7 = start.ReadU8();
        m_numberOfSpatialStreams = byte7 >> 6;
        m_DFRedundancyVersion = (byte7 >> 4) & 0x03;
        m_DFNewDataIndication = (byte7 >> 3) & 0x01;
        m_DFHARQProcessNumber = byte7 & 0x07;

        bytesRead++;

        // byte 8 and 9
        uint8_t byte8 = start.ReadU8();
        m_feedbackFormat = byte8 >> 4;
        m_feedbackInfo = (byte8 << 8) | start.ReadU8();

        bytesRead += 2;
    }
    else if (m_headerFormat == FORMAT_001)
    {
        uint8_t byte7 = start.ReadU8();
        m_numberOfSpatialStreams = byte7 >> 6;

        bytesRead++;

        // byte 8 and 9
        uint8_t byte8 = start.ReadU8();
        m_feedbackFormat = byte8 >> 4;
        m_feedbackInfo = (byte8 << 8) | start.ReadU8();

        bytesRead += 2;
    }

    return bytesRead;
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetHeaderFormat() const
{
    return m_headerFormat;
}

void
Dect2020ExtPHYControlFieldType2::SetPacketLengthType(uint8_t packetLengthType)
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
Dect2020ExtPHYControlFieldType2::GetPacketLengthType() const
{
    return m_packetLengthType;
}

void
Dect2020ExtPHYControlFieldType2::SetPacketLength(uint8_t packetLength)
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
Dect2020ExtPHYControlFieldType2::GetPacketLength() const
{
    return m_packetLength;
}

void
Dect2020ExtPHYControlFieldType2::SetShortNetworkID(uint8_t shortNetworkID)
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
Dect2020ExtPHYControlFieldType2::GetShortNetworkID() const
{
    return m_shortNetworkID;
}

void
Dect2020ExtPHYControlFieldType2::SetTransmitterIdentity(uint16_t transmitterIdentity)
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
Dect2020ExtPHYControlFieldType2::GetTransmitterIdentity() const
{
    return m_transmitterIdentity;
}

void
Dect2020ExtPHYControlFieldType2::SetTransmitPower(uint8_t transmitPower)
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
Dect2020ExtPHYControlFieldType2::GetTransmitPower() const
{
    return m_transmitPower;
}

void
Dect2020ExtPHYControlFieldType2::SetDFMCS(uint8_t DFMCS)
{
    if (DFMCS >= 0 && DFMCS <= pow(2, 4) - 1)
    {
        m_DFMCS = DFMCS;
    }
    else
    {
        NS_LOG_WARN("Invalid DF MCS. Must be between 0 and " << pow(2, 4) - 1);
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetDFMCS() const
{
    return m_DFMCS;
}

void
Dect2020ExtPHYControlFieldType2::SetReceiverIdentity(uint16_t receiverIdentity)
{
    if (receiverIdentity >= 0 && receiverIdentity <= pow(2, 16) - 1)
    {
        m_receiverIdentity = receiverIdentity;
    }
    else
    {
        NS_LOG_WARN("Invalid Receiver Identity. Must be between 0 and " << (pow(2, 16) - 1));
    }
}

uint16_t
Dect2020ExtPHYControlFieldType2::GetReceiverIdentity() const
{
    return m_receiverIdentity;
}

void
Dect2020ExtPHYControlFieldType2::SetNumberOfSpatialStreams(uint8_t numberOfSpatialStreams)
{
    if (numberOfSpatialStreams >= 0 && numberOfSpatialStreams <= pow(2, 2) - 1)
    {
        m_numberOfSpatialStreams = numberOfSpatialStreams;
    }
    else
    {
        NS_LOG_WARN("Invalid Number of Spatial Streams. Must be between 0 and " << (pow(2, 2) - 1));
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetNumberOfSpatialStreams() const
{
    return m_numberOfSpatialStreams;
}

void
Dect2020ExtPHYControlFieldType2::SetDFRedundancyVersion(uint8_t dFRedundancyVersion)
{
    if (dFRedundancyVersion >= 0 && dFRedundancyVersion <= pow(2, 2) - 1)
    {
        m_DFRedundancyVersion = dFRedundancyVersion;
    }
    else
    {
        NS_LOG_WARN("Invalid DF Redundancy Version. Must be between 0 and " << (pow(2, 2) - 1));
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetDFRedundancyVersion() const
{
    return m_DFRedundancyVersion;
}

void
Dect2020ExtPHYControlFieldType2::SetDFNewDataIndication(uint8_t dFNewDataIndication)
{
    if (dFNewDataIndication == 0 || dFNewDataIndication == 1)
    {
        m_DFNewDataIndication = dFNewDataIndication;
    }
    else
    {
        NS_LOG_WARN("Invalid DF New Data Indication. Must be 0 or 1.");
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetDFNewDataIndication() const
{
    return m_DFNewDataIndication;
}

void
Dect2020ExtPHYControlFieldType2::SetDFHARQProcessNumber(uint8_t dFHARQProcessNumber)
{
    if (dFHARQProcessNumber >= 0 && dFHARQProcessNumber <= pow(2, 3) - 1)
    {
        m_DFHARQProcessNumber = dFHARQProcessNumber;
    }
    else
    {
        NS_LOG_WARN("Invalid DF HARQ Process Number. Must be between 0 and " << (pow(2, 3) - 1));
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetDFHARQProcessNumber() const
{
    return m_DFHARQProcessNumber;
}

void
Dect2020ExtPHYControlFieldType2::SetFeedbackFormat(uint8_t feedbackFormat)
{
    if (feedbackFormat >= 0 && feedbackFormat <= pow(2, 4) - 1)
    {
        m_feedbackFormat = feedbackFormat;
    }
    else
    {
        NS_LOG_WARN("Invalid Feedback Format. Must be between 0 and " << (pow(2, 4) - 1));
    }
}

uint8_t
Dect2020ExtPHYControlFieldType2::GetFeedbackFormat() const
{
    return m_feedbackFormat;
}

void
Dect2020ExtPHYControlFieldType2::SetFeedbackInfo(uint16_t feedbackInfo)
{
    if (feedbackInfo >= 0 && feedbackInfo <= pow(2, 12) - 1)
    {
        m_feedbackInfo = feedbackInfo;
    }
    else
    {
        NS_LOG_WARN("Invalid Feedback Info. Must be between 0 and " << (pow(2, 12) - 1));
    }
}

uint16_t
Dect2020ExtPHYControlFieldType2::GetFeedbackInfo() const
{
    return m_feedbackInfo;
}

} // namespace ns3
