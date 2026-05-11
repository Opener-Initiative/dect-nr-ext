#include "dect2020ext-association-response-message-ie-serialization-test-case.h"

#include "ns3/dect2020ext-association-response-message-ie.h"

#include "ns3/packet.h"

#include <random>

namespace ns3
{

Dect2020ExtAssociationResponseMessageIeSerializationTestCase::Dect2020ExtAssociationResponseMessageIeSerializationTestCase()
    : TestCase("Dect2020ExtAssociationResponseMessageIeSerializationTestCase")
{
}

Dect2020ExtAssociationResponseMessageIeSerializationTestCase::~Dect2020ExtAssociationResponseMessageIeSerializationTestCase()
{
}

void
Dect2020ExtAssociationResponseMessageIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtAssociationResponseMessageIeSerializationTestCase::DoRun()
{
    Dect2020ExtAssociationResponseMessageIe originalHeader;
    Dect2020ExtAssociationResponseMessageIe deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());

    std::bernoulli_distribution associationAcceptedDist(0.5);
    std::bernoulli_distribution harqModDist(0.5);
    std::bernoulli_distribution groupIdAndResourceTagIncludedDist(0.5);
    std::uniform_int_distribution<unsigned int> numberOfFlowsDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> rejectCauseDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> rejectTimerDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> harqProcessesRxDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> maxHarqReRxDelayDist(0, 0x1F);
    std::uniform_int_distribution<unsigned int> harqProcessesTxDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> maxHarqReTxDelayDist(0, 0x1F);
    std::uniform_int_distribution<unsigned int> flowIdDist(0, 0x3F);
    std::uniform_int_distribution<unsigned int> groupIdDist(0, 0x7F);
    std::uniform_int_distribution<unsigned int> resourceTagDist(0, 0x7F);

    bool associationAccepted = associationAcceptedDist(mt);
    bool harqMod = harqModDist(mt);
    bool groupIdAndResourceTagIncluded = groupIdAndResourceTagIncludedDist(mt);
    auto numberOfFlows = static_cast<uint8_t>(numberOfFlowsDist(mt));
    auto rejectCause = static_cast<uint8_t>(rejectCauseDist(mt));
    auto rejectTimer = static_cast<uint8_t>(rejectTimerDist(mt));
    auto harqProcessesRx = static_cast<uint8_t>(harqProcessesRxDist(mt));
    auto maxHarqReRxDelay = static_cast<uint8_t>(maxHarqReRxDelayDist(mt));
    auto harqProcessesTx = static_cast<uint8_t>(harqProcessesTxDist(mt));
    auto maxHarqReTxDelay = static_cast<uint8_t>(maxHarqReTxDelayDist(mt));
    auto flowId = static_cast<uint8_t>(flowIdDist(mt));
    auto groupId = static_cast<uint8_t>(groupIdDist(mt));
    auto resourceTag = static_cast<uint8_t>(resourceTagDist(mt));

    originalHeader.SetAssociationAccepted(associationAccepted);
    originalHeader.SetHarqMod(harqMod);
    originalHeader.SetNumberOfFlows(numberOfFlows);
    originalHeader.SetGroupIdAndResourceTagIncluded(groupIdAndResourceTagIncluded);
    originalHeader.SetFlowId(flowId);

    if (!associationAccepted)
    {
        originalHeader.SetRejectCause(rejectCause);
        originalHeader.SetRejectTimer(rejectTimer);
    }

    if (harqMod)
    {
        originalHeader.SetHarqProcessesRx(harqProcessesRx);
        originalHeader.SetMaxHarqReRxDelay(maxHarqReRxDelay);
        originalHeader.SetHarqProcessesTx(harqProcessesTx);
        originalHeader.SetMaxHarqReTxDelay(maxHarqReTxDelay);
    }

    if (groupIdAndResourceTagIncluded)
    {
        originalHeader.SetGroupId(groupId);
        originalHeader.SetResourceTag(resourceTag);
    }

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "AssociationAccepted: " << originalHeader.GetAssociationAccepted() << "\n";
    std::cout << "HarqMod: " << originalHeader.GetHarqMod() << "\n";
    std::cout << "NumberOfFlows: " << std::hex << (unsigned int)originalHeader.GetNumberOfFlows() << "\n";
    std::cout << "GroupIdAndResourceTagIncluded: " << originalHeader.GetGroupIdAndResourceTagIncluded() << "\n";
    std::cout << "FlowId: " << std::hex << (unsigned int)originalHeader.GetFlowId() << "\n";

    if (!associationAccepted)
    {
        std::cout << "RejectCause: " << std::hex << (unsigned int)originalHeader.GetRejectCause() << "\n";
        std::cout << "RejectTimer: " << std::hex << (unsigned int)originalHeader.GetRejectTimer() << "\n";
    }

    if (harqMod)
    {
        std::cout << "HarqProcessesRx: " << std::hex << (unsigned int)originalHeader.GetHarqProcessesRx() << "\n";
        std::cout << "MaxHarqReRxDelay: " << std::hex << (unsigned int)originalHeader.GetMaxHarqReRxDelay() << "\n";
        std::cout << "HarqProcessesTx: " << std::hex << (unsigned int)originalHeader.GetHarqProcessesTx() << "\n";
        std::cout << "MaxHarqReTxDelay: " << std::hex << (unsigned int)originalHeader.GetMaxHarqReTxDelay() << "\n";
    }

    if (groupIdAndResourceTagIncluded)
    {
        std::cout << "GroupId: " << std::hex << (unsigned int)originalHeader.GetGroupId() << "\n";
        std::cout << "ResourceTag: " << std::hex << (unsigned int)originalHeader.GetResourceTag() << "\n";
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
    std::cout << "AssociationAccepted: " << deserializedHeader.GetAssociationAccepted() << "\n";
    std::cout << "HarqMod: " << deserializedHeader.GetHarqMod() << "\n";
    std::cout << "NumberOfFlows: " << std::hex << (unsigned int)deserializedHeader.GetNumberOfFlows() << "\n";
    std::cout << "GroupIdAndResourceTagIncluded: " << deserializedHeader.GetGroupIdAndResourceTagIncluded() << "\n";
    std::cout << "FlowId: " << std::hex << (unsigned int)deserializedHeader.GetFlowId() << "\n";

    if (!associationAccepted)
    {
        std::cout << "RejectCause: " << std::hex << (unsigned int)deserializedHeader.GetRejectCause() << "\n";
        std::cout << "RejectTimer: " << std::hex << (unsigned int)deserializedHeader.GetRejectTimer() << "\n";
    }

    if (harqMod)
    {
        std::cout << "HarqProcessesRx: " << std::hex << (unsigned int)deserializedHeader.GetHarqProcessesRx() << "\n";
        std::cout << "MaxHarqReRxDelay: " << std::hex << (unsigned int)deserializedHeader.GetMaxHarqReRxDelay() << "\n";
        std::cout << "HarqProcessesTx: " << std::hex << (unsigned int)deserializedHeader.GetHarqProcessesTx() << "\n";
        std::cout << "MaxHarqReTxDelay: " << std::hex << (unsigned int)deserializedHeader.GetMaxHarqReTxDelay() << "\n";
    }

    if (groupIdAndResourceTagIncluded)
    {
        std::cout << "GroupId: " << std::hex << (unsigned int)deserializedHeader.GetGroupId() << "\n";
        std::cout << "ResourceTag: " << std::hex << (unsigned int)deserializedHeader.GetResourceTag() << "\n";
    }

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, expectedSize, "Packet size after adding header mismatch");

    NS_TEST_ASSERT_MSG_EQ(sizeAfterDeserialization, 0, "Packet size after removing header should be 0");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetAssociationAccepted(),
                          associationAccepted,
                          "AssociationAccepted mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqMod(),
                          harqMod,
                          "HarqMod mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNumberOfFlows(),
                          numberOfFlows,
                          "NumberOfFlows mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetGroupIdAndResourceTagIncluded(),
                          groupIdAndResourceTagIncluded,
                          "GroupIdAndResourceTagIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFlowId(),
                          flowId,
                          "FlowId mismatch after serialization/deserialization");

    if (!associationAccepted)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRejectCause(),
                              rejectCause,
                              "RejectCause mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRejectTimer(),
                              rejectTimer,
                              "RejectTimer mismatch after serialization/deserialization");
    }

    if (harqMod)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqProcessesRx(),
                              harqProcessesRx,
                              "HarqProcessesRx mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxHarqReRxDelay(),
                              maxHarqReRxDelay,
                              "MaxHarqReRxDelay mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqProcessesTx(),
                              harqProcessesTx,
                              "HarqProcessesTx mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxHarqReTxDelay(),
                              maxHarqReTxDelay,
                              "MaxHarqReTxDelay mismatch after serialization/deserialization");
    }

    if (groupIdAndResourceTagIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetGroupId(),
                              groupId,
                              "GroupId mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetResourceTag(),
                              resourceTag,
                              "ResourceTag mismatch after serialization/deserialization");
    }
}

} // namespace ns3
