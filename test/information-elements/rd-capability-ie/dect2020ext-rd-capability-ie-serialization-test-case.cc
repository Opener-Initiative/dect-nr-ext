#include "dect2020ext-rd-capability-ie-serialization-test-case.h"

#include "ns3/dect2020ext-rd-capability-ie.h"

#include "ns3/packet.h"

#include <random>

namespace ns3
{

Dect2020ExtRdCapabilityIeSerializationTestCase::Dect2020ExtRdCapabilityIeSerializationTestCase()
    : TestCase("Dect2020ExtRdCapabilityIeSerializationTestCase")
{
}

Dect2020ExtRdCapabilityIeSerializationTestCase::~Dect2020ExtRdCapabilityIeSerializationTestCase()
{
}

void
Dect2020ExtRdCapabilityIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtRdCapabilityIeSerializationTestCase::DoRun()
{
    Dect2020ExtRdCapabilityIe originalHeader;
    Dect2020ExtRdCapabilityIe deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<unsigned int> numOfPhyCapabilitiesDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> releaseDist(0, 0x1F);
    std::bernoulli_distribution groupAssignmentDist(0.5);
    std::bernoulli_distribution pagingDist(0.5);
    std::uniform_int_distribution<unsigned int> operatingModesDist(0, 0x03);
    std::bernoulli_distribution meshDist(0.5);
    std::bernoulli_distribution scheduledAccessDataTransferDist(0.5);
    std::uniform_int_distribution<unsigned int> macSecurityDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> dlcServiceTypeDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> rdPowerClassDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> maxNssFoRxDist(0, 0x03);
    std::uniform_int_distribution<unsigned int> rxForTxDiversityDist(0, 0x03);
    std::uniform_int_distribution<unsigned int> rxGainDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> maxMcsDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> softBufferSizeDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> numOfHarqProcessesDist(0, 0x03);
    std::uniform_int_distribution<unsigned int> harqFeedbackDelayDist(0, 0x0F);
    std::bernoulli_distribution dDelayDist(0.5);
    std::bernoulli_distribution halfDulpDist(0.5);

    auto numOfPhyCapabilities = static_cast<uint8_t>(numOfPhyCapabilitiesDist(mt));
    auto release = static_cast<uint8_t>(releaseDist(mt));
    bool groupAssignment = groupAssignmentDist(mt);
    bool paging = pagingDist(mt);
    auto operatingModes = static_cast<uint8_t>(operatingModesDist(mt));
    bool mesh = meshDist(mt);
    bool scheduledAccessDataTransfer = scheduledAccessDataTransferDist(mt);
    auto macSecurity = static_cast<uint8_t>(macSecurityDist(mt));
    auto dlcServiceType = static_cast<uint8_t>(dlcServiceTypeDist(mt));
    auto rdPowerClass = static_cast<uint8_t>(rdPowerClassDist(mt));
    auto maxNssFoRx = static_cast<uint8_t>(maxNssFoRxDist(mt));
    auto rxForTxDiversity = static_cast<uint8_t>(rxForTxDiversityDist(mt));
    auto rxGain = static_cast<uint8_t>(rxGainDist(mt));
    auto maxMcs = static_cast<uint8_t>(maxMcsDist(mt));
    auto softBufferSize = static_cast<uint8_t>(softBufferSizeDist(mt));
    auto numOfHarqProcesses = static_cast<uint8_t>(numOfHarqProcessesDist(mt));
    auto harqFeedbackDelay = static_cast<uint8_t>(harqFeedbackDelayDist(mt));
    bool dDelay = dDelayDist(mt);
    bool halfDulp = halfDulpDist(mt);

    originalHeader.SetNumOfPhyCapabilities(numOfPhyCapabilities);
    originalHeader.SetRelease(release);
    originalHeader.SetGroupAssignment(groupAssignment);
    originalHeader.SetPaging(paging);
    originalHeader.SetOperatingModes(operatingModes);
    originalHeader.SetMesh(mesh);
    originalHeader.SetScheduledAccessDataTransfer(scheduledAccessDataTransfer);
    originalHeader.SetMacSecurity(macSecurity);
    originalHeader.SetDlcServiceType(dlcServiceType);
    originalHeader.SetRdPowerClass(rdPowerClass);
    originalHeader.SetMaxNssFoRx(maxNssFoRx);
    originalHeader.SetRxForTxDiversity(rxForTxDiversity);
    originalHeader.SetRxGain(rxGain);
    originalHeader.SetMaxMcs(maxMcs);
    originalHeader.SetSoftBufferSize(softBufferSize);
    originalHeader.SetNumOfHarqProcesses(numOfHarqProcesses);
    originalHeader.SetHarqFeedbackDelay(harqFeedbackDelay);
    originalHeader.SetDDelay(dDelay);
    originalHeader.SetHalfDulp(halfDulp);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "NumOfPhyCapabilities: 0x" << std::hex << (unsigned int)originalHeader.GetNumOfPhyCapabilities() << "\n";
    std::cout << "Release: 0x" << std::hex << (unsigned int)originalHeader.GetRelease() << "\n";
    std::cout << "GroupAssignment: " << originalHeader.GetGroupAssignment() << "\n";
    std::cout << "Paging: " << originalHeader.GetPaging() << "\n";
    std::cout << "OperatingModes: 0x" << std::hex << (unsigned int)originalHeader.GetOperatingModes() << "\n";
    std::cout << "Mesh: " << originalHeader.GetMesh() << "\n";
    std::cout << "ScheduledAccessDataTransfer: " << originalHeader.GetScheduledAccessDataTransfer() << "\n";
    std::cout << "MacSecurity: 0x" << std::hex << (unsigned int)originalHeader.GetMacSecurity() << "\n";
    std::cout << "DlcServiceType: 0x" << std::hex << (unsigned int)originalHeader.GetDlcServiceType() << "\n";
    std::cout << "RdPowerClass: 0x" << std::hex << (unsigned int)originalHeader.GetRdPowerClass() << "\n";
    std::cout << "MaxNssFoRx: 0x" << std::hex << (unsigned int)originalHeader.GetMaxNssFoRx() << "\n";
    std::cout << "RxForTxDiversity: 0x" << std::hex << (unsigned int)originalHeader.GetRxForTxDiversity() << "\n";
    std::cout << "RxGain: 0x" << std::hex << (unsigned int)originalHeader.GetRxGain() << "\n";
    std::cout << "MaxMcs: 0x" << std::hex << (unsigned int)originalHeader.GetMaxMcs() << "\n";
    std::cout << "SoftBufferSize: 0x" << std::hex << (unsigned int)originalHeader.GetSoftBufferSize() << "\n";
    std::cout << "NumOfHarqProcesses: 0x" << std::hex << (unsigned int)originalHeader.GetNumOfHarqProcesses() << "\n";
    std::cout << "HarqFeedbackDelay: 0x" << std::hex << (unsigned int)originalHeader.GetHarqFeedbackDelay() << "\n";
    std::cout << "DDelay: " << originalHeader.GetDDelay() << "\n";
    std::cout << "HalfDulp: " << originalHeader.GetHalfDulp() << "\n\n";

    uint32_t sizeBefore = packet.GetSize();
    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();
    std::cout << "Size after adding header: " << sizeAfter << "\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 7, "Size should be 7 bytes");

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();
    std::cout << "Size after removing header: " << sizeAfterRemove << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "NumOfPhyCapabilities: 0x" << std::hex << (unsigned int)deserializedHeader.GetNumOfPhyCapabilities() << "\n";
    std::cout << "Release: 0x" << std::hex << (unsigned int)deserializedHeader.GetRelease() << "\n";
    std::cout << "GroupAssignment: " << deserializedHeader.GetGroupAssignment() << "\n";
    std::cout << "Paging: " << deserializedHeader.GetPaging() << "\n";
    std::cout << "OperatingModes: 0x" << std::hex << (unsigned int)deserializedHeader.GetOperatingModes() << "\n";
    std::cout << "Mesh: " << deserializedHeader.GetMesh() << "\n";
    std::cout << "ScheduledAccessDataTransfer: " << deserializedHeader.GetScheduledAccessDataTransfer() << "\n";
    std::cout << "MacSecurity: 0x" << std::hex << (unsigned int)deserializedHeader.GetMacSecurity() << "\n";
    std::cout << "DlcServiceType: 0x" << std::hex << (unsigned int)deserializedHeader.GetDlcServiceType() << "\n";
    std::cout << "RdPowerClass: 0x" << std::hex << (unsigned int)deserializedHeader.GetRdPowerClass() << "\n";
    std::cout << "MaxNssFoRx: 0x" << std::hex << (unsigned int)deserializedHeader.GetMaxNssFoRx() << "\n";
    std::cout << "RxForTxDiversity: 0x" << std::hex << (unsigned int)deserializedHeader.GetRxForTxDiversity() << "\n";
    std::cout << "RxGain: 0x" << std::hex << (unsigned int)deserializedHeader.GetRxGain() << "\n";
    std::cout << "MaxMcs: 0x" << std::hex << (unsigned int)deserializedHeader.GetMaxMcs() << "\n";
    std::cout << "SoftBufferSize: 0x" << std::hex << (unsigned int)deserializedHeader.GetSoftBufferSize() << "\n";
    std::cout << "NumOfHarqProcesses: 0x" << std::hex << (unsigned int)deserializedHeader.GetNumOfHarqProcesses() << "\n";
    std::cout << "HarqFeedbackDelay: 0x" << std::hex << (unsigned int)deserializedHeader.GetHarqFeedbackDelay() << "\n";
    std::cout << "DDelay: " << deserializedHeader.GetDDelay() << "\n";
    std::cout << "HalfDulp: " << deserializedHeader.GetHalfDulp() << "\n\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNumOfPhyCapabilities(), numOfPhyCapabilities,
                          "NumOfPhyCapabilities mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRelease(), release,
                          "Release mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetGroupAssignment(), groupAssignment,
                          "GroupAssignment mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPaging(), paging,
                          "Paging mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetOperatingModes(), operatingModes,
                          "OperatingModes mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMesh(), mesh,
                          "Mesh mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetScheduledAccessDataTransfer(), scheduledAccessDataTransfer,
                          "ScheduledAccessDataTransfer mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacSecurity(), macSecurity,
                          "MacSecurity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDlcServiceType(), dlcServiceType,
                          "DlcServiceType mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRdPowerClass(), rdPowerClass,
                          "RdPowerClass mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxNssFoRx(), maxNssFoRx,
                          "MaxNssFoRx mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRxForTxDiversity(), rxForTxDiversity,
                          "RxForTxDiversity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRxGain(), rxGain,
                          "RxGain mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxMcs(), maxMcs,
                          "MaxMcs mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSoftBufferSize(), softBufferSize,
                          "SoftBufferSize mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetNumOfHarqProcesses(), numOfHarqProcesses,
                          "NumOfHarqProcesses mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHarqFeedbackDelay(), harqFeedbackDelay,
                          "HarqFeedbackDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDDelay(), dDelay,
                          "DDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHalfDulp(), halfDulp,
                          "HalfDulp mismatch after serialization/deserialization");
}

} // namespace ns3
