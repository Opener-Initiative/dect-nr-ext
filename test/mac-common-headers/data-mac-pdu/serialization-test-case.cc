#include "serialization-test-case.h"
#include "ns3/dect2020-data-mac-pdu-header.h"
#include "ns3/packet.h"

namespace ns3
{

Dect2020DataMacPduSerializationTestCase::Dect2020DataMacPduSerializationTestCase()
    : TestCase("Dect2020DataMacPduHeaderSerializationTestCase")
{
}

Dect2020DataMacPduSerializationTestCase::~Dect2020DataMacPduSerializationTestCase()
{
}

void
Dect2020DataMacPduSerializationTestCase::DoSetup()
{
}

void
Dect2020DataMacPduSerializationTestCase::DoRun()
{
    Dect2020DataMacPduHeader originalHeader;
    Dect2020DataMacPduHeader deserializedHeader;
    Packet packet;

    // Set test values
    uint8_t reset = 0x1;
    uint16_t sequenceNumber = 0xFFF; // 12-bit value

    originalHeader.SetReset(reset);
    originalHeader.SetSequenceNumber(sequenceNumber);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original reset: 0x" << std::hex << (int)originalHeader.GetReset() << "\n";
    std::cout << "Original sequence number: 0x" << std::hex << originalHeader.GetSequenceNumber() << "\n\n";

    // Perform serialization/deserialization
    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized reset: 0x" << std::hex << (int)deserializedHeader.GetReset() << "\n";
    std::cout << "Deserialized sequence number: 0x" << std::hex << deserializedHeader.GetSequenceNumber();

    // Test reset field
    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReset(),
                        reset,
                        "Reset field mismatch after serialization/deserialization");

    // Test sequence number field
    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(),
                        sequenceNumber,
                        "Sequence number mismatch after serialization/deserialization");
}

} // namespace ns3