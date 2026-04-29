#include "dect2020ext-data-mac-pdu-serialization-test-case.h"

#include "ns3/dect2020ext-data-mac-pdu-header.h"

#include "ns3/packet.h"

#include <random>

namespace ns3
{

Dect2020ExtDataMacPduSerializationTestCase::Dect2020ExtDataMacPduSerializationTestCase()
    : TestCase("Dect2020ExtDataMacPduHeaderSerializationTestCase")
{
}

Dect2020ExtDataMacPduSerializationTestCase::~Dect2020ExtDataMacPduSerializationTestCase()
{
}

void
Dect2020ExtDataMacPduSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtDataMacPduSerializationTestCase::DoRun()
{
    Dect2020ExtDataMacPduHeader originalHeader;
    Dect2020ExtDataMacPduHeader deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::bernoulli_distribution resetDist(0.5);
    std::uniform_int_distribution<unsigned int> sequenceNumberDist(0, 0xFFF);

    bool reset = resetDist(mt); // 1-bit value
    auto sequenceNumber = static_cast<uint16_t>(sequenceNumberDist(mt)); // 12-bit value

    originalHeader.SetReset(reset);
    originalHeader.SetSequenceNumber(sequenceNumber);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original reset: 0x" << std::hex << (int)originalHeader.GetReset() << "\n";
    std::cout << "Original sequence number: 0x" << std::hex << originalHeader.GetSequenceNumber() << "\n\n";

    uint32_t sizeBefore = packet.GetSize();
    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();
    std::cout << "Size after adding header: " << sizeAfter << "\n";
    
    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 2, "Size should be 2 bytes");

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();
    std::cout << "Size after removing header: " << sizeAfterRemove << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized reset: 0x" << std::hex << (int)deserializedHeader.GetReset() << "\n";
    std::cout << "Deserialized sequence number: 0x" << std::hex << deserializedHeader.GetSequenceNumber();

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReset(), reset,
                            "Reset field mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(), sequenceNumber,
                            "Sequence number mismatch after serialization/deserialization");
}

} // namespace ns3