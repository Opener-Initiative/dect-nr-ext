#include "serialization-test-case.h"
#include "ns3/packet.h"

namespace ns3
{

Dect2020MacMultiplexingHeaderTypeBSerializationTestCase::Dect2020MacMultiplexingHeaderTypeBSerializationTestCase()
  : TestCase("Dect2020MacMultiplexingHeaderTypeBSerializationTestCase")
{
}

Dect2020MacMultiplexingHeaderTypeBSerializationTestCase::~Dect2020MacMultiplexingHeaderTypeBSerializationTestCase()
{
}

void
Dect2020MacMultiplexingHeaderTypeBSerializationTestCase::DoSetup()
{
}

void
Dect2020MacMultiplexingHeaderTypeBSerializationTestCase::DoRun()
{
    Dect2020MacMultiplexingHeaderTypeB originalMuxHeader;
    Dect2020MacMultiplexingHeaderTypeB deserializedMuxHeader;
    Packet packet;

    uint8_t macExtension = 0x01;
    uint8_t length = 0x10;
    uint8_t ieType = 0x01;
    uint8_t payload = 0xFF;

    originalMuxHeader.SetMacExtension(macExtension);
    originalMuxHeader.SetLength(length);
    originalMuxHeader.SetIEType(Dect2020MacMultiplexingHeaderTypeB::OneBytePayloadIEType::RadioDeviceStatusIE);
    originalMuxHeader.InsertSDU(payload);

    // Serialize and deserialize

    packet.AddHeader(originalMuxHeader);
    packet.RemoveHeader(deserializedMuxHeader);

    NS_TEST_ASSERT_MSG_EQ(originalMuxHeader.GetMacExtension(), deserializedMuxHeader.GetMacExtension(), "Mac extension mismatch");
    NS_TEST_ASSERT_MSG_EQ(originalMuxHeader.GetLength(), deserializedMuxHeader.GetLength(), "Length mismatch");
    NS_TEST_ASSERT_MSG_EQ(originalMuxHeader.GetIEType(), deserializedMuxHeader.GetIEType(), "IE type mismatch");
    NS_TEST_ASSERT_MSG_EQ(payload, deserializedMuxHeader.GetSDU(), "SDU mismatch");
}

} // namespace ns3
