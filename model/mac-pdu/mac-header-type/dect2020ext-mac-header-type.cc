#include "dect2020ext-mac-header-type.h"

#include "ns3/dect2020ext-mac-pdu-defs.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacHeaderType");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacHeaderType);

Dect2020ExtMacHeaderType::Dect2020ExtMacHeaderType()
{
}

Dect2020ExtMacHeaderType::~Dect2020ExtMacHeaderType()
{
}

TypeId
Dect2020ExtMacHeaderType::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacHeaderType")
                            .SetParent<Header>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtMacHeaderType>();
    return tid;
}

TypeId
Dect2020ExtMacHeaderType::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020ExtMacHeaderType::Print(std::ostream& os) const
{
    os << "Beacon Header Type:" << std::endl << "Version = " << static_cast<int>(m_version) << std::endl
       << "MAC Security = " << static_cast<int>(m_macSecurity) << std::endl
       << "MAC Header Type = " << static_cast<int>(m_macHeaderTypeField) << std::endl;
}

uint32_t
Dect2020ExtMacHeaderType::GetSerializedSize() const
{
    return 1;
}

void
Dect2020ExtMacHeaderType::Serialize(Buffer::Iterator start) const
{
    // byte 0
    uint8_t byte0 = 0;

    byte0 |= (m_version & 0x03) << 6;       // Bit 0-1
    byte0 |= (m_macSecurity & 0x03) << 4;   // Bit 2-3
    byte0 |= (m_macHeaderTypeField & 0x0F); // Bit 4-7

    start.WriteU8(byte0);
}

uint32_t
Dect2020ExtMacHeaderType::Deserialize(Buffer::Iterator start)
{
    // byte 0
    uint8_t bytesRead = 0;
    uint8_t byte0 = 0;

    byte0 = start.ReadU8();
    bytesRead++;

    m_version = (byte0 >> 6) & 0x03;
    m_macSecurity = static_cast<MacSecurityField>((byte0 >> 4) & 0x03);
    m_macHeaderTypeField = static_cast<MacHeaderTypeField>(byte0 & 0x0F);

    return bytesRead;
}

void
Dect2020ExtMacHeaderType::SetVersion(uint8_t version)
{
    if (version != 0)
    {
        DECT2020EXT_MAC_PDU_FATAL("Version must be 0", static_cast<uint32_t>(version));
    }

    m_version = version;
}

uint8_t
Dect2020ExtMacHeaderType::GetVersion() const
{
    return m_version;
}

void
Dect2020ExtMacHeaderType::SetMacSecurity(MacSecurityField macSecurity)
{
    if ((macSecurity < 0) || (macSecurity > 3))
    {
        DECT2020EXT_MAC_PDU_FATAL("Mac Security must be between 0 and 3", static_cast<uint32_t>(macSecurity));
    }

    m_macSecurity = macSecurity;
}

ns3::Dect2020ExtMacHeaderType::MacSecurityField
Dect2020ExtMacHeaderType::GetMacSecurity() const
{
    return m_macSecurity;
}

void
Dect2020ExtMacHeaderType::SetMacHeaderTypeField(MacHeaderTypeField macHeaderTypeField)
{
    if ((macHeaderTypeField < 0) || (macHeaderTypeField > 15))
    {
        DECT2020EXT_MAC_PDU_FATAL("Mac Header Type Field must be between 0 and 15", static_cast<uint32_t>(macHeaderTypeField));
    }

    m_macHeaderTypeField = macHeaderTypeField;
}

ns3::Dect2020ExtMacHeaderType::MacHeaderTypeField
Dect2020ExtMacHeaderType::GetMacHeaderTypeField() const
{
    return m_macHeaderTypeField;
}

} // namespace ns3