#include "dect2020ext-beacon-header-serialization-test-case.h"
#include "ns3/dect2020ext-beacon-header.h"

#include "ns3/packet.h"

#include <random>

namespace ns3 {

Dect2020ExtBeaconHeaderSerializationTestCase::Dect2020ExtBeaconHeaderSerializationTestCase()
    : TestCase("Dect2020ExtBeaconHeaderSerializationTestCase")
{
}

Dect2020ExtBeaconHeaderSerializationTestCase::~Dect2020ExtBeaconHeaderSerializationTestCase()
{
}

void Dect2020ExtBeaconHeaderSerializationTestCase::DoSetup()
{
}

void Dect2020ExtBeaconHeaderSerializationTestCase::DoRun()
{
    Dect2020ExtBeaconHeader originalHeader;
    Dect2020ExtBeaconHeader deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint32_t> networkIdDist(0, 0xFFFFFF);
    std::uniform_int_distribution<uint32_t> transmitterAddressDist(0, 0xFFFFFFFF);

    uint32_t networkId = networkIdDist(mt); // 24-bit ID
    uint32_t transmitterAddress = transmitterAddressDist(mt); // 32-bit address

    originalHeader.SetNetworkId(networkId);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original network ID: 0x" << std::hex << originalHeader.GetNetworkId() << "\n";
    std::cout << "Original transmitter address: 0x" << std::hex << originalHeader.GetTransmitterAddress() << "\n" << "\n";

    uint32_t sizeBefore = packet.GetSize();
    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();
    std::cout << "Size after adding header: " << sizeAfter << "\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 7, "Size should be 7 bytes");

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();
    std::cout << "Size after removing header: " << sizeAfterRemove << "\n" << "\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized network ID: 0x" << std::hex << deserializedHeader.GetNetworkId() << "\n";
    std::cout << "Deserialized transmitter address: 0x" << std::hex << deserializedHeader.GetTransmitterAddress();

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkId(), networkId,
                            "Network ID mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(), transmitterAddress,
                            "Transmitter address mismatch after serialization/deserialization");
}

} // namespace ns3