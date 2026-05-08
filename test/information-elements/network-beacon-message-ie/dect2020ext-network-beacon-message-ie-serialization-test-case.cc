#include "dect2020ext-network-beacon-message-ie-serialization-test-case.h"

#include "ns3/dect2020ext-network-beacon-message-ie.h"

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

    std::random_device rd;
    std::mt19937 mt(rd());

    std::bernoulli_distribution txPowerIncludedDist(0.5);
    std::bernoulli_distribution powerConstraintsDist(0.5);
    std::bernoulli_distribution currentClusterChannelIncludedDist(0.5);
    std::uniform_int_distribution<unsigned int> networkBeaconChannelsDist(0, 3);
    std::uniform_int_distribution<size_t> networkBeaconPeriodIdxDist(0, kValidNetworkBeaconPeriods.size() - 1);
    std::uniform_int_distribution<size_t> clusterBeaconPeriodIdxDist(0, kValidClusterBeaconPeriods.size() - 1);
    std::uniform_int_distribution<unsigned int> nextClusterChannelDist(0, 0x1FFF);
    std::uniform_int_distribution<unsigned int> timeToNextDist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<unsigned int> clusterMaxTxPowerDist(0, 15);
    std::uniform_int_distribution<unsigned int> currentClusterChannelDist(0, 0x1FFF);
    std::uniform_int_distribution<unsigned int> additionalNetworkBeaconChannelsDist(0, 0x1FFF);

    bool txPowerIncluded = txPowerIncludedDist(mt);
    bool powerConstraints = powerConstraintsDist(mt);
    bool currentClusterChannelIncluded = currentClusterChannelIncludedDist(mt);
    auto networkBeaconChannels = static_cast<uint8_t>(networkBeaconChannelsDist(mt));
    auto networkBeaconPeriod = kValidNetworkBeaconPeriods[networkBeaconPeriodIdxDist(mt)];
    auto clusterBeaconPeriod = kValidClusterBeaconPeriods[clusterBeaconPeriodIdxDist(mt)];
    auto nextClusterChannel = static_cast<uint16_t>(nextClusterChannelDist(mt));
    auto timeToNext = static_cast<uint32_t>(timeToNextDist(mt));
    auto clusterMaxTxPower = static_cast<uint8_t>(clusterMaxTxPowerDist(mt));
    auto currentClusterChannel = static_cast<uint16_t>(currentClusterChannelDist(mt));
    uint16_t additionalNetworkBeaconChannels[3]{};

    for (uint8_t i = 0; i < networkBeaconChannels; i++)
    {
        additionalNetworkBeaconChannels[i] = additionalNetworkBeaconChannelsDist(mt);
    }

    originalHeader.SetTxPowerIncluded(txPowerIncluded);
    originalHeader.SetPowerConstraints(powerConstraints);
    originalHeader.SetCurrentClusterChannelIncluded(currentClusterChannelIncluded);
    originalHeader.SetNetworkBeaconChannels(networkBeaconChannels);
    originalHeader.SetNetworkBeaconPeriod(networkBeaconPeriod);
    originalHeader.SetClusterBeaconPeriod(clusterBeaconPeriod);
    originalHeader.SetNextClusterChannel(nextClusterChannel);
    originalHeader.SetTimeToNext(timeToNext);

    if (txPowerIncluded)
    {
        originalHeader.SetClustersMaxTxPower(clusterMaxTxPower);
    }

    if (currentClusterChannelIncluded)
    {
        originalHeader.SetCurrentClusterChannel(currentClusterChannel);
    }

    if (networkBeaconChannels > 0)
    {
        originalHeader.SetAdditionalNetworkBeaconChannels(additionalNetworkBeaconChannels);
    }

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "TxPowerIncluded: " << originalHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << originalHeader.GetPowerConstraints() << "\n";
    std::cout << "CurrentClusterChannelIncluded: " << originalHeader.GetCurrentClusterChannelIncluded() << "\n";
    std::cout << "NetworkBeaconChannels: " << std::hex << (unsigned int)originalHeader.GetNetworkBeaconChannels() << "\n";
    std::cout << "NetworkBeaconPeriod: " << std::hex << originalHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << std::hex << originalHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "NextClusterChannel: " << std::hex << originalHeader.GetNextClusterChannel() << "\n";
    std::cout << "TimeToNext: " << std::hex << originalHeader.GetTimeToNext() << "\n";

    if (txPowerIncluded)
    {
        std::cout << "ClustersMaxTxPower: " << std::hex << (unsigned int)originalHeader.GetClustersMaxTxPower() << "\n";
    }

    if (currentClusterChannelIncluded)
    {
        std::cout << "CurrentClusterChannel: " << std::hex << originalHeader.GetCurrentClusterChannel() << "\n";
    }

    for (uint8_t i = 0; i < networkBeaconChannels; i++)
    {
        std::cout << "AdditionalNetworkBeaconChannels[" << (unsigned int)i << "]: "
                  << std::hex << originalHeader.GetAdditionalNetworkBeaconChannels()[i] << "\n";
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
    std::cout << "TxPowerIncluded: " << deserializedHeader.GetTxPowerIncluded() << "\n";
    std::cout << "PowerConstraints: " << deserializedHeader.GetPowerConstraints() << "\n";
    std::cout << "CurrentClusterChannelIncluded: " << deserializedHeader.GetCurrentClusterChannelIncluded() << "\n";
    std::cout << "NetworkBeaconChannels: " << std::hex << (unsigned int)deserializedHeader.GetNetworkBeaconChannels() << "\n";
    std::cout << "NetworkBeaconPeriod: " << std::hex << deserializedHeader.GetNetworkBeaconPeriod() << "\n";
    std::cout << "ClusterBeaconPeriod: " << std::hex << deserializedHeader.GetClusterBeaconPeriod() << "\n";
    std::cout << "NextClusterChannel: " << std::hex << deserializedHeader.GetNextClusterChannel() << "\n";
    std::cout << "TimeToNext: " << std::hex << deserializedHeader.GetTimeToNext() << "\n";

    if (txPowerIncluded)
    {
        std::cout << "ClustersMaxTxPower: " << std::hex << (unsigned int)deserializedHeader.GetClustersMaxTxPower() << "\n";
    }

    if (currentClusterChannelIncluded)
    {
        std::cout << "CurrentClusterChannel: " << std::hex << deserializedHeader.GetCurrentClusterChannel() << "\n";
    }

    for (uint8_t i = 0; i < networkBeaconChannels; i++)
    {
        std::cout << "AdditionalNetworkBeaconChannels[" << (unsigned int)i << "]: "
                  << std::hex << deserializedHeader.GetAdditionalNetworkBeaconChannels()[i] << "\n";
    }

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, expectedSize, "Packet size after adding header mismatch");

    NS_TEST_ASSERT_MSG_EQ(sizeAfterDeserialization, 0, "Packet size after removing header should be 0");

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

    if (txPowerIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetClustersMaxTxPower(),
                              originalHeader.GetClustersMaxTxPower(),
                              "ClustersMaxTxPower mismatch after serialization/deserialization");
    }

    if (currentClusterChannelIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrentClusterChannel(),
                              originalHeader.GetCurrentClusterChannel(),
                              "CurrentClusterChannel mismatch after serialization/deserialization");
    }

    for (uint8_t i = 0; i < networkBeaconChannels; i++)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetAdditionalNetworkBeaconChannels()[i],
                              originalHeader.GetAdditionalNetworkBeaconChannels()[i],
                              "AdditionalNetworkBeaconChannels mismatch after serialization/deserialization");
    }
}

} // namespace ns3