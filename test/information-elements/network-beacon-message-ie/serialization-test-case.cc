#include "serialization-test-case.h"
#include "ns3/dect2020-network-beacon-message-ie.h"
#include "ns3/packet.h"
#include "ns3/test.h"

namespace ns3 {

Dect2020NetworkBeaconMessageIESerializationTestCase::Dect2020NetworkBeaconMessageIESerializationTestCase() : TestCase("NetworkBeaconMessageIESerializationTest")
{
}

Dect2020NetworkBeaconMessageIESerializationTestCase::~Dect2020NetworkBeaconMessageIESerializationTestCase()
{
}

void
Dect2020NetworkBeaconMessageIESerializationTestCase::DoSetup()
{
}

void
Dect2020NetworkBeaconMessageIESerializationTestCase::DoRun()
{
    Dect2020NetworkBeaconMessageIE originalHeader;
    Dect2020NetworkBeaconMessageIE deserializedHeader;
    Packet packet;

    originalHeader.SetTxPowerIncluded(true);
    originalHeader.SetPowerConstraints(true);
    originalHeader.SetCurrentClusterChannelIncluded(true);
    originalHeader.SetNetworkBeaconChannels(0x3);
    originalHeader.SetNetworkBeaconPeriod(Dect2020NetworkBeaconMessageIE::NetworkBeaconPeriod::NETWORK_PERIOD_1000MS);
    originalHeader.SetClusterBeaconPeriod(Dect2020NetworkBeaconMessageIE::ClusterBeaconPeriod::CLUSTER_PERIOD_50MS);
    originalHeader.SetNextClusterChannel(0x1234);
    originalHeader.SetTimeToNext(0xABCDEF01);
    originalHeader.SetClustersMaxTxPower(0xF);
    originalHeader.SetCurrentClusterChannel(0x561);
    
    uint16_t additionalChannels[3] = {0x11, 0x22, 0x33};
    originalHeader.SetAdditionalNetworkBeaconChannels(additionalChannels);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "TxPowerIncluded: " << originalHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << originalHeader.GetPowerConstraints() << "\n";
    std::cout << "CurrentClusterChannelIncluded: " << originalHeader.GetCurrentClusterChannelIncluded() << "\n";
    std::cout << "NetworkBeaconChannels: 0x" << std::hex << (int)originalHeader.GetNetworkBeaconChannels() << "\n";
    std::cout << "NetworkBeaconPeriod: " << (int)originalHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << (int)originalHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "NextClusterChannel: 0x" << std::hex << originalHeader.GetNextClusterChannel() << "\n";
    std::cout << "TimeToNext: 0x" << std::hex << originalHeader.GetTimeToNext() << "\n";
    std::cout << "ClustersMaxTxPower: 0x" << std::hex << (int)originalHeader.GetClustersMaxTxPower() << "\n";
    std::cout << "CurrentClusterChannel: 0x" << std::hex << originalHeader.GetCurrentClusterChannel() << "\n";
    
    uint16_t* origAdditionalChannels = originalHeader.GetAdditionalNetworkBeaconChannels();
    std::cout << "AdditionalNetworkBeaconChannels: [0x" << std::hex << origAdditionalChannels[0] 
              << ", 0x" << origAdditionalChannels[1] 
              << ", 0x" << origAdditionalChannels[2] << "]" << "\n" << "\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "TxPowerIncluded: " << deserializedHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << deserializedHeader.GetPowerConstraints() << "\n";
    std::cout << "CurrentClusterChannelIncluded: " << deserializedHeader.GetCurrentClusterChannelIncluded() << "\n";
    std::cout << "NetworkBeaconChannels: 0x" << std::hex << (int)deserializedHeader.GetNetworkBeaconChannels() << "\n";
    std::cout << "NetworkBeaconPeriod: " << (int)deserializedHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << (int)deserializedHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "NextClusterChannel: 0x" << std::hex << deserializedHeader.GetNextClusterChannel() << "\n";
    std::cout << "TimeToNext: 0x" << std::hex << deserializedHeader.GetTimeToNext() << "\n";
    std::cout << "ClustersMaxTxPower: 0x" << std::hex << (int)deserializedHeader.GetClustersMaxTxPower() << "\n";
    std::cout << "CurrentClusterChannel: 0x" << std::hex << deserializedHeader.GetCurrentClusterChannel() << "\n";
    
    uint16_t* deserializedAdditionalChannels = deserializedHeader.GetAdditionalNetworkBeaconChannels();
    std::cout << "AdditionalNetworkBeaconChannels: [0x" << std::hex 
              << deserializedAdditionalChannels[0] 
              << ", 0x" << deserializedAdditionalChannels[1] 
              << ", 0x" << deserializedAdditionalChannels[2] << "]";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTxPowerIncluded(), 
                         originalHeader.GetTxPowerIncluded(),
                         "TxPowerIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPowerConstraints(), 
                         originalHeader.GetPowerConstraints(),
                         "PowerConstraints mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrentClusterChannelIncluded(), 
                         originalHeader.GetCurrentClusterChannelIncluded(),
                         "CurrentClusterChannelIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkBeaconChannels(), 
                         originalHeader.GetNetworkBeaconChannels(),
                         "NetworkBeaconChannels mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkBeaconPeriod(), 
                         originalHeader.GetNetworkBeaconPeriod(),
                         "NetworkBeaconPeriod mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClusterBeaconPeriod(), 
                         originalHeader.GetClusterBeaconPeriod(),
                         "ClusterBeaconPeriod mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNextClusterChannel(), 
                         originalHeader.GetNextClusterChannel(),
                         "NextClusterChannel mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTimeToNext(), 
                         originalHeader.GetTimeToNext(),
                         "TimeToNext mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClustersMaxTxPower(), 
                         originalHeader.GetClustersMaxTxPower(),
                         "ClustersMaxTxPower mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrentClusterChannel(), 
                         originalHeader.GetCurrentClusterChannel(),
                         "CurrentClusterChannel mismatch after serialization/deserialization");

    uint16_t* origChannels = originalHeader.GetAdditionalNetworkBeaconChannels();
    uint16_t* deserializedChannels = deserializedHeader.GetAdditionalNetworkBeaconChannels();
    for (int i = 0; i < 3; i++) {
        NS_TEST_ASSERT_MSG_EQ(deserializedChannels[i], 
                             origChannels[i],
                             "AdditionalNetworkBeaconChannels[" << i << "] mismatch after serialization/deserialization");
    }
}

} // namespace ns3