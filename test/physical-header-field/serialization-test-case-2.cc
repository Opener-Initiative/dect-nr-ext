#include "serialization-test-case-2.h"
#include "ns3/dect2020-physical-header-field.h"
#include "ns3/packet.h"

namespace ns3 {

Dect2020PhysicalHeaderFieldSerializationTestCase2::Dect2020PhysicalHeaderFieldSerializationTestCase2() 
    : TestCase("PhysicalHeaderFieldSerializationTest")
{
}

Dect2020PhysicalHeaderFieldSerializationTestCase2::~Dect2020PhysicalHeaderFieldSerializationTestCase2()
{
}

void
Dect2020PhysicalHeaderFieldSerializationTestCase2::DoSetup()
{
}

void
Dect2020PhysicalHeaderFieldSerializationTestCase2::DoRun()
{
    Dect2020PhysicalHeaderField originalHeader;
    Dect2020PhysicalHeaderField deserializedHeader;
    Packet packet;

    // Set test values
    uint8_t packetLengthType = 1;
    uint8_t packetLength = 0xF;
    uint8_t shortNetworkID = 0xAF;
    uint16_t transmitterIdentity = 0xFFFF;
    uint8_t transmitPower = 0x0F;
    uint8_t dFMCS = 0x1;

    originalHeader.SetPacketLengthType(packetLengthType);
    originalHeader.SetPacketLength(packetLength);
    originalHeader.SetShortNetworkID(shortNetworkID);
    originalHeader.SetTransmitterIdentity(transmitterIdentity);
    originalHeader.SetTransmitPower(transmitPower);
    originalHeader.SetDFMCS(dFMCS);
    originalHeader.SetType(Dect2020PhysicalHeaderField::Type::TYPE2);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original packet length type: 0x" << std::hex << (int)originalHeader.GetPacketLengthType() << "\n";
    std::cout << "Original packet length: 0x" << std::hex << (int)originalHeader.GetPacketLength() << "\n";
    std::cout << "Original short network ID: 0x" << std::hex << (int)originalHeader.GetShortNetworkID() << "\n";
    std::cout << "Original transmitter identity: 0x" << std::hex << originalHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Original transmit power: 0x" << std::hex << (int)originalHeader.GetTransmitPower() << "\n";
    std::cout << "Original DF MCS: 0x" << std::hex << (int)originalHeader.GetDFMCS() << "\n\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized packet length type: 0x" << std::hex << (int)deserializedHeader.GetPacketLengthType() << "\n";
    std::cout << "Deserialized packet length: 0x" << std::hex << (int)deserializedHeader.GetPacketLength() << "\n";
    std::cout << "Deserialized short network ID: 0x" << std::hex << (int)deserializedHeader.GetShortNetworkID() << "\n";
    std::cout << "Deserialized transmitter identity: 0x" << std::hex << deserializedHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Deserialized transmit power: 0x" << std::hex << (int)deserializedHeader.GetTransmitPower() << "\n";
    std::cout << "Deserialized DF MCS: 0x" << std::hex << (int)deserializedHeader.GetDFMCS() << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLengthType(), 
                         packetLengthType,
                         "Packet length type mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLength(), 
                         packetLength,
                         "Packet length mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetShortNetworkID(), 
                         shortNetworkID,
                         "Short network ID mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterIdentity(), 
                         transmitterIdentity,
                         "Transmitter identity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitPower(), 
                         transmitPower,
                         "Transmit power mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDFMCS(), 
                         dFMCS,
                         "DF MCS mismatch after serialization/deserialization");
}

} // namespace ns3 