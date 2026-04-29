#include "dect2020ext-unicast-header-serialization-test-case.h"

#include "ns3/dect2020ext-unicast-header.h"

#include "ns3/packet.h"

#include <random>

namespace ns3 {

Dect2020ExtUnicastHeaderSerializationTestCase::Dect2020ExtUnicastHeaderSerializationTestCase()
    : TestCase("Dect2020ExtUnicastHeaderSerializationTest")
{
}

Dect2020ExtUnicastHeaderSerializationTestCase::~Dect2020ExtUnicastHeaderSerializationTestCase()
{
}

void Dect2020ExtUnicastHeaderSerializationTestCase::DoSetup()
{
}

void Dect2020ExtUnicastHeaderSerializationTestCase::DoRun()
{
    Dect2020ExtUnicastHeader originalHeader;
    Dect2020ExtUnicastHeader deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::bernoulli_distribution resetDist(0.5);
    std::uniform_int_distribution<unsigned int> macSequenceDist(0, 0xF);
    std::uniform_int_distribution<unsigned int> sequenceNumberDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> receiverAddressDist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<unsigned int> transmitterAddressDist(0, 0xFFFFFFFF);

    bool reset = resetDist(mt); // 1-bit value
    auto macSequence = static_cast<uint8_t>(macSequenceDist(mt)); // 4-bit value
    auto sequenceNumber = static_cast<uint8_t>(sequenceNumberDist(mt)); // 8-bit value
    auto receiverAddress = static_cast<uint32_t>(receiverAddressDist(mt)); // 32-bit value
    auto transmitterAddress = static_cast<uint32_t>(transmitterAddressDist(mt)); // 32-bit value

    originalHeader.SetReset(reset);
    originalHeader.SetMacSequence(macSequence);
    originalHeader.SetSequenceNumber(sequenceNumber);
    originalHeader.SetReceiverAddress(receiverAddress);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original reset: 0x" << std::hex << (int)originalHeader.GetReset() << "\n";
    std::cout << "Original MAC sequence: 0x" << std::hex << (int)originalHeader.GetMacSequence() << "\n";
    std::cout << "Original sequence number: 0x" << std::hex << (int)originalHeader.GetSequenceNumber() << "\n";
    std::cout << "Original receiver address: 0x" << std::hex << originalHeader.GetReceiverAddress() << "\n";
    std::cout << "Original transmitter address: 0x" << std::hex << originalHeader.GetTransmitterAddress() << "\n\n";

    uint32_t sizeBefore = packet.GetSize();
    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();
    std::cout << "Size after adding header: " << sizeAfter << "\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 10, "Size should be 10 bytes");

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();
    std::cout << "Size after removing header: " << sizeAfterRemove << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized reset: 0x" << std::hex << (int)deserializedHeader.GetReset() << "\n";
    std::cout << "Deserialized MAC sequence: 0x" << std::hex << (int)deserializedHeader.GetMacSequence() << "\n";
    std::cout << "Deserialized sequence number: 0x" << std::hex << (int)deserializedHeader.GetSequenceNumber() << "\n";
    std::cout << "Deserialized receiver address: 0x" << std::hex << deserializedHeader.GetReceiverAddress() << "\n";
    std::cout << "Deserialized transmitter address: 0x" << std::hex << deserializedHeader.GetTransmitterAddress();

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReset(), reset,
                            "Reset mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacSequence(), macSequence,
                            "MAC sequence mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(), sequenceNumber,
                            "Sequence number mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReceiverAddress(), receiverAddress,
                            "Receiver address mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(), transmitterAddress,
                            "Transmitter address mismatch after serialization/deserialization");
}

} // namespace ns3 