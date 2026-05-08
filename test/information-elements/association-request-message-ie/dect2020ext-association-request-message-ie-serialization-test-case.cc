#include "dect2020ext-association-request-message-ie-serialization-test-case.h"

#include "ns3/dect2020ext-association-request-message-ie.h"

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

Dect2020ExtAssociationRequestMessageIeSerializationTestCase::Dect2020ExtAssociationRequestMessageIeSerializationTestCase()
    : TestCase("Dect2020ExtAssociationRequestMessageIeSerializationTestCase")
{
}

Dect2020ExtAssociationRequestMessageIeSerializationTestCase::~Dect2020ExtAssociationRequestMessageIeSerializationTestCase()
{
}

void
Dect2020ExtAssociationRequestMessageIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtAssociationRequestMessageIeSerializationTestCase::DoRun()
{
    Dect2020ExtAssociationRequestMessageIe originalHeader;
    Dect2020ExtAssociationRequestMessageIe deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());

    std::bernoulli_distribution powerConstraintsDist(0.5);
    std::bernoulli_distribution ftModeDist(0.5);
    std::bernoulli_distribution currentDist(0.5);
    std::uniform_int_distribution<unsigned int> setupCauseDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> numberOfFlowsDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> harqProcessesTxDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> maxHarqReTxDelayDist(0, 0x1F);
    std::uniform_int_distribution<unsigned int> harqProcessesRxDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> maxHarqReRxDelayDist(0, 0x1F);
    std::uniform_int_distribution<unsigned int> flowIdDist(0, 0x3F);
    std::uniform_int_distribution<size_t> networkBeaconPeriodIdxDist(0, kValidNetworkBeaconPeriods.size() - 1);
    std::uniform_int_distribution<size_t> clusterBeaconPeriodIdxDist(0, kValidClusterBeaconPeriods.size() - 1);
    std::uniform_int_distribution<unsigned int> nextClusterChannelDist(0, 0x1FFF);
    std::uniform_int_distribution<unsigned int> timeToNextDist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<unsigned int> currentClusterChannelDist(0, 0x1FFF);

    auto setupCause = static_cast<uint8_t>(setupCauseDist(mt));
    auto numberOfFlows = static_cast<uint8_t>(numberOfFlowsDist(mt));
    bool powerConstraints = powerConstraintsDist(mt);
    bool ftMode = ftModeDist(mt);
    bool current = currentDist(mt);
    auto harqProcessesTx = static_cast<uint8_t>(harqProcessesTxDist(mt));
    auto maxHarqReTxDelay = static_cast<uint8_t>(maxHarqReTxDelayDist(mt));
    auto harqProcessesRx = static_cast<uint8_t>(harqProcessesRxDist(mt));
    auto maxHarqReRxDelay = static_cast<uint8_t>(maxHarqReRxDelayDist(mt));
    auto flowId = static_cast<uint8_t>(flowIdDist(mt));
    auto networkBeaconPeriod = kValidNetworkBeaconPeriods[networkBeaconPeriodIdxDist(mt)];
    auto clusterBeaconPeriod = kValidClusterBeaconPeriods[clusterBeaconPeriodIdxDist(mt)];
    auto nextClusterChannel = static_cast<uint16_t>(nextClusterChannelDist(mt));
    auto timeToNext = static_cast<uint32_t>(timeToNextDist(mt));
    auto currentClusterChannel = static_cast<uint16_t>(currentClusterChannelDist(mt));

    originalHeader.SetSetupCause(setupCause);
    originalHeader.SetNumberOfFlows(numberOfFlows);
    originalHeader.SetPowerConstraints(powerConstraints);
    originalHeader.SetFtMode(ftMode);
    originalHeader.SetCurrent(current);
    originalHeader.SetHarqProcessesTx(harqProcessesTx);
    originalHeader.SetMaxHarqReTxDelay(maxHarqReTxDelay);
    originalHeader.SetHarqProcessesRx(harqProcessesRx);
    originalHeader.SetMaxHarqReRxDelay(maxHarqReRxDelay);
    originalHeader.SetFlowId(flowId);

    if (ftMode)
    {
        originalHeader.SetNetworkBeaconPeriod(networkBeaconPeriod);
        originalHeader.SetClusterBeaconPeriod(clusterBeaconPeriod);
        originalHeader.SetNextClusterChannel(nextClusterChannel);
        originalHeader.SetTimeToNext(timeToNext);
    }

    if (current)
    {
        originalHeader.SetCurrentClusterChannel(currentClusterChannel);
    }

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "SetupCause: " << std::hex << (unsigned int)originalHeader.GetSetupCause() << "\n";
    std::cout << "NumberOfFlows: " << std::hex << (unsigned int)originalHeader.GetNumberOfFlows() << "\n";
    std::cout << "PowerConstraints: " << originalHeader.GetPowerConstraints() << "\n";
    std::cout << "FtMode: " << originalHeader.GetFtMode() << "\n";
    std::cout << "Current: " << originalHeader.GetCurrent() << "\n";
    std::cout << "HarqProcessesTx: " << std::hex << (unsigned int)originalHeader.GetHarqProcessesTx() << "\n";
    std::cout << "MaxHarqReTxDelay: " << std::hex << (unsigned int)originalHeader.GetMaxHarqReTxDelay() << "\n";
    std::cout << "HarqProcessesRx: " << std::hex << (unsigned int)originalHeader.GetHarqProcessesRx() << "\n";
    std::cout << "MaxHarqReRxDelay: " << std::hex << (unsigned int)originalHeader.GetMaxHarqReRxDelay() << "\n";
    std::cout << "FlowId: " << std::hex << (unsigned int)originalHeader.GetFlowId() << "\n";

    if (ftMode)
    {
        std::cout << "NetworkBeaconPeriod: " << std::hex << originalHeader.GetNetworkBeaconPeriod() << "\n";
        std::cout << "ClusterBeaconPeriod: " << std::hex << originalHeader.GetClusterBeaconPeriod() << "\n";
        std::cout << "NextClusterChannel: " << std::hex << originalHeader.GetNextClusterChannel() << "\n";
        std::cout << "TimeToNext: " << std::hex << originalHeader.GetTimeToNext() << "\n";
    }

    if (current)
    {
        std::cout << "CurrentClusterChannel: " << std::hex << originalHeader.GetCurrentClusterChannel() << "\n";
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
    std::cout << "SetupCause: " << std::hex << (unsigned int)deserializedHeader.GetSetupCause() << "\n";
    std::cout << "NumberOfFlows: " << std::hex << (unsigned int)deserializedHeader.GetNumberOfFlows() << "\n";
    std::cout << "PowerConstraints: " << deserializedHeader.GetPowerConstraints() << "\n";
    std::cout << "FtMode: " << deserializedHeader.GetFtMode() << "\n";
    std::cout << "Current: " << deserializedHeader.GetCurrent() << "\n";
    std::cout << "HarqProcessesTx: " << std::hex << (unsigned int)deserializedHeader.GetHarqProcessesTx() << "\n";
    std::cout << "MaxHarqReTxDelay: " << std::hex << (unsigned int)deserializedHeader.GetMaxHarqReTxDelay() << "\n";
    std::cout << "HarqProcessesRx: " << std::hex << (unsigned int)deserializedHeader.GetHarqProcessesRx() << "\n";
    std::cout << "MaxHarqReRxDelay: " << std::hex << (unsigned int)deserializedHeader.GetMaxHarqReRxDelay() << "\n";
    std::cout << "FlowId: " << std::hex << (unsigned int)deserializedHeader.GetFlowId() << "\n";

    if (ftMode)
    {
        std::cout << "NetworkBeaconPeriod: " << std::hex << deserializedHeader.GetNetworkBeaconPeriod() << "\n";
        std::cout << "ClusterBeaconPeriod: " << std::hex << deserializedHeader.GetClusterBeaconPeriod() << "\n";
        std::cout << "NextClusterChannel: " << std::hex << deserializedHeader.GetNextClusterChannel() << "\n";
        std::cout << "TimeToNext: " << std::hex << deserializedHeader.GetTimeToNext() << "\n";
    }

    if (current)
    {
        std::cout << "CurrentClusterChannel: " << std::hex << deserializedHeader.GetCurrentClusterChannel() << "\n";
    }

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, expectedSize, "Packet size after adding header mismatch");

    NS_TEST_ASSERT_MSG_EQ(sizeAfterDeserialization, 0, "Packet size after removing header should be 0");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSetupCause(),
                          originalHeader.GetSetupCause(),
                          "SetupCause mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNumberOfFlows(),
                          originalHeader.GetNumberOfFlows(),
                          "NumberOfFlows mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPowerConstraints(),
                          originalHeader.GetPowerConstraints(),
                          "PowerConstraints mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFtMode(),
                          originalHeader.GetFtMode(),
                          "FtMode mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrent(),
                          originalHeader.GetCurrent(),
                          "Current mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqProcessesTx(),
                          originalHeader.GetHarqProcessesTx(),
                          "HarqProcessesTx mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxHarqReTxDelay(),
                          originalHeader.GetMaxHarqReTxDelay(),
                          "MaxHarqReTxDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqProcessesRx(),
                          originalHeader.GetHarqProcessesRx(),
                          "HarqProcessesRx mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxHarqReRxDelay(),
                          originalHeader.GetMaxHarqReRxDelay(),
                          "MaxHarqReRxDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFlowId(),
                          originalHeader.GetFlowId(),
                          "FlowId mismatch after serialization/deserialization");

    if (ftMode)
    {
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
    }

    if (current)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCurrentClusterChannel(),
                              originalHeader.GetCurrentClusterChannel(),
                              "CurrentClusterChannel mismatch after serialization/deserialization");
    }
}

} // namespace ns3
