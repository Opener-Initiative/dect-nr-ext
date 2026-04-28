#include "dect2020ext-mac-header-type-serialization-test-case.h"

#include "ns3/dect2020ext-mac-header-type.h"

#include "ns3/packet.h"

namespace ns3
{

Dect2020ExtMacHeaderTypeSerializationTestCase::Dect2020ExtMacHeaderTypeSerializationTestCase()
    : TestCase("Dect2020ExtMacHeaderTypeSerializationTestCase")
{
}

Dect2020ExtMacHeaderTypeSerializationTestCase::~Dect2020ExtMacHeaderTypeSerializationTestCase()
{
}

void
Dect2020ExtMacHeaderTypeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtMacHeaderTypeSerializationTestCase::DoRun()
{
    Dect2020ExtMacHeaderType originalHeader;
    Dect2020ExtMacHeaderType deserializedHeader;
    Packet packet;
    uint8_t version = 0x1;

    originalHeader.SetVersion(version);
    originalHeader.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    originalHeader.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original version: 0x" << std::hex << originalHeader.GetVersion() << "\n";
    std::cout << "Original MAC security: " << std::hex << originalHeader.GetMacSecurity() << "\n";
    std::cout << "Original MAC header type field: " << std::hex << originalHeader.GetMacHeaderTypeField() << "\n" << "\n";

    uint32_t sizeBefore = packet.GetSize();

    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);

    uint32_t sizeAfter = packet.GetSize();

    std::cout << "Size after adding header: " << sizeAfter << "\n";

    packet.RemoveHeader(deserializedHeader);

    uint32_t sizeAfterDeserialization = packet.GetSize();

    std::cout << "Size after deserialization: " << sizeAfterDeserialization << "\n" << "\n";

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Original version: 0x" << std::hex << deserializedHeader.GetVersion() << "\n";
    std::cout << "Original MAC security: " << std::hex << deserializedHeader.GetMacSecurity() << "\n";
    std::cout << "Original MAC header type field: " << std::hex << deserializedHeader.GetMacHeaderTypeField();

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetVersion(), 
                        originalHeader.GetVersion(),
                        "Version mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacSecurity(), 
                        originalHeader.GetMacSecurity(),
                        "MAC security mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacHeaderTypeField(), 
                        originalHeader.GetMacHeaderTypeField(),
                        "MAC header type field mismatch after serialization/deserialization");
}

} // namespace ns3