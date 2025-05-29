#include "serialization-test-case.h"
#include "ns3/dect2020-rd-broadcasting-header.h"
#include "ns3/packet.h"

namespace ns3 {

SerializationTestCase::SerializationTestCase()
    : TestCase("Dect2020RDBroadcastingHeaderSerializationTestCase")
{
}

SerializationTestCase::~SerializationTestCase()
{
}

void SerializationTestCase::DoSetup()
{
}

void SerializationTestCase::DoRun()
{
    Dect2020RDBroadcastingHeader originalHeader;
    Dect2020RDBroadcastingHeader deserializedHeader;
    Packet packet;

    // Set test values
    uint8_t reset = 0x1;
    uint16_t sequenceNumber = 0xFFF;
    uint32_t transmitterAddress = 0xFFFFFFFF;

    originalHeader.SetReset(reset);
    originalHeader.SetSequenceNumber(sequenceNumber);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    originalHeader.Print(std::cout);

    // Perform serialization/deserialization
    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    deserializedHeader.Print(std::cout);

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReset(),
                        reset,
                        "Reset mismatch");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(),
                        sequenceNumber,
                        "Sequence Number mismatch");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(),
                        transmitterAddress,
                        "Transmitter Address mismatch");
}

} // namespace ns3