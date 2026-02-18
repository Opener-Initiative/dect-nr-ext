#include "dect2020ext-phy-control-field-type-2.h"

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

} // namespace ns3
