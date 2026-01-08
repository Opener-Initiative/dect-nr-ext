#include "dect2020ext-cluster-beacon-message-ie-serialization-test-case.h"
#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/packet.h"
#include "ns3/test.h"

namespace ns3
{

Dect2020ExtClusterBeaconMessageIESerializationTestCase::Dect2020ExtClusterBeaconMessageIESerializationTestCase() 
    : TestCase("ClusterBeaconMessageIESerializationTest")
{
}

Dect2020ExtClusterBeaconMessageIESerializationTestCase::~Dect2020ExtClusterBeaconMessageIESerializationTestCase()
{
}

void
Dect2020ExtClusterBeaconMessageIESerializationTestCase::DoSetup()
{
}

void
Dect2020ExtClusterBeaconMessageIESerializationTestCase::DoRun()
{
    Dect2020ExtClusterBeaconMessageIE originalHeader;
    Dect2020ExtClusterBeaconMessageIE deserializedHeader;
    Packet packet;

    originalHeader.SetSFN(5);
    originalHeader.SetTxPowerIncluded(false);
    originalHeader.SetPowerConstraints(false);
    originalHeader.SetFrameOffsetIncluded(false);
    originalHeader.SetNextChannelIncluded(false);
    originalHeader.SetTimeToNextFieldPresent(false);
    originalHeader.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod::NETWORK_BEACON_PERIOD_500MS); // 2
    originalHeader.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod::CLUSTER_BEACON_PERIOD_100MS);
    originalHeader.SetCountToTrigger(0xA);
    originalHeader.SetRelQuality(0x2);
    originalHeader.SetMinQuality(0x1);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "SFN: 0x" << std::hex << (int)originalHeader.GetSFN() << "\n";
    std::cout << "TxPowerIncluded: " << originalHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << originalHeader.GetPowerConstraints() << "\n";
    std::cout << "FrameOffsetIncluded: " << originalHeader.GetFrameOffsetIncluded() << "\n";
    std::cout << "NextChannelIncluded: " << originalHeader.GetNextChannelIncluded() << "\n";
    std::cout << "TimeToNextFieldPresent: " << originalHeader.GetTimeToNextFieldPresent() << "\n";
    std::cout << "NetworkBeaconPeriod: " << (int)originalHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << (int)originalHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "CountToTrigger: 0x" << std::hex << (int)originalHeader.GetCountToTrigger() << "\n";
    std::cout << "RelQuality: 0x" << std::hex << (int)originalHeader.GetRelQuality() << "\n";
    std::cout << "MinQuality: 0x" << std::hex << (int)originalHeader.GetMinQuality() << "\n";

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    packet.AddHeader(originalHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 4, "Should be 4");

    packet.RemoveHeader(deserializedHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "SFN: 0x" << std::hex << (int)deserializedHeader.GetSFN() << "\n";
    std::cout << "TxPowerIncluded: " << deserializedHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << deserializedHeader.GetPowerConstraints() << "\n";
    std::cout << "FrameOffsetIncluded: " << deserializedHeader.GetFrameOffsetIncluded() << "\n";
    std::cout << "NextChannelIncluded: " << deserializedHeader.GetNextChannelIncluded() << "\n";
    std::cout << "TimeToNextFieldPresent: " << deserializedHeader.GetTimeToNextFieldPresent() << "\n";
    std::cout << "NetworkBeaconPeriod: " << (int)deserializedHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << (int)deserializedHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "CountToTrigger: 0x" << std::hex << (int)deserializedHeader.GetCountToTrigger() << "\n";
    std::cout << "RelQuality: 0x" << std::hex << (int)deserializedHeader.GetRelQuality() << "\n";
    std::cout << "MinQuality: 0x" << std::hex << (int)deserializedHeader.GetMinQuality() << "\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSFN(), 
                         originalHeader.GetSFN(),
                         "SFN mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTxPowerIncluded(), 
                         originalHeader.GetTxPowerIncluded(),
                         "TxPowerIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPowerConstraints(), 
                         originalHeader.GetPowerConstraints(),
                         "PowerConstraints mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFrameOffsetIncluded(), 
                         originalHeader.GetFrameOffsetIncluded(),
                         "FrameOffsetIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNextChannelIncluded(), 
                         originalHeader.GetNextChannelIncluded(),
                         "NextChannelIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTimeToNextFieldPresent(), 
                         originalHeader.GetTimeToNextFieldPresent(),
                         "TimeToNextFieldPresent mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNetworkBeaconPeriod(), 
                         originalHeader.GetNetworkBeaconPeriod(),
                         "NetworkBeaconPeriod mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClusterBeaconPeriod(), 
                         originalHeader.GetClusterBeaconPeriod(),
                         "ClusterBeaconPeriod mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCountToTrigger(), 
                         originalHeader.GetCountToTrigger(),
                         "CountToTrigger mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRelQuality(), 
                         originalHeader.GetRelQuality(),
                         "RelQuality mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMinQuality(), 
                         originalHeader.GetMinQuality(),
                         "MinQuality mismatch after serialization/deserialization");
}

} // namespace ns3

