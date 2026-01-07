#include "dect2020ext-network-beacon-message-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtNetworkBeaconMessageTest : public TestSuite
{
    public:
        Dect2020ExtNetworkBeaconMessageTest();
        virtual ~Dect2020ExtNetworkBeaconMessageTest();
};

Dect2020ExtNetworkBeaconMessageTest::Dect2020ExtNetworkBeaconMessageTest()
    : TestSuite("dect2020ext-network-beacon-message-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtNetworkBeaconMessageSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtNetworkBeaconMessageTest::~Dect2020ExtNetworkBeaconMessageTest()
{
}

static Dect2020ExtNetworkBeaconMessageTest dect2020ExtNetworkBeaconMessageTest;

} // namespace ns3

