#include "dect2020ext-mac-header-type-serialization-test-case.h"

#include "ns3/dect2020ext-mac-header-type.h"

#include "ns3/packet.h"

#include <array>
#include <random>

namespace ns3
{

namespace
{

constexpr std::array<Dect2020ExtMacHeaderType::MacSecurityField, 4> kValidMacSecurityValues = {
    Dect2020ExtMacHeaderType::MAC_SECURITY_NOT_USED,
    Dect2020ExtMacHeaderType::MAC_SECURITY_USED_NO_IE,
    Dect2020ExtMacHeaderType::MAC_SECURITY_USED_WITH_IE,
    Dect2020ExtMacHeaderType::RESERVED};

constexpr std::array<Dect2020ExtMacHeaderType::MacHeaderTypeField, 5> kValidMacHeaderTypeValues = {
    Dect2020ExtMacHeaderType::DATA_MAC_PDU_HEADER,
    Dect2020ExtMacHeaderType::BEACON_HEADER,
    Dect2020ExtMacHeaderType::UNICAST_HEADER,
    Dect2020ExtMacHeaderType::RD_BROADCASTING_HEADER,
    Dect2020ExtMacHeaderType::ESCAPE};

} // namespace

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

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> macSecurityIdxDist(0, kValidMacSecurityValues.size() - 1);
    std::uniform_int_distribution<size_t> macHeaderTypeIdxDist(0, kValidMacHeaderTypeValues.size() - 1);

    uint8_t version = 0;
    auto macSecurity = kValidMacSecurityValues[macSecurityIdxDist(mt)];
    auto macHeaderType = kValidMacHeaderTypeValues[macHeaderTypeIdxDist(mt)];

    originalHeader.SetVersion(version);
    originalHeader.SetMacSecurity(macSecurity);
    originalHeader.SetMacHeaderTypeField(macHeaderType);

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