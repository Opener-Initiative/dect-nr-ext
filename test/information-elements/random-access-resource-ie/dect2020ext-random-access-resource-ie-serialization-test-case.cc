#include "dect2020ext-random-access-resource-ie-serialization-test-case.h"

#include "ns3/dect2020ext-random-access-resource-ie.h"

#include "ns3/packet.h"

#include <random>

namespace ns3
{

Dect2020ExtRandomAccessResourceIeSerializationTestCase::Dect2020ExtRandomAccessResourceIeSerializationTestCase()
    : TestCase("Dect2020ExtRandomAccessResourceIeSerializationTestCase")
{
}

Dect2020ExtRandomAccessResourceIeSerializationTestCase::~Dect2020ExtRandomAccessResourceIeSerializationTestCase()
{
}

void
Dect2020ExtRandomAccessResourceIeSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtRandomAccessResourceIeSerializationTestCase::DoRun()
{
    Dect2020ExtRandomAccessResourceIe originalHeader;
    Dect2020ExtRandomAccessResourceIe deserializedHeader;
    Packet packet;

    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<unsigned int> repeatDist(0, 3);
    std::bernoulli_distribution sfnIncludedDist(0.5);
    std::bernoulli_distribution channelIncludedDist(0.5);
    std::bernoulli_distribution separateChannelIncludedDist(0.5);
    std::uniform_int_distribution<unsigned int> startSubslotDist(0, 0xFF);
    std::bernoulli_distribution lengthTypeDist(0.5);
    std::uniform_int_distribution<unsigned int> raraLengthDist(0, 0x7F);
    std::bernoulli_distribution maxRachLengthTypeDist(0.5);
    std::uniform_int_distribution<unsigned int> maxRachLengthDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> cwMinSigDist(0, 0x07);
    std::bernoulli_distribution dectDelayDist(0.5);
    std::uniform_int_distribution<unsigned int> responseWindowDist(0, 0x0F);
    std::uniform_int_distribution<unsigned int> cwMaxSigDist(0, 0x07);
    std::uniform_int_distribution<unsigned int> repetitionDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> validityDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> sfnValueDist(0, 0xFF);
    std::uniform_int_distribution<unsigned int> channelFreqDist(0, 0x1FFF);
    std::uniform_int_distribution<unsigned int> separateChannelFreqDist(0, 0x1FFF);

    auto repeat = static_cast<uint8_t>(repeatDist(mt));
    bool sfnIncluded = sfnIncludedDist(mt);
    bool channelIncluded = channelIncludedDist(mt);
    bool separateChannelIncluded = separateChannelIncludedDist(mt);
    auto startSubslot = static_cast<uint8_t>(startSubslotDist(mt));
    bool lengthType = lengthTypeDist(mt);
    auto raraLength = static_cast<uint8_t>(raraLengthDist(mt));
    bool maxRachLengthType = maxRachLengthTypeDist(mt);
    auto maxRachLength = static_cast<uint8_t>(maxRachLengthDist(mt));
    auto cwMinSig = static_cast<uint8_t>(cwMinSigDist(mt));
    bool dectDelay = dectDelayDist(mt);
    auto responseWindow = static_cast<uint8_t>(responseWindowDist(mt));
    auto cwMaxSig = static_cast<uint8_t>(cwMaxSigDist(mt));
    auto repetition = static_cast<uint8_t>(repetitionDist(mt));
    auto validity = static_cast<uint8_t>(validityDist(mt));
    auto sfnValue = static_cast<uint8_t>(sfnValueDist(mt));
    auto channelFreq = static_cast<uint16_t>(channelFreqDist(mt));
    auto separateChannelFreq = static_cast<uint16_t>(separateChannelFreqDist(mt));

    originalHeader.SetRepeat(repeat);
    originalHeader.SetSystemFrameNumberFieldIncluded(sfnIncluded);
    originalHeader.SetChannelFieldIncluded(channelIncluded);
    originalHeader.SetSeparateChannelFieldIncluded(separateChannelIncluded);
    originalHeader.SetStartSubslot(startSubslot);
    originalHeader.SetLengthType(lengthType);
    originalHeader.SetRaraLength(raraLength);
    originalHeader.SetMaxRachLengthType(maxRachLengthType);
    originalHeader.SetMaxRachLength(maxRachLength);
    originalHeader.SetCwMinSig(cwMinSig);
    originalHeader.SetDectDelay(dectDelay);
    originalHeader.SetResponseWindow(responseWindow);
    originalHeader.SetCwMaxSig(cwMaxSig);

    if (repeat == 1 || repeat == 2)
    {
        originalHeader.SetRepetition(repetition);
        originalHeader.SetValidity(validity);
    }

    if (sfnIncluded)
    {
        originalHeader.SetSystemFrameNumberValue(sfnValue);
    }

    if (channelIncluded)
    {
        originalHeader.SetChannelAbsoluteCarrierCenterFrequency(channelFreq);
    }

    if (separateChannelIncluded)
    {
        originalHeader.SetSeparateChannelAbsoluteCarrierCenterFrequency(separateChannelFreq);
    }

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Repeat: 0x" << std::hex << (unsigned int)originalHeader.GetRepeat() << "\n";
    std::cout << "SystemFrameNumberFieldIncluded: " << originalHeader.GetSystemFrameNumberFieldIncluded() << "\n";
    std::cout << "ChannelFieldIncluded: " << originalHeader.GetChannelFieldIncluded() << "\n";
    std::cout << "SeparateChannelFieldIncluded: " << originalHeader.GetSeparateChannelFieldIncluded() << "\n";
    std::cout << "StartSubslot: 0x" << std::hex << (unsigned int)originalHeader.GetStartSubslot() << "\n";
    std::cout << "LengthType: " << originalHeader.GetLengthType() << "\n";
    std::cout << "RaraLength: 0x" << std::hex << (unsigned int)originalHeader.GetRaraLength() << "\n";
    std::cout << "MaxRachLengthType: " << originalHeader.GetMaxRachLengthType() << "\n";
    std::cout << "MaxRachLength: 0x" << std::hex << (unsigned int)originalHeader.GetMaxRachLength() << "\n";
    std::cout << "CwMinSig: 0x" << std::hex << (unsigned int)originalHeader.GetCwMinSig() << "\n";
    std::cout << "DectDelay: " << originalHeader.GetDectDelay() << "\n";
    std::cout << "ResponseWindow: 0x" << std::hex << (unsigned int)originalHeader.GetResponseWindow() << "\n";
    std::cout << "CwMaxSig: 0x" << std::hex << (unsigned int)originalHeader.GetCwMaxSig() << "\n";

    if (repeat == 1 || repeat == 2)
    {
        std::cout << "Repetition: 0x" << std::hex << (unsigned int)originalHeader.GetRepetition() << "\n";
        std::cout << "Validity: 0x" << std::hex << (unsigned int)originalHeader.GetValidity() << "\n";
    }

    if (sfnIncluded)
    {
        std::cout << "SystemFrameNumberValue: 0x" << std::hex << (unsigned int)originalHeader.GetSystemFrameNumberValue() << "\n";
    }

    if (channelIncluded)
    {
        std::cout << "ChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << originalHeader.GetChannelAbsoluteCarrierCenterFrequency() << "\n";
    }

    if (separateChannelIncluded)
    {
        std::cout << "SeparateChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << originalHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency() << "\n";
    }

    std::cout << "\n";

    uint32_t expectedSize = originalHeader.GetSerializedSize();
    uint32_t sizeBefore = packet.GetSize();

    std::cout << "Size before adding header: " << sizeBefore << "\n";

    packet.AddHeader(originalHeader);
    uint32_t sizeAfter = packet.GetSize();

    std::cout << "Size after adding header: " << sizeAfter << "\n";

    packet.RemoveHeader(deserializedHeader);
    uint32_t sizeAfterRemove = packet.GetSize();

    std::cout << "Size after removing header: " << sizeAfterRemove << "\n\n";

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Repeat: 0x" << std::hex << (unsigned int)deserializedHeader.GetRepeat() << "\n";
    std::cout << "SystemFrameNumberFieldIncluded: " << deserializedHeader.GetSystemFrameNumberFieldIncluded() << "\n";
    std::cout << "ChannelFieldIncluded: " << deserializedHeader.GetChannelFieldIncluded() << "\n";
    std::cout << "SeparateChannelFieldIncluded: " << deserializedHeader.GetSeparateChannelFieldIncluded() << "\n";
    std::cout << "StartSubslot: 0x" << std::hex << (unsigned int)deserializedHeader.GetStartSubslot() << "\n";
    std::cout << "LengthType: " << deserializedHeader.GetLengthType() << "\n";
    std::cout << "RaraLength: 0x" << std::hex << (unsigned int)deserializedHeader.GetRaraLength() << "\n";
    std::cout << "MaxRachLengthType: " << deserializedHeader.GetMaxRachLengthType() << "\n";
    std::cout << "MaxRachLength: 0x" << std::hex << (unsigned int)deserializedHeader.GetMaxRachLength() << "\n";
    std::cout << "CwMinSig: 0x" << std::hex << (unsigned int)deserializedHeader.GetCwMinSig() << "\n";
    std::cout << "DectDelay: " << deserializedHeader.GetDectDelay() << "\n";
    std::cout << "ResponseWindow: 0x" << std::hex << (unsigned int)deserializedHeader.GetResponseWindow() << "\n";
    std::cout << "CwMaxSig: 0x" << std::hex << (unsigned int)deserializedHeader.GetCwMaxSig() << "\n";

    if (repeat == 1 || repeat == 2)
    {
        std::cout << "Repetition: 0x" << std::hex << (unsigned int)deserializedHeader.GetRepetition() << "\n";
        std::cout << "Validity: 0x" << std::hex << (unsigned int)deserializedHeader.GetValidity() << "\n";
    }

    if (sfnIncluded)
    {
        std::cout << "SystemFrameNumberValue: 0x" << std::hex << (unsigned int)deserializedHeader.GetSystemFrameNumberValue() << "\n";
    }

    if (channelIncluded)
    {
        std::cout << "ChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << deserializedHeader.GetChannelAbsoluteCarrierCenterFrequency() << "\n";
    }

    if (separateChannelIncluded)
    {
        std::cout << "SeparateChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << deserializedHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency() << "\n";
    }

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, expectedSize, "Packet size after adding header mismatch");

    NS_TEST_ASSERT_MSG_EQ(sizeAfterRemove, 0, "Size after removal should be 0");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRepeat(), repeat,
                          "Repeat mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSystemFrameNumberFieldIncluded(), sfnIncluded,
                          "SystemFrameNumberFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetChannelFieldIncluded(), channelIncluded,
                          "ChannelFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSeparateChannelFieldIncluded(), separateChannelIncluded,
                          "SeparateChannelFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetStartSubslot(), startSubslot,
                          "StartSubslot mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetLengthType(), lengthType,
                          "LengthType mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRaraLength(), raraLength,
                          "RaraLength mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxRachLengthType(), maxRachLengthType,
                          "MaxRachLengthType mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxRachLength(), maxRachLength,
                          "MaxRachLength mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCwMinSig(), cwMinSig,
                          "CwMinSig mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDectDelay(), dectDelay,
                          "DectDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetResponseWindow(), responseWindow,
                          "ResponseWindow mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCwMaxSig(), cwMaxSig,
                          "CwMaxSig mismatch after serialization/deserialization");

    if (repeat == 1 || repeat == 2)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRepetition(), repetition,
                              "Repetition mismatch after serialization/deserialization");

        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetValidity(), validity,
                              "Validity mismatch after serialization/deserialization");
    }

    if (sfnIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSystemFrameNumberValue(), sfnValue,
                              "SystemFrameNumberValue mismatch after serialization/deserialization");
    }

    if (channelIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetChannelAbsoluteCarrierCenterFrequency(), channelFreq,
                              "ChannelAbsoluteCarrierCenterFrequency mismatch after serialization/deserialization");
    }

    if (separateChannelIncluded)
    {
        NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency(), separateChannelFreq,
                              "SeparateChannelAbsoluteCarrierCenterFrequency mismatch after serialization/deserialization");
    }
}

} // namespace ns3
