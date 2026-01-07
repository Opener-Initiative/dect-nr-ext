#include "dect2020ext-physical-header-field-serialization-test-case.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/packet.h"

namespace ns3 {

Dect2020ExtPhysicalHeaderFieldSerializationTestCase::Dect2020ExtPhysicalHeaderFieldSerializationTestCase() 
    : TestCase("Dect2020ExtPhysicalHeaderFieldSerializationTestCase")
{
}

Dect2020ExtPhysicalHeaderFieldSerializationTestCase::~Dect2020ExtPhysicalHeaderFieldSerializationTestCase()
{
}

void
Dect2020ExtPhysicalHeaderFieldSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtPhysicalHeaderFieldSerializationTestCase::DoRun()
{
    Dect2020ExtPHYControlFieldType1 originalHeader;
    Dect2020ExtPHYControlFieldType1 deserializedHeader;
    Packet packet;

    // Set test values
    uint8_t packetLengthType = 1;
    uint8_t packetLength = 0x5; // 0101 -> 5
    uint8_t shortNetworkID = 0xAF; // 1010 1111 -> 175
    uint16_t transmitterIdentity = 65535;
    uint8_t transmitPower = 0xC; // 1100 -> 12
    uint8_t DFMCS = 0x7; // 0011 -> 7

    originalHeader.SetPacketLengthType(packetLengthType);
    originalHeader.SetPacketLength(packetLength);
    originalHeader.SetShortNetworkID(shortNetworkID);
    originalHeader.SetTransmitterIdentity(transmitterIdentity);
    originalHeader.SetTransmitPower(transmitPower);
    originalHeader.SetDFMCS(DFMCS);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    packet.AddHeader(originalHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 5, "Should be 5");

    packet.RemoveHeader(deserializedHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Header format: " << (int)deserializedHeader.GetHeaderFormat() << "\n";
    std::cout << "Packet length type: " << (int)deserializedHeader.GetPacketLengthType() << "\n";
    std::cout << "Packet length: " << (int)deserializedHeader.GetPacketLength() << "\n";
    std::cout << "Short network ID: " << (int)deserializedHeader.GetShortNetworkID() << "\n";
    std::cout << "Transmitter identity: " << (int)deserializedHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Transmit power: " << (int)deserializedHeader.GetTransmitPower() << "\n";
    std::cout << "DF MCS: " << (int)deserializedHeader.GetDFMCS() << "\n";

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
                         DFMCS,
                         "DF MCS mismatch after serialization/deserialization");
}

} // namespace ns3 