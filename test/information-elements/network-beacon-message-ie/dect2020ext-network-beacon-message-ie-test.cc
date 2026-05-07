#include "dect2020ext-network-beacon-message-ie-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtNetworkBeaconMessageIeSerializationTest : public TestSuite
{
    public:
        Dect2020ExtNetworkBeaconMessageIeSerializationTest();
        virtual ~Dect2020ExtNetworkBeaconMessageIeSerializationTest();
};

Dect2020ExtNetworkBeaconMessageIeSerializationTest::Dect2020ExtNetworkBeaconMessageIeSerializationTest() 
    : TestSuite("dect2020ext-network-beacon-message-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtNetworkBeaconMessageIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtNetworkBeaconMessageIeSerializationTest::~Dect2020ExtNetworkBeaconMessageIeSerializationTest()
{
}

static Dect2020ExtNetworkBeaconMessageIeSerializationTest dect2020ExtNetworkBeaconMessageIeSerializationTest;

} // namespace ns3