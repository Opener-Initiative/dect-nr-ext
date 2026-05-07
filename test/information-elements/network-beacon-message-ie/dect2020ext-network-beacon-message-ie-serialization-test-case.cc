#include "dect2020ext-network-beacon-message-ie-serialization-test-case.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/packet.h"
#include "ns3/test.h"

namespace ns3 {

Dect2020ExtNetworkBeaconMessageIeSerializationTestCase::Dect2020ExtNetworkBeaconMessageIeSerializationTestCase()
    : TestCase("Dect2020ExtNetworkBeaconMessageIeSerializationTestCase")
{
}

Dect2020ExtNetworkBeaconMessageIeSerializationTestCase::~Dect2020ExtNetworkBeaconMessageIeSerializationTestCase()
{
}

void
Dect2020ExtNetworkBeaconMessageIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtNetworkBeaconMessageIeSerializationTestCase::DoRun()
{
    Dect2020ExtNetworkBeaconMessageIe originalHeader;
    Dect2020ExtNetworkBeaconMessageIe deserializedHeader;
    Packet packet;

    // // Setting the values to the original header with no optional fields set
    // originalHeader.SetTxPowerIncluded(false);
    // originalHeader.SetPowerConstraints(false);
    // originalHeader.SetCurrentClusterChannelIncluded(false);
    // originalHeader.SetNetworkBeaconChannels(0);
    // originalHeader.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod::NETWORK_BEACON_PERIOD_1000MS);
    // originalHeader.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod::CLUSTER_BEACON_PERIOD_50MS);
    // originalHeader.SetNextClusterChannel(1657);
    // originalHeader.SetTimeToNext(0);

    // uint32_t sizeBefore = packet.GetSize();
    // packet.AddHeader(originalHeader);

    // std::cout << "Packet size before adding header: " << sizeBefore << "\n";
    // std::cout << "Packet size after adding header: " << packet.GetSize() << "\n";

    // // Verify that the packet size after adding the header is 8 bytes
    // NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 8, "The packet size after adding the header should be 8 bytes");

    // packet.RemoveHeader(deserializedHeader);

    // NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "The packet size after removing the header should be 0 bytes");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTxPowerIncluded(), 
    //                      originalHeader.GetTxPowerIncluded(),
    //                      "TxPowerIncluded mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPowerConstraints(), 
    //                      originalHeader.GetPowerConstraints(),
    //                      "PowerConstraints mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrentClusterChannelIncluded(), 
    //                      originalHeader.GetCurrentClusterChannelIncluded(),
    //                      "CurrentClusterChannelIncluded mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkBeaconChannels(), 
    //                      originalHeader.GetNetworkBeaconChannels(),
    //                      "NetworkBeaconChannels mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkBeaconPeriod(), 
    //                      originalHeader.GetNetworkBeaconPeriod(),
    //                      "NetworkBeaconPeriod mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClusterBeaconPeriod(), 
    //                      originalHeader.GetClusterBeaconPeriod(),
    //                      "ClusterBeaconPeriod mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNextClusterChannel(), 
    //                      originalHeader.GetNextClusterChannel(),
    //                      "NextClusterChannel mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTimeToNext(), 
    //                      originalHeader.GetTimeToNext(),
    //                      "TimeToNext mismatch after serialization/deserialization");
}

} // namespace ns3