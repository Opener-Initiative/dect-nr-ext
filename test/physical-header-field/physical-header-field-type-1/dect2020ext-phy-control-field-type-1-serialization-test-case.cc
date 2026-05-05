#include "dect2020ext-phy-control-field-type-1-serialization-test-case.h"

#include "ns3/dect2020ext-phy-control-field-type-1.h"

#include "ns3/packet.h"

#include <random>

namespace ns3 {

Dect2020ExtPhyControlFieldType1SerializationTestCase::Dect2020ExtPhyControlFieldType1SerializationTestCase() 
    : TestCase("Dect2020ExtPhyControlFieldType1SerializationTestCase")
{
}

Dect2020ExtPhyControlFieldType1SerializationTestCase::~Dect2020ExtPhyControlFieldType1SerializationTestCase()
{
}

void
Dect2020ExtPhyControlFieldType1SerializationTestCase::DoSetup()
{
}

void
Dect2020ExtPhyControlFieldType1SerializationTestCase::DoRun()
{
    Dect2020ExtPhyControlFieldType1 originalHeader;
    Dect2020ExtPhyControlFieldType1 deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::bernoulli_distribution packetLengthTypeDist(0.5);
    std::uniform_int_distribution<unsigned int> packetLengthDist(0, 15);
    std::uniform_int_distribution<unsigned int> shortNetworkIdDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> transmitterIdentityDist(0, 0xFFFF);
    std::uniform_int_distribution<unsigned int> transmitPowerDist(0, 15);
    std::uniform_int_distribution<unsigned int> dfmcsDist(0, 7);

    bool packetLengthType = packetLengthTypeDist(mt);
    auto packetLength = static_cast<uint8_t>(packetLengthDist(mt));
    auto shortNetworkId = static_cast<uint8_t>(shortNetworkIdDist(mt));
    auto transmitterIdentity = static_cast<uint16_t>(transmitterIdentityDist(mt));
    auto transmitPower = static_cast<uint8_t>(transmitPowerDist(mt));
    auto dfmcs = static_cast<uint8_t>(dfmcsDist(mt));

    originalHeader.SetPacketLengthType(packetLengthType);
    originalHeader.SetPacketLength(packetLength);
    originalHeader.SetShortNetworkId(shortNetworkId);
    originalHeader.SetTransmitterIdentity(transmitterIdentity);
    originalHeader.SetTransmitPower(transmitPower);
    originalHeader.SetDFMCS(dfmcs);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Header format: " << (int)originalHeader.GetHeaderFormat() << "\n";
    std::cout << "Packet length type: " << (int)originalHeader.GetPacketLengthType() << "\n";
    std::cout << "Packet length: " << (int)originalHeader.GetPacketLength() << "\n";
    std::cout << "Short network ID: " << (int)originalHeader.GetShortNetworkId() << "\n";
    std::cout << "Transmitter identity: " << (int)originalHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Transmit power: " << (int)originalHeader.GetTransmitPower() << "\n";
    std::cout << "DF MCS: " << (int)originalHeader.GetDFMCS() << "\n";

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    packet.AddHeader(originalHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 5, "Should be 5");

    packet.RemoveHeader(deserializedHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Header format: " << (int)deserializedHeader.GetHeaderFormat() << "\n";
    std::cout << "Packet length type: " << (int)deserializedHeader.GetPacketLengthType() << "\n";
    std::cout << "Packet length: " << (int)deserializedHeader.GetPacketLength() << "\n";
    std::cout << "Short network ID: " << (int)deserializedHeader.GetShortNetworkId() << "\n";
    std::cout << "Transmitter identity: " << (int)deserializedHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Transmit power: " << (int)deserializedHeader.GetTransmitPower() << "\n";
    std::cout << "DF MCS: " << (int)deserializedHeader.GetDFMCS() << "\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHeaderFormat(), 0,
                            "Packet length type mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLengthType(), packetLengthType,
                            "Packet length type mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLength(), packetLength,
                            "Packet length mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetShortNetworkId(), shortNetworkId,
                            "Short network ID mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterIdentity(), transmitterIdentity,
                            "Transmitter identity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitPower(), transmitPower,
                            "Transmit power mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDFMCS(), dfmcs,
                            "DF MCS mismatch after serialization/deserialization");
}

} // namespace ns3 