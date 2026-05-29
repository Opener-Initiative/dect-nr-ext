#include "payload-insertion-test-case.h"

namespace ns3
{

Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase::Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase()
  : TestCase("Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase")
{
}

Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase::~Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase()
{
}

void
Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase::DoSetup()
{
}

void
Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase::DoRun()
{
    Dect2020MacMultiplexingHeaderTypeB muxHeader;
    uint8_t originalPayload = 0xFF;
    uint8_t removedPayload = 0x00;

    muxHeader.InsertSDU(originalPayload);
    std::cout << "Inserted payload: " << std::hex << muxHeader.GetSDU() << "\n";
    removedPayload = muxHeader.RemoveSDU();
    std::cout << "Removed payload: " << std::hex << removedPayload << "\n";

    NS_TEST_ASSERT_MSG_EQ(originalPayload, removedPayload, "Payload insertion and removal failed");
}

} // namespace ns3
