#include "dect2020-beacon-header-test-case.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/packet.h"

namespace ns3 {

Dect2020BeaconHeaderTestCase::Dect2020BeaconHeaderTestCase() : TestCase("BeaconHeaderSerializationTest")
{
}

Dect2020BeaconHeaderTestCase::~Dect2020BeaconHeaderTestCase()
{
}

void
Dect2020BeaconHeaderTestCase::DoSetup()
{
}

void
Dect2020BeaconHeaderTestCase::DoRun()
{
    Dect2020BeaconHeader originalHeader;
    Dect2020BeaconHeader deserializedHeader;
    Packet packet;
    uint32_t networkId = 0xBCE68F; // 24-bit ID
    uint32_t transmitterAddress = 0xA0120000; // 32-bit address

    originalHeader.SetNetworkId(networkId);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    // Print original values for debugging
    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original network ID: 0x" << std::hex << originalHeader.GetNetworkId() << "\n";
    std::cout << "Original transmitter address: 0x" << std::hex << originalHeader.GetTransmitterAddress() << "\n" << "\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    // Print deserialized values for debugging
    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized network ID: 0x" << std::hex << deserializedHeader.GetNetworkId() << "\n";
    std::cout << "Deserialized transmitter address: 0x" << std::hex << deserializedHeader.GetTransmitterAddress();

    // Test network ID
    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkId(), 
                         networkId,
                         "Network ID mismatch after serialization/deserialization");

    // Test transmitter address
    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterAddress(), 
                         transmitterAddress,
                         "Transmitter address mismatch after serialization/deserialization");
}

} // namespace ns3