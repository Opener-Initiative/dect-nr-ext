#include "dect2020ext-random-access-resource-ie.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRandomAccessResourceIe");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtRandomAccessResourceIe);

Dect2020ExtRandomAccessResourceIe::Dect2020ExtRandomAccessResourceIe()
{
}

Dect2020ExtRandomAccessResourceIe::~Dect2020ExtRandomAccessResourceIe()
{
}

TypeId
Dect2020ExtRandomAccessResourceIe::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtRandomAccessResourceIe")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtRandomAccessResourceIe>();
    return tid;
}

TypeId
Dect2020ExtRandomAccessResourceIe::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
Dect2020ExtRandomAccessResourceIe::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
    uint32_t size = 0;

    size += 1; // Byte 0 (repeat, flags)
    // Byte 1 (reserved for μ ≥ 4) is not used

    size += 1; // Byte 2: StartSubslot (8 Bit)
    size += 1; // Byte 3: LengthType + RARA Length
    size += 1; // Byte 4: Max RACH Length Type + Max Length + CWmin
    size += 1; // Byte 5: DECT delay + ResponseWindow + CWmax

    if (m_repeat == 1 || m_repeat == 2)
    {
        size += 2; // Byte 6–7: Repetition + Validity
    }

    if (m_systemFrameNumberFieldIncluded)
    {
        size += 1; // Byte 8: SFN Value
    }

    if (m_channelFieldIncluded)
    {
        size += 2; // Bytes 9–10: Channel (13 Bit)
    }

    if (m_separateChannelFieldIncluded)
    {
        size += 2; // Bytes 11–12: Separate Channel (13 Bit)
    }

    return size;
}

void
Dect2020ExtRandomAccessResourceIe::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
    // Byte 0
    uint8_t byte0 = 0;

    // Bit 0-2 Reserved
    byte0 |= (m_repeat & 0x03) << 3;                          // Bit 3-4
    byte0 |= (m_systemFrameNumberFieldIncluded ? 1 : 0) << 2; // Bit 5
    byte0 |= (m_channelFieldIncluded ? 1 : 0) << 1;           // Bit 6
    byte0 |= (m_separateChannelFieldIncluded ? 1 : 0) << 0;   // Bit 7

    start.WriteU8(byte0);

    // if (false) // Byte1 only if subcarrier scaling factor >= 4 --> currently not implemented
    // {
    //     // Byte 1
    // }

    // Byte 2
    uint8_t byte2 = m_startSubslot & 0xFF; // Bit 0-7;

    start.WriteU8(byte2);

    // Byte 3
    uint8_t byte3 = 0;
    byte3 |= (m_lengthType ? 1 : 0) << 7; // Bit 0
    byte3 |= (m_raraLength & 0x7F);       // Bit 1-7

    start.WriteU8(byte3);

    // Byte 4
    uint8_t byte4 = 0;
    byte4 |= (m_maxRachLengthType ? 1 : 0) << 7; // Bit 0
    byte4 |= (m_maxRachLength & 0x0F) << 3;      // Bit 1-4
    byte4 |= (m_cwMin_sig & 0x07);               // Bit 5-7

    start.WriteU8(byte4);

    // Byte 5
    uint8_t byte5 = 0;
    byte5 |= (m_dectDelay ? 1 : 0) << 7;     // Bit 0
    byte5 |= (m_responseWindow & 0x0F) << 3; // Bit 1-4
    byte5 |= (m_cwMax_sig & 0x07);           // Bit 5-7

    start.WriteU8(byte5);

    if (m_repeat == 1 || m_repeat == 2)
    {
        // Byte 6
        uint8_t byte6 = m_repitition; // Bit 0-7

        start.WriteU8(byte6);

        // Byte 7
        uint8_t byte7 = m_validity; // Bit 0-7

        start.WriteU8(byte7);
    }

    if (m_systemFrameNumberFieldIncluded)
    {
        // Byte 8
        uint8_t byte8 = m_systemFrameNumberValue; // Bit 0-7

        start.WriteU8(byte8);
    }

    if (m_channelFieldIncluded)
    {
        // Byte 9
        uint8_t byte9 = (m_channelAbsoluteCarrierCentryFrequency >> 8) & 0x1F; // Bit 3-7

        start.WriteU8(byte9);

        // Byte 10
        uint8_t byte10 = m_channelAbsoluteCarrierCentryFrequency & 0xFF; // Bit 1-7

        start.WriteU8(byte10);
    }

    if (m_separateChannelFieldIncluded)
    {
        // Byte 11
        uint8_t byte11 = (m_separateChannelAbsoluteCarrierCentryFrequency >> 8) & 0x1F; // Bit 3-7

        start.WriteU8(byte11);

        // Byte 12
        uint8_t byte12 = m_separateChannelAbsoluteCarrierCentryFrequency & 0xFF; // Bit 1-7

        start.WriteU8(byte12);
    }
}

uint32_t
Dect2020ExtRandomAccessResourceIe::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    Buffer::Iterator i = start;

    // Byte 0
    uint8_t byte0 = i.ReadU8();

    m_repeat = (byte0 >> 3) & 0x03; // Bits 3–4

    m_systemFrameNumberFieldIncluded = (byte0 & (1 << 2)) != 0; // Bit 5
    m_channelFieldIncluded = (byte0 & (1 << 1)) != 0;           // Bit 6
    m_separateChannelFieldIncluded = (byte0 & (1 << 0)) != 0;   // Bit 7

    // Byte 1 skipped (currently unused / reserved for μ ≥ 4)
    // i.Next();

    // Byte 2
    m_startSubslot = i.ReadU8();

    // Byte 3
    uint8_t byte3 = i.ReadU8();
    m_lengthType = (byte3 >> 7) & 0x01; // Bit 0
    m_raraLength = byte3 & 0x7F;        // Bits 1–7

    // Byte 4
    uint8_t byte4 = i.ReadU8();
    m_maxRachLengthType = (byte4 >> 7) & 0x01; // Bit 0
    m_maxRachLength = (byte4 >> 3) & 0x0F;     // Bits 1–4
    m_cwMin_sig = byte4 & 0x07;                // Bits 5–7

    // Byte 5
    uint8_t byte5 = i.ReadU8();
    m_dectDelay = (byte5 >> 7) & 0x01;      // Bit 0
    m_responseWindow = (byte5 >> 3) & 0x0F; // Bits 1–4
    m_cwMax_sig = byte5 & 0x07;             // Bits 5–7

    // Byte 6–7
    if (m_repeat == 1 || m_repeat == 2)
    {
        m_repitition = i.ReadU8(); // Byte 6
        m_validity = i.ReadU8();   // Byte 7
    }

    // Byte 8
    if (m_systemFrameNumberFieldIncluded)
    {
        m_systemFrameNumberValue = i.ReadU8(); // Byte 8
    }

    // Byte 9–10
    if (m_channelFieldIncluded)
    {
        uint8_t byte9 = i.ReadU8();  // Bits 3–7 relevant
        uint8_t byte10 = i.ReadU8(); // Bits 1–7

        m_channelAbsoluteCarrierCentryFrequency = ((byte9 & 0x1F) << 8) | byte10;
    }

    // Byte 11–12
    if (m_separateChannelFieldIncluded)
    {
        uint8_t byte11 = i.ReadU8(); // Bits 3–7 relevant
        uint8_t byte12 = i.ReadU8(); // Bits 1–7

        m_separateChannelAbsoluteCarrierCentryFrequency = ((byte11 & 0x1F) << 8) | byte12;
    }

    return i.GetDistanceFrom(start);
}

void
Dect2020ExtRandomAccessResourceIe::Print(std::ostream &os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "Dect2020RandomAccessResourceIE:" << std::endl;
    os << "  Repeat: " << static_cast<uint16_t>(m_repeat) << std::endl;
    os << "  System Frame Number Field Included: " << std::boolalpha
       << m_systemFrameNumberFieldIncluded << std::endl;
    os << "  Channel Field Included: " << std::boolalpha << m_channelFieldIncluded << std::endl;
    os << "  Separate Channel Field Included: " << std::boolalpha << m_separateChannelFieldIncluded
       << std::endl;

    os << "  Start Subslot: " << m_startSubslot << std::endl;
    os << "  Length Type (0=subslots, 1=slots): " << std::boolalpha << m_lengthType << std::endl;
    os << "  RARA Length: " << static_cast<uint16_t>(m_raraLength) << std::endl;

    os << "  Max RACH Length Type: " << std::boolalpha << m_maxRachLengthType << std::endl;
    os << "  Max RACH Length: " << static_cast<uint16_t>(m_maxRachLength) << std::endl;

    os << "  CWmin_sig: " << static_cast<uint16_t>(m_cwMin_sig) << std::endl;
    os << "  DECT Delay: " << std::boolalpha << m_dectDelay << std::endl;
    os << "  Response Window: " << static_cast<uint16_t>(m_responseWindow) << std::endl;
    os << "  CWmax_sig: " << static_cast<uint16_t>(m_cwMax_sig) << std::endl;

    if (m_repeat == 1 || m_repeat == 2)
    {
        os << "  Repetition: " << static_cast<uint16_t>(m_repitition) << std::endl;
        os << "  Validity: " << static_cast<uint16_t>(m_validity) << std::endl;
    }

    if (m_systemFrameNumberFieldIncluded)
    {
        os << "  System Frame Number Value: " << static_cast<uint16_t>(m_systemFrameNumberValue)
           << std::endl;
    }

    if (m_channelFieldIncluded)
    {
        os << "  Channel Absolute Carrier Center Frequency: "
           << m_channelAbsoluteCarrierCentryFrequency << std::endl;
    }

    if (m_separateChannelFieldIncluded)
    {
        os << "  Separate Channel Absolute Carrier Center Frequency: "
           << m_separateChannelAbsoluteCarrierCentryFrequency << std::endl;
    }
}

void
Dect2020ExtRandomAccessResourceIe::SetRepeat(uint8_t repeat)
{
    m_repeat = repeat & 0x03;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetRepeat() const
{
    return m_repeat;
}

void
Dect2020ExtRandomAccessResourceIe::SetSystemFrameNumberFieldIncluded(bool included)
{
    m_systemFrameNumberFieldIncluded = included;
}

bool
Dect2020ExtRandomAccessResourceIe::GetSystemFrameNumberFieldIncluded() const
{
    return m_systemFrameNumberFieldIncluded;
}

void
Dect2020ExtRandomAccessResourceIe::SetChannelFieldIncluded(bool included)
{
    m_channelFieldIncluded = included;
}

bool
Dect2020ExtRandomAccessResourceIe::GetChannelFieldIncluded() const
{
    return m_channelFieldIncluded;
}

void
Dect2020ExtRandomAccessResourceIe::SetSeparateChannelFieldIncluded(bool included)
{
    m_separateChannelFieldIncluded = included;
}

bool
Dect2020ExtRandomAccessResourceIe::GetSeparateChannelFieldIncluded() const
{
    return m_separateChannelFieldIncluded;
}

void
Dect2020ExtRandomAccessResourceIe::SetStartSubslot(uint16_t subslot)
{
    m_startSubslot = subslot;
}

uint16_t
Dect2020ExtRandomAccessResourceIe::GetStartSubslot() const
{
    return m_startSubslot;
}

void
Dect2020ExtRandomAccessResourceIe::SetLengthType(bool lengthType)
{
    m_lengthType = lengthType;
}

bool
Dect2020ExtRandomAccessResourceIe::GetLengthType() const
{
    return m_lengthType;
}

void
Dect2020ExtRandomAccessResourceIe::SetRaraLength(uint8_t length)
{
    m_raraLength = length & 0x7F;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetRaraLength() const
{
    return m_raraLength;
}

void
Dect2020ExtRandomAccessResourceIe::SetMaxRachLengthType(bool type)
{
    m_maxRachLengthType = type;
}

bool
Dect2020ExtRandomAccessResourceIe::GetMaxRachLengthType() const
{
    return m_maxRachLengthType;
}

void
Dect2020ExtRandomAccessResourceIe::SetMaxRachLength(uint8_t length)
{
    m_maxRachLength = length & 0x0F;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetMaxRachLength() const
{
    return m_maxRachLength;
}

void
Dect2020ExtRandomAccessResourceIe::SetCwMinSig(uint8_t cwMinSig)
{
    m_cwMin_sig = cwMinSig & 0x07;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetCwMinSig() const
{
    return m_cwMin_sig;
}

void
Dect2020ExtRandomAccessResourceIe::SetDectDelay(bool dectDelay)
{
    m_dectDelay = dectDelay;
}

bool
Dect2020ExtRandomAccessResourceIe::GetDectDelay() const
{
    return m_dectDelay;
}

void
Dect2020ExtRandomAccessResourceIe::SetResponseWindow(uint8_t responseWindow)
{
    m_responseWindow = responseWindow & 0x0F;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetResponseWindow() const
{
    return m_responseWindow;
}

void
Dect2020ExtRandomAccessResourceIe::SetCwMaxSig(uint8_t cwMaxSig)
{
    m_cwMax_sig = cwMaxSig & 0x07;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetCwMaxSig() const
{
    return m_cwMax_sig;
}

void
Dect2020ExtRandomAccessResourceIe::SetRepetition(uint8_t repetition)
{
    m_repitition = repetition;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetRepetition() const
{
    return m_repitition;
}

void
Dect2020ExtRandomAccessResourceIe::SetValidity(uint8_t validity)
{
    m_validity = validity;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetValidity() const
{
    return m_validity;
}

void
Dect2020ExtRandomAccessResourceIe::SetSystemFrameNumberValue(uint8_t sfn)
{
    m_systemFrameNumberValue = sfn;
}

uint8_t
Dect2020ExtRandomAccessResourceIe::GetSystemFrameNumberValue() const
{
    return m_systemFrameNumberValue;
}

void
Dect2020ExtRandomAccessResourceIe::SetChannelAbsoluteCarrierCenterFrequency(uint16_t freq)
{
    m_channelAbsoluteCarrierCentryFrequency = freq & 0x1FFF;
}

uint16_t
Dect2020ExtRandomAccessResourceIe::GetChannelAbsoluteCarrierCenterFrequency() const
{
    return m_channelAbsoluteCarrierCentryFrequency;
}

void
Dect2020ExtRandomAccessResourceIe::SetSeparateChannelAbsoluteCarrierCenterFrequency(uint16_t freq)
{
    m_separateChannelAbsoluteCarrierCentryFrequency = freq & 0x1FFF;
}

uint16_t
Dect2020ExtRandomAccessResourceIe::GetSeparateChannelAbsoluteCarrierCenterFrequency() const
{
    return m_separateChannelAbsoluteCarrierCentryFrequency;
}

} // namespace ns3 