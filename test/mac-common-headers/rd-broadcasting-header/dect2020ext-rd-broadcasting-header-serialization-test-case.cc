#include "dect2020ext-rd-broadcasting-header-serialization-test-case.h"
#include "ns3/dect2020ext-rd-broadcasting-header.h"

#include "ns3/packet.h"

#include <random>

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

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint8_t> resetDist(0, 1);
    std::uniform_int_distribution<uint16_t> sequenceNumberDist(0, 0xFFF);
    std::uniform_int_distribution<uint32_t> transmitterAddressDist(0, 0xFFFFFFFF);

    uint8_t reset = resetDist(mt); // 1-bit value
    uint16_t sequenceNumber = sequenceNumberDist(mt); // 12-bit value
    uint32_t transmitterAddress = transmitterAddressDist(mt); // 32-bit value

    originalHeader.SetReset(reset);
    originalHeader.SetSequenceNumber(sequenceNumber);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original reset: 0x" << std::hex << (int)originalHeader.GetReset() << "\n";
    std::cout << "Original sequence number: 0x" << std::hex << originalHeader.GetSequenceNumber() << "\n";
    std::cout << "Original transmitter address: 0x" << std::hex << originalHeader.GetTransmitterAddress() << "\n\n";

    uint32_t sizeBefore = packet.GetSize();
    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();
    std::cout << "Size after adding header: " << sizeAfter << "\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 6, "Size should be 6 bytes");

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();
    std::cout << "Size after removing header: " << sizeAfterRemove << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized reset: 0x" << std::hex << (int)deserializedHeader.GetReset() << "\n";
    std::cout << "Deserialized sequence number: 0x" << std::hex << deserializedHeader.GetSequenceNumber() << "\n";
    std::cout << "Deserialized transmitter address: 0x" << std::hex << deserializedHeader.GetTransmitterAddress() << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReset(), reset,
                            "Reset field mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(), sequenceNumber,
                            "Sequence Number mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(), transmitterAddress,
                            "Transmitter Address mismatch after serialization/deserialization");
}

} // namespace ns3