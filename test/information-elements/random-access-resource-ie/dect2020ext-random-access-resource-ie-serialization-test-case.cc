#include "dect2020ext-random-access-resource-ie-serialization-test-case.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"
#include "ns3/packet.h"
#include "ns3/test.h"

namespace ns3
{

Dect2020ExtRandomAccessResourceIESerializationTestCase::Dect2020ExtRandomAccessResourceIESerializationTestCase() 
    : TestCase("RandomAccessResourceIESerializationTest")
{
}

Dect2020ExtRandomAccessResourceIESerializationTestCase::~Dect2020ExtRandomAccessResourceIESerializationTestCase()
{
}

void
Dect2020ExtRandomAccessResourceIESerializationTestCase::DoSetup()
{
}

void
Dect2020ExtRandomAccessResourceIESerializationTestCase::DoRun()
{
    Dect2020ExtRandomAccessResourceIE originalHeader;
    Dect2020ExtRandomAccessResourceIE deserializedHeader;
    Packet packet;

    originalHeader.SetRepeat(0);
    originalHeader.SetSystemFrameNumberFieldIncluded(0);
    originalHeader.SetChannelFieldIncluded(0);
    originalHeader.SetSeparateChannelFieldIncluded(0);

    originalHeader.SetStartSubslot((uint8_t)22);
    originalHeader.SetLengthType(0);
    originalHeader.SetRaraLength(10);
    originalHeader.SetMaxRachLengthType(0);
    originalHeader.SetMaxRachLength(2);
    originalHeader.SetCwMinSig(0);
    originalHeader.SetDectDelay(false);
    originalHeader.SetResponseWindow(5);
    originalHeader.SetCwMaxSig(0);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Repeat: " << (int)originalHeader.GetRepeat() << "\n";
    std::cout << "SystemFrameNumberFieldIncluded: " << originalHeader.GetSystemFrameNumberFieldIncluded() << "\n";
    std::cout << "ChannelFieldIncluded: " << originalHeader.GetChannelFieldIncluded() << "\n";
    std::cout << "SeparateChannelFieldIncluded: " << originalHeader.GetSeparateChannelFieldIncluded() << "\n";
    std::cout << "StartSubslot: " << originalHeader.GetStartSubslot() << "\n";
    std::cout << "LengthType: " << originalHeader.GetLengthType() << "\n";
    std::cout << "RaraLength: " << (int)originalHeader.GetRaraLength() << "\n";
    std::cout << "MaxRachLengthType: " << originalHeader.GetMaxRachLengthType() << "\n";
    std::cout << "MaxRachLength: " << (int)originalHeader.GetMaxRachLength() << "\n";
    std::cout << "CwMinSig: " << (int)originalHeader.GetCwMinSig() << "\n";
    std::cout << "DectDelay: " << originalHeader.GetDectDelay() << "\n";
    std::cout << "ResponseWindow: " << (int)originalHeader.GetResponseWindow() << "\n";
    std::cout << "CwMaxSig: " << (int)originalHeader.GetCwMaxSig() << "\n";
    // std::cout << "Repetition: 0x" << std::hex << (int)originalHeader.GetRepetition() << "\n";
    // std::cout << "Validity: 0x" << std::hex << (int)originalHeader.GetValidity() << "\n";
    // std::cout << "SystemFrameNumberValue: 0x" << std::hex << (int)originalHeader.GetSystemFrameNumberValue() << "\n";
    // std::cout << "ChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << originalHeader.GetChannelAbsoluteCarrierCenterFrequency() << "\n";
    // std::cout << "SeparateChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << originalHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency() << "\n" << "\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Repeat: " << deserializedHeader.GetRepeat() << "\n";
    std::cout << "SystemFrameNumberFieldIncluded: " << deserializedHeader.GetSystemFrameNumberFieldIncluded() << "\n";
    std::cout << "ChannelFieldIncluded: " << deserializedHeader.GetChannelFieldIncluded() << "\n";
    std::cout << "SeparateChannelFieldIncluded: " << deserializedHeader.GetSeparateChannelFieldIncluded() << "\n";
    std::cout << "StartSubslot: " << deserializedHeader.GetStartSubslot() << "\n";
    std::cout << "LengthType: " << deserializedHeader.GetLengthType() << "\n";
    std::cout << "RaraLength: " << deserializedHeader.GetRaraLength() << "\n";
    std::cout << "MaxRachLengthType: " << deserializedHeader.GetMaxRachLengthType() << "\n";
    std::cout << "MaxRachLength: " << deserializedHeader.GetMaxRachLength() << "\n";
    std::cout << "CwMinSig: " << deserializedHeader.GetCwMinSig() << "\n";
    std::cout << "DectDelay: " << deserializedHeader.GetDectDelay() << "\n";
    std::cout << "ResponseWindow: " << deserializedHeader.GetResponseWindow() << "\n";
    std::cout << "CwMaxSig: " << deserializedHeader.GetCwMaxSig() << "\n";
    // std::cout << "Repetition: 0x" << std::hex << (int)deserializedHeader.GetRepetition() << "\n";
    // std::cout << "Validity: 0x" << std::hex << (int)deserializedHeader.GetValidity() << "\n";
    // std::cout << "SystemFrameNumberValue: 0x" << std::hex << (int)deserializedHeader.GetSystemFrameNumberValue() << "\n";
    // std::cout << "ChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << deserializedHeader.GetChannelAbsoluteCarrierCenterFrequency() << "\n";
    // std::cout << "SeparateChannelAbsoluteCarrierCenterFrequency: 0x" << std::hex << deserializedHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency() << "\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRepeat(), 
                         originalHeader.GetRepeat(),
                         "Repeat mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSystemFrameNumberFieldIncluded(), 
                         originalHeader.GetSystemFrameNumberFieldIncluded(),
                         "SystemFrameNumberFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetChannelFieldIncluded(), 
                         originalHeader.GetChannelFieldIncluded(),
                         "ChannelFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSeparateChannelFieldIncluded(), 
                         originalHeader.GetSeparateChannelFieldIncluded(),
                         "SeparateChannelFieldIncluded mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetStartSubslot(), 
                         originalHeader.GetStartSubslot(),
                         "StartSubslot mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetLengthType(), 
                         originalHeader.GetLengthType(),
                         "LengthType mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRaraLength(), 
                         originalHeader.GetRaraLength(),
                         "RaraLength mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxRachLengthType(), 
                         originalHeader.GetMaxRachLengthType(),
                         "MaxRachLengthType mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMaxRachLength(), 
                         originalHeader.GetMaxRachLength(),
                         "MaxRachLength mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCwMinSig(), 
                         originalHeader.GetCwMinSig(),
                         "CwMinSig mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetDectDelay(), 
                         originalHeader.GetDectDelay(),
                         "DectDelay mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetResponseWindow(), 
                         originalHeader.GetResponseWindow(),
                         "ResponseWindow mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetCwMaxSig(), 
                         originalHeader.GetCwMaxSig(),
                         "CwMaxSig mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRepetition(), 
    //                      originalHeader.GetRepetition(),
    //                      "Repetition mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetValidity(), 
    //                      originalHeader.GetValidity(),
    //                      "Validity mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSystemFrameNumberValue(), 
    //                      originalHeader.GetSystemFrameNumberValue(),
    //                      "SystemFrameNumberValue mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetChannelAbsoluteCarrierCenterFrequency(), 
    //                      originalHeader.GetChannelAbsoluteCarrierCenterFrequency(),
    //                      "ChannelAbsoluteCarrierCenterFrequency mismatch after serialization/deserialization");

    // NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency(), 
    //                      originalHeader.GetSeparateChannelAbsoluteCarrierCenterFrequency(),
    //                      "SeparateChannelAbsoluteCarrierCenterFrequency mismatch after serialization/deserialization");
}

} // namespace ns3

