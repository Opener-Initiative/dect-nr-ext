#include "dect2020ext-mac-multiplexing-header-type-a-serialization-test-case.h"

#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"

#include "ns3/packet.h"

#include <array>
#include <random>

namespace ns3
{

namespace
{

constexpr std::array<Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding, 5> kValidIeTypeFieldEncodingValues = {
    Dect2020ExtMacMultiplexingHeaderTypeA::NETWORK_BEACON_MESSAGE,
    Dect2020ExtMacMultiplexingHeaderTypeA::CLUSTER_BEACON_MESSAGE,
    Dect2020ExtMacMultiplexingHeaderTypeA::ASSOCIATION_REQUEST_MESSAGE,
    Dect2020ExtMacMultiplexingHeaderTypeA::ASSOCIATION_RESPONSE_MESSAGE,
    Dect2020ExtMacMultiplexingHeaderTypeA::RANDOM_ACCESS_RESOURCE_IE
};

} // namespace

Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase()
    : TestCase("Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase")
{
}

Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::~Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase()
{
}

void
Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::DoSetup()
{
}

void
Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::DoRun()
{
    Dect2020ExtMacMultiplexingHeaderTypeA originalHeader;
    Dect2020ExtMacMultiplexingHeaderTypeA deserializedHeader;
    Ptr<Packet> packet = Create<Packet>();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<unsigned int> macExtensionFieldEncodingDist(0, 3);
    std::bernoulli_distribution lengthFieldDist(0.5);
    std::uniform_int_distribution<size_t> ieTypeFieldEncodingIdxDist(0, kValidIeTypeFieldEncodingValues.size() - 1);

    auto macExtensionFieldEncoding = static_cast<uint8_t>(macExtensionFieldEncodingDist(mt));
    bool lengthField = lengthFieldDist(mt);
    auto ieTypeFieldEncoding = kValidIeTypeFieldEncodingValues[ieTypeFieldEncodingIdxDist(mt)];

    originalHeader.SetMacExtensionFieldEncoding(macExtensionFieldEncoding);
    originalHeader.SetLengthField(lengthField);
    originalHeader.SetIeTypeFieldEncoding(ieTypeFieldEncoding);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original MAC Extension Field Encoding " << (int)originalHeader.GetMacExtensionFieldEncoding() << "\n";
    std::cout << "Original length field " << (int)originalHeader.GetLengthField() << "\n";
    std::cout << "Original IE Type Field Encoding " << (int)originalHeader.GetIeTypeFieldEncoding() << "\n\n";

    uint32_t sizeBefore = packet->GetSize();
    std::cout << "Packet size before adding header: " << sizeBefore << "\n";

    packet->AddHeader(originalHeader);
    uint32_t sizeAfter = packet->GetSize();
    std::cout << "Packet size after adding header: " << sizeAfter << "\n\n";
    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 1, "After adding header packet size should be 1");

    packet->RemoveHeader(deserializedHeader);
    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "After removing header packet size should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized MAC Extension Field Encoding " << (int)deserializedHeader.GetMacExtensionFieldEncoding() << "\n";
    std::cout << "Deserialized length field " << (int)deserializedHeader.GetLengthField() << "\n";
    std::cout << "Deserialized IE Type Field Encoding " << (int)deserializedHeader.GetIeTypeFieldEncoding();

    NS_TEST_ASSERT_MSG_EQ(originalHeader.GetMacExtensionFieldEncoding(),
                          deserializedHeader.GetMacExtensionFieldEncoding(),
                          "Mismatch between original and deserialized MAC Extension Field Encoding");

    NS_TEST_ASSERT_MSG_EQ(originalHeader.GetLengthField(),
                          deserializedHeader.GetLengthField(),
                          "Mismatch between original and deserialized length field");

    NS_TEST_ASSERT_MSG_EQ(originalHeader.GetIeTypeFieldEncoding(),
                          deserializedHeader.GetIeTypeFieldEncoding(),
                          "Mismatch between original and deserialized IE Type Field Encoding");
}

} // namespace ns3