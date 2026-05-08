#include "dect2020ext-cluster-beacon-message-ie-serialization-test-case.h"

#include "ns3/dect2020ext-cluster-beacon-message-ie.h"

#include "ns3/packet.h"

#include <array>
#include <random>

namespace ns3
{

namespace
{

constexpr std::array<Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod, 7> kValidNetworkBeaconPeriods = {

    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_100MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_500MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_1000MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_1500MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_2000MS,
    Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_4000MS
};

constexpr std::array<Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod, 11> kValidClusterBeaconPeriods = {

    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_10MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_50MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_500MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_1000MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_1500MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_2000MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_4000MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_8000MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_16000MS,
    Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_32000MS
};

} // namespace

Dect2020ExtClusterBeaconMessageIeSerializationTestCase::Dect2020ExtClusterBeaconMessageIeSerializationTestCase()
    : TestCase("Dect2020ExtClusterBeaconMessageIeSerializationTestCase")
{
}

Dect2020ExtClusterBeaconMessageIeSerializationTestCase::~Dect2020ExtClusterBeaconMessageIeSerializationTestCase()
{
}

void
Dect2020ExtClusterBeaconMessageIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtClusterBeaconMessageIeSerializationTestCase::DoRun()
{
    Dect2020ExtClusterBeaconMessageIe originalHeader;
    Dect2020ExtClusterBeaconMessageIe deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());

    std::bernoulli_distribution txPowerIncludedDist(0.5);
    std::bernoulli_distribution powerConstraintsDist(0.5);
    std::bernoulli_distribution frameOffsetIncludedDist(0.5);
    std::bernoulli_distribution nextChannelIncludedDist(0.5);
    std::bernoulli_distribution timeToNextFieldPresentDist(0.5);
    std::uniform_int_distribution<unsigned int> sfnDist(0, 0xFF);
    std::uniform_int_distribution<size_t> networkBeaconPeriodIdxDist(0, kValidNetworkBeaconPeriods.size() - 1);
    std::uniform_int_distribution<size_t> clusterBeaconPeriodIdxDist(0, kValidClusterBeaconPeriods.size() - 1);
    std::uniform_int_distribution<unsigned int> countToTriggerDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> relQualityDist(0, 0x03);
    std::uniform_int_distribution<unsigned int> minQualityDist(0, 0x03);
    std::uniform_int_distribution<unsigned int> clusterMaxTxPowerDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> frameOffsetDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> nextClusterChannelDist(0, 0x1FFF);
    std::uniform_int_distribution<unsigned int> timeToNextDist(0, 0xFFFFFFFF);

    auto sfn = static_cast<uint8_t>(sfnDist(mt));
    bool txPowerIncluded = txPowerIncludedDist(mt);
    bool powerConstraints = powerConstraintsDist(mt);
    bool frameOffsetIncluded = frameOffsetIncludedDist(mt);
    bool nextChannelIncluded = nextChannelIncludedDist(mt);
    bool timeToNextFieldPresent = timeToNextFieldPresentDist(mt);
    auto networkBeaconPeriod = kValidNetworkBeaconPeriods[networkBeaconPeriodIdxDist(mt)];
    auto clusterBeaconPeriod = kValidClusterBeaconPeriods[clusterBeaconPeriodIdxDist(mt)];
    auto countToTrigger = static_cast<uint8_t>(countToTriggerDist(mt));
    auto relQuality = static_cast<uint8_t>(relQualityDist(mt));
    auto minQuality = static_cast<uint8_t>(minQualityDist(mt));
    auto clusterMaxTxPower = static_cast<uint8_t>(clusterMaxTxPowerDist(mt));
    auto frameOffset = static_cast<uint8_t>(frameOffsetDist(mt));
    auto nextClusterChannel = static_cast<uint16_t>(nextClusterChannelDist(mt));
    auto timeToNext = static_cast<uint32_t>(timeToNextDist(mt));

    originalHeader.SetSFN(sfn);
    originalHeader.SetTxPowerIncluded(txPowerIncluded);
    originalHeader.SetPowerConstraints(powerConstraints);
    originalHeader.SetFrameOffsetIncluded(frameOffsetIncluded);
    originalHeader.SetNextChannelIncluded(nextChannelIncluded);
    originalHeader.SetTimeToNextFieldPresent(timeToNextFieldPresent);
    originalHeader.SetNetworkBeaconPeriod(networkBeaconPeriod);
    originalHeader.SetClusterBeaconPeriod(clusterBeaconPeriod);
    originalHeader.SetCountToTrigger(countToTrigger);
    originalHeader.SetRelQuality(relQuality);
    originalHeader.SetMinQuality(minQuality);

    if (txPowerIncluded)
    {
        originalHeader.SetClusterMaxTXPower(clusterMaxTxPower);
    }

    if (frameOffsetIncluded)
    {
        originalHeader.SetFrameOffset(frameOffset);
    }

    if (nextChannelIncluded)
    {
        originalHeader.SetNextClusterChannel(nextClusterChannel);
    }

    if (timeToNextFieldPresent)
    {
        originalHeader.SetTimeToNext(timeToNext);
    }

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "SFN: " << std::hex << (unsigned int)originalHeader.GetSFN() << "\n";
    std::cout << "TxPowerIncluded: " << originalHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << originalHeader.GetPowerConstraints() << "\n";
    std::cout << "FrameOffsetIncluded: " << originalHeader.GetFrameOffsetIncluded() << "\n";
    std::cout << "NextChannelIncluded: " << originalHeader.GetNextChannelIncluded() << "\n";
    std::cout << "TimeToNextFieldPresent: " << originalHeader.GetTimeToNextFieldPresent() << "\n";
    std::cout << "NetworkBeaconPeriod: " << std::hex << originalHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << std::hex << originalHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "CountToTrigger: " << std::hex << (unsigned int)originalHeader.GetCountToTrigger() << "\n";
    std::cout << "RelQuality: " << std::hex << (unsigned int)originalHeader.GetRelQuality() << "\n";
    std::cout << "MinQuality: " << std::hex << (unsigned int)originalHeader.GetMinQuality() << "\n";

    if (txPowerIncluded)
    {
        std::cout << "ClusterMaxTxPower: " << std::hex << (unsigned int)originalHeader.GetClusterMaxTXPower() << "\n";
    }

    if (frameOffsetIncluded)
    {
        std::cout << "FrameOffset: " << std::hex << (unsigned int)originalHeader.GetFrameOffset() << "\n";
    }

    if (nextChannelIncluded)
    {
        std::cout << "NextClusterChannel: " << std::hex << originalHeader.GetNextClusterChannel() << "\n";
    }

    if (timeToNextFieldPresent)
    {
        std::cout << "TimeToNext: " << std::hex << originalHeader.GetTimeToNext() << "\n";
    }

    std::cout << "\n";

    uint32_t expectedSize = originalHeader.GetSerializedSize();
    uint32_t sizeBefore = packet.GetSize();

    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);

    uint32_t sizeAfter = packet.GetSize();

    std::cout << "Size after adding header: " << sizeAfter << "\n";

    packet.RemoveHeader(deserializedHeader);

    uint32_t sizeAfterDeserialization = packet.GetSize();

    std::cout << "Size after deserialization: " << sizeAfterDeserialization << "\n" << "\n";

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "SFN: " << std::hex << (unsigned int)deserializedHeader.GetSFN() << "\n";
    std::cout << "TxPowerIncluded: " << deserializedHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << deserializedHeader.GetPowerConstraints() << "\n";
    std::cout << "FrameOffsetIncluded: " << deserializedHeader.GetFrameOffsetIncluded() << "\n";
    std::cout << "NextChannelIncluded: " << deserializedHeader.GetNextChannelIncluded() << "\n";
    std::cout << "TimeToNextFieldPresent: " << deserializedHeader.GetTimeToNextFieldPresent() << "\n";
    std::cout << "NetworkBeaconPeriod: " << std::hex << deserializedHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << std::hex << deserializedHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "CountToTrigger: " << std::hex << (unsigned int)deserializedHeader.GetCountToTrigger() << "\n";
    std::cout << "RelQuality: " << std::hex << (unsigned int)deserializedHeader.GetRelQuality() << "\n";
    std::cout << "MinQuality: " << std::hex << (unsigned int)deserializedHeader.GetMinQuality() << "\n";

    if (txPowerIncluded)
    {
        std::cout << "ClusterMaxTxPower: " << std::hex << (unsigned int)deserializedHeader.GetClusterMaxTXPower() << "\n";
    }

    if (frameOffsetIncluded)
    {
        std::cout << "FrameOffset: " << std::hex << (unsigned int)deserializedHeader.GetFrameOffset() << "\n";
    }

    if (nextChannelIncluded)
    {
        std::cout << "NextClusterChannel: " << std::hex << deserializedHeader.GetNextClusterChannel() << "\n";
    }

    if (timeToNextFieldPresent)
    {
        std::cout << "TimeToNext: " << std::hex << deserializedHeader.GetTimeToNext() << "\n";
    }

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, expectedSize, "Packet size after adding header mismatch");

    NS_TEST_ASSERT_MSG_EQ(sizeAfterDeserialization, 0, "Packet size after removing header should be 0");

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

    if (txPowerIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClusterMaxTXPower(),
                              originalHeader.GetClusterMaxTXPower(),
                              "ClusterMaxTxPower mismatch after serialization/deserialization");
    }

    if (frameOffsetIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFrameOffset(),
                              originalHeader.GetFrameOffset(),
                              "FrameOffset mismatch after serialization/deserialization");
    }

    if (nextChannelIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNextClusterChannel(),
                              originalHeader.GetNextClusterChannel(),
                              "NextClusterChannel mismatch after serialization/deserialization");
    }

    if (timeToNextFieldPresent)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTimeToNext(),
                              originalHeader.GetTimeToNext(),
                              "TimeToNext mismatch after serialization/deserialization");
    }
}

} // namespace ns3
