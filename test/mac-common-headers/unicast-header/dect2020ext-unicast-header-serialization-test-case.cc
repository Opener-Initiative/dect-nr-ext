#include "dect2020ext-unicast-header-serialization-test-case.h"
#include "ns3/dect2020ext-unicast-header.h"
#include "ns3/packet.h"

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
    uint8_t macSequence = 0xF;
    uint8_t sequenceNumber = 0xAB;
    uint32_t receiverAddress = 0xFBFED010;
    uint32_t transmitterAddress = 0xABCDEF01;

    originalHeader.SetMacSequence(macSequence);
    originalHeader.SetSequenceNumber(sequenceNumber);
    originalHeader.SetReceiverAddress(receiverAddress);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original MAC sequence: 0x" << std::hex << (int)originalHeader.GetMacSequence() << "\n";
    std::cout << "Original sequence number: 0x" << std::hex << (int)originalHeader.GetSequenceNumber() << "\n";
    std::cout << "Original receiver address: 0x" << std::hex << originalHeader.GetReceiverAddress() << "\n";
    std::cout << "Original transmitter address: 0x" << std::hex << originalHeader.GetTransmitterAddress() << "\n\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized MAC sequence: 0x" << std::hex << (int)deserializedHeader.GetMacSequence() << "\n";
    std::cout << "Deserialized sequence number: 0x" << std::hex << (int)deserializedHeader.GetSequenceNumber() << "\n";
    std::cout << "Deserialized receiver address: 0x" << std::hex << deserializedHeader.GetReceiverAddress() << "\n";
    std::cout << "Deserialized transmitter address: 0x" << std::hex << deserializedHeader.GetTransmitterAddress();

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacSequence(), 
                        macSequence,
                        "MAC sequence mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSequenceNumber(), 
                        sequenceNumber,
                        "Sequence number mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReceiverAddress(), 
                        receiverAddress,
                        "Receiver address mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(), 
                        transmitterAddress,
                        "Transmitter address mismatch after serialization/deserialization");
}

} // namespace ns3 