#include "dect2020ext-rd-broadcasting-header-serialization-test-case.h"
#include "ns3/dect2020ext-rd-broadcasting-header.h"
#include "ns3/packet.h"

namespace ns3 {

Dect2020ExtRdBroadcastingHeaderSerializationTestCase::Dect2020ExtRdBroadcastingHeaderSerializationTestCase()
    : TestCase("Dect2020ExtRdBroadcastingHeaderSerializationTestCase")
{
}

Dect2020ExtRdBroadcastingHeaderSerializationTestCase::~Dect2020ExtRdBroadcastingHeaderSerializationTestCase()
{
}

void Dect2020ExtRdBroadcastingHeaderSerializationTestCase::DoSetup()
{
}

void Dect2020ExtRdBroadcastingHeaderSerializationTestCase::DoRun()
{
    Dect2020ExtRdBroadcastingHeader originalHeader;
    Dect2020ExtRdBroadcastingHeader deserializedHeader;
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