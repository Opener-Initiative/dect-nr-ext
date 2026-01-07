#include "dect2020ext-mac-multiplexing-header-type-a-serialization-test-case.h"

#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include <ostream>

namespace ns3
{

Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase()
    : TestCase("Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase")
{
}

Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::~Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase()
{
}

void
Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::DoSetup()
{
}

void
Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase::DoRun()
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020ExtMacMultiplexingHeaderTypeA originalHeader;
    Dect2020ExtMacMultiplexingHeaderTypeA deserializedHeader;

    originalHeader.SetMacExtensionFieldEncoding(0);
    originalHeader.SetLengthField(0);
    originalHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    uint32_t sizeBefore = packet->GetSize();

    std::cout << "Packet size before adding header: " << sizeBefore << std::endl;

    packet->AddHeader(originalHeader);

    uint32_t sizeAfter = packet->GetSize();

    std::cout << "Packet size after adding header: " << sizeAfter << std::endl;

    NS_TEST_ASSERT_MSG_EQ(sizeAfter, 1, "After adding header packet size should be 1");

    packet->RemoveHeader(deserializedHeader);

    uint32_t sizeAfterRemove = packet->GetSize();

    std::cout << "Packet size after removing header: " << sizeAfterRemove << std::endl;

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "After removing header packet size should be 0");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetMacExtensionFieldEncoding(), 
                          originalHeader.GetMacExtensionFieldEncoding(),
                          "Mac extension field encoding should be the same as the original header");
}

} // namespace ns3