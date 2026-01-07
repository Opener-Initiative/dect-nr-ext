#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtNetworkBeaconMessageIESerializationTest : public TestSuite
{
    public:
        Dect2020ExtNetworkBeaconMessageIESerializationTest();
        virtual ~Dect2020ExtNetworkBeaconMessageIESerializationTest();
};

Dect2020ExtNetworkBeaconMessageIESerializationTest::Dect2020ExtNetworkBeaconMessageIESerializationTest() 
    : TestSuite("dect2020ext-network-beacon-message-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtNetworkBeaconMessageIESerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtNetworkBeaconMessageIESerializationTest::~Dect2020ExtNetworkBeaconMessageIESerializationTest()
{
}

static Dect2020ExtNetworkBeaconMessageIESerializationTest dect2020ExtNetworkBeaconMessageIESerializationTest;

} // namespace ns3