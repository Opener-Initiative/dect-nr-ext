#include "dect2020ext-phy-control-field-type-2-serialization-test-case.h"

#include "ns3/dect2020ext-phy-control-field-type-2.h"

#include "ns3/packet.h"

#include <array>
#include <random>

namespace ns3
{

namespace
{

constexpr std::array<Dect2020ExtPhyControlFieldType2::PhyControlFieldType2Format, 2>
    kValidHeaderFormats = {Dect2020ExtPhyControlFieldType2::FORMAT_000,
                           Dect2020ExtPhyControlFieldType2::FORMAT_001};

} // namespace

Dect2020ExtPhyControlFieldType2SerializationTestCase::
    Dect2020ExtPhyControlFieldType2SerializationTestCase()
    : TestCase("Dect2020ExtPhyControlFieldType2SerializationTestCase")
{
}

Dect2020ExtPhyControlFieldType2SerializationTestCase::
    ~Dect2020ExtPhyControlFieldType2SerializationTestCase()
{
}

void
Dect2020ExtPhyControlFieldType2SerializationTestCase::DoSetup()
{
}

void
Dect2020ExtPhyControlFieldType2SerializationTestCase::DoRun()
{
    Dect2020ExtPhyControlFieldType2 originalHeader;
    Dect2020ExtPhyControlFieldType2 deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> headerFormatIdxDist(0, kValidHeaderFormats.size() - 1);
    std::bernoulli_distribution packetLengthTypeDist(0.5);
    std::uniform_int_distribution<unsigned int> packetLengthDist(0, 15);
    std::uniform_int_distribution<unsigned int> shortNetworkIdDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> transmitterIdentityDist(0, 0xFFFF);
    std::uniform_int_distribution<unsigned int> transmitPowerDist(0, 15);
    std::uniform_int_distribution<unsigned int> dfmcsDist(0, 15);
    std::uniform_int_distribution<unsigned int> receiverIdentityDist(0, 0xFFFF);
    std::uniform_int_distribution<unsigned int> numberOfSpatialStreamsDist(0, 3);
    std::uniform_int_distribution<unsigned int> dfRedundancyVersionDist(0, 3);
    std::bernoulli_distribution dfNewDataIndicationDist(0.5);
    std::uniform_int_distribution<unsigned int> dfHarqProcessNumberDist(0, 7);
    std::uniform_int_distribution<unsigned int> feedbackFormatDist(0, 15);
    std::uniform_int_distribution<unsigned int> feedbackInfoDist(0, 0xFFF);

    auto headerFormat = kValidHeaderFormats[headerFormatIdxDist(mt)];
    auto packetLengthType = static_cast<uint8_t>(packetLengthTypeDist(mt));
    auto packetLength = static_cast<uint8_t>(packetLengthDist(mt));
    auto shortNetworkId = static_cast<uint8_t>(shortNetworkIdDist(mt));
    auto transmitterIdentity = static_cast<uint16_t>(transmitterIdentityDist(mt));
    auto transmitPower = static_cast<uint8_t>(transmitPowerDist(mt));
    auto dfmcs = static_cast<uint8_t>(dfmcsDist(mt));
    auto receiverIdentity = static_cast<uint16_t>(receiverIdentityDist(mt));
    auto numberOfSpatialStreams = static_cast<uint8_t>(numberOfSpatialStreamsDist(mt));
    auto dfRedundancyVersion = static_cast<uint8_t>(dfRedundancyVersionDist(mt));
    auto dfNewDataIndication = static_cast<uint8_t>(dfNewDataIndicationDist(mt));
    auto dfHarqProcessNumber = static_cast<uint8_t>(dfHarqProcessNumberDist(mt));
    auto feedbackFormat = static_cast<uint8_t>(feedbackFormatDist(mt));
    auto feedbackInfo = static_cast<uint16_t>(feedbackInfoDist(mt));

    originalHeader.SetHeaderFormat(headerFormat);
    originalHeader.SetPacketLengthType(packetLengthType);
    originalHeader.SetPacketLength(packetLength);
    originalHeader.SetShortNetworkID(shortNetworkId);
    originalHeader.SetTransmitterIdentity(transmitterIdentity);
    originalHeader.SetTransmitPower(transmitPower);
    originalHeader.SetDFMCS(dfmcs);
    originalHeader.SetReceiverIdentity(receiverIdentity);
    originalHeader.SetNumberOfSpatialStreams(numberOfSpatialStreams);
    originalHeader.SetDFRedundancyVersion(dfRedundancyVersion);
    originalHeader.SetDFNewDataIndication(dfNewDataIndication);
    originalHeader.SetDFHARQProcessNumber(dfHarqProcessNumber);
    originalHeader.SetFeedbackFormat(feedbackFormat);
    originalHeader.SetFeedbackInfo(feedbackInfo);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Header format: " << (int)originalHeader.GetHeaderFormat() << "\n";
    std::cout << "Packet length type: " << (int)originalHeader.GetPacketLengthType() << "\n";
    std::cout << "Packet length: " << (int)originalHeader.GetPacketLength() << "\n";
    std::cout << "Short network ID: " << (int)originalHeader.GetShortNetworkID() << "\n";
    std::cout << "Transmitter identity: " << (int)originalHeader.GetTransmitterIdentity() << "\n";
    std::cout << "Transmit power: " << (int)originalHeader.GetTransmitPower() << "\n";
    std::cout << "DF MCS: " << (int)originalHeader.GetDFMCS() << "\n";
    std::cout << "Receiver identity: " << (int)originalHeader.GetReceiverIdentity() << "\n";
    std::cout << "Number of spatial streams: " << (int)originalHeader.GetNumberOfSpatialStreams()
              << "\n";
    if (headerFormat == Dect2020ExtPhyControlFieldType2::FORMAT_000)
    {
        std::cout << "DF redundancy version: " << (int)originalHeader.GetDFRedundancyVersion()
                  << "\n";
        std::cout << "DF new data indication: " << (int)originalHeader.GetDFNewDataIndication()
                  << "\n";
        std::cout << "DF HARQ process number: " << (int)originalHeader.GetDFHARQProcessNumber()
                  << "\n";
    }
    std::cout << "Feedback format: " << (int)originalHeader.GetFeedbackFormat() << "\n";
    std::cout << "Feedback info: " << (int)originalHeader.GetFeedbackInfo() << "\n";

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    packet.AddHeader(originalHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 10, "Should be 10");

    packet.RemoveHeader(deserializedHeader);

    NS_TEST_ASSERT_MSG_EQ(packet.GetSize(), 0, "Should be 0");

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Header format: " << (int)deserializedHeader.GetHeaderFormat() << "\n";
    std::cout << "Packet length type: " << (int)deserializedHeader.GetPacketLengthType() << "\n";
    std::cout << "Packet length: " << (int)deserializedHeader.GetPacketLength() << "\n";
    std::cout << "Short network ID: " << (int)deserializedHeader.GetShortNetworkID() << "\n";
    std::cout << "Transmitter identity: " << (int)deserializedHeader.GetTransmitterIdentity()
              << "\n";
    std::cout << "Transmit power: " << (int)deserializedHeader.GetTransmitPower() << "\n";
    std::cout << "DF MCS: " << (int)deserializedHeader.GetDFMCS() << "\n";
    std::cout << "Receiver identity: " << (int)deserializedHeader.GetReceiverIdentity() << "\n";
    std::cout << "Number of spatial streams: "
              << (int)deserializedHeader.GetNumberOfSpatialStreams() << "\n";
    if (deserializedHeader.GetHeaderFormat() == Dect2020ExtPhyControlFieldType2::FORMAT_000)
    {
        std::cout << "DF redundancy version: " << (int)deserializedHeader.GetDFRedundancyVersion()
                  << "\n";
        std::cout << "DF new data indication: " << (int)deserializedHeader.GetDFNewDataIndication()
                  << "\n";
        std::cout << "DF HARQ process number: " << (int)deserializedHeader.GetDFHARQProcessNumber()
                  << "\n";
    }
    std::cout << "Feedback format: " << (int)deserializedHeader.GetFeedbackFormat() << "\n";
    std::cout << "Feedback info: " << (int)deserializedHeader.GetFeedbackInfo() << "\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetHeaderFormat(),
                          originalHeader.GetHeaderFormat(),
                          "Header format mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLengthType(),
                          originalHeader.GetPacketLengthType(),
                          "Packet length type mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetPacketLength(),
                          originalHeader.GetPacketLength(),
                          "Packet length mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetShortNetworkID(),
                          originalHeader.GetShortNetworkID(),
                          "Short network ID mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitterIdentity(),
                          originalHeader.GetTransmitterIdentity(),
                          "Transmitter identity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetTransmitPower(),
                          originalHeader.GetTransmitPower(),
                          "Transmit power mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDFMCS(),
                          originalHeader.GetDFMCS(),
                          "DF MCS mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetReceiverIdentity(),
                          originalHeader.GetReceiverIdentity(),
                          "Receiver identity mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(
        deserializedHeader.GetNumberOfSpatialStreams(),
        originalHeader.GetNumberOfSpatialStreams(),
        "Number of spatial streams mismatch after serialization/deserialization");

    if (headerFormat == Dect2020ExtPhyControlFieldType2::FORMAT_000)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDFRedundancyVersion(),
                              originalHeader.GetDFRedundancyVersion(),
                              "DF redundancy version mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(
            deserializedHeader.GetDFNewDataIndication(),
            originalHeader.GetDFNewDataIndication(),
            "DF new data indication mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(
            deserializedHeader.GetDFHARQProcessNumber(),
            originalHeader.GetDFHARQProcessNumber(),
            "DF HARQ process number mismatch after serialization/deserialization");
    }

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFeedbackFormat(),
                          originalHeader.GetFeedbackFormat(),
                          "Feedback format mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetFeedbackInfo(),
                          originalHeader.GetFeedbackInfo(),
                          "Feedback info mismatch after serialization/deserialization");
}

} // namespace ns3
