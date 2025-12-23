#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/packet.h"
#include "serialization-test-case.h"

namespace ns3 {

Dect2020MacHeaderTypeSerializationTestCase::Dect2020MacHeaderTypeSerializationTestCase()
    : TestCase("Dect2020MacHeaderTypeSerializationTest")
{
}

Dect2020MacHeaderTypeSerializationTestCase::~Dect2020MacHeaderTypeSerializationTestCase()
{
}

void
Dect2020MacHeaderTypeSerializationTestCase::DoSetup()
{
}

void
Dect2020MacHeaderTypeSerializationTestCase::DoRun()
{
    Dect2020MacHeaderType originalHeader;
    Dect2020MacHeaderType deserializedHeader;
    Packet packet;
    uint8_t version = 0x0;

    originalHeader.SetVersion(version);
    originalHeader.SetMacSecurity(Dect2020MacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    originalHeader.SetMacHeaderTypeField(Dect2020MacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original version: 0x" << std::hex << originalHeader.GetVersion() << "\n";
    std::cout << "Original MAC security: " << std::hex << originalHeader.GetMacSecurity() << "\n";
    std::cout << "Original MAC header type field: " << std::hex << originalHeader.GetMacHeaderTypeField() << "\n" << "\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Original version: 0x" << std::hex << deserializedHeader.GetVersion() << "\n";
    std::cout << "Original MAC security: " << std::hex << deserializedHeader.GetMacSecurity() << "\n";
    std::cout << "Original MAC header type field: " << std::hex << deserializedHeader.GetMacHeaderTypeField() << "\n" << "\n";

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