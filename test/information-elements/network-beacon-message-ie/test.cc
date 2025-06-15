#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020NetworkBeaconMessageIESerializationTest : public TestSuite
{
    public:
        Dect2020NetworkBeaconMessageIESerializationTest();
        virtual ~Dect2020NetworkBeaconMessageIESerializationTest();
};

Dect2020NetworkBeaconMessageIESerializationTest::Dect2020NetworkBeaconMessageIESerializationTest() 
    : TestSuite("dect2020-network-beacon-message-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020NetworkBeaconMessageIESerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020NetworkBeaconMessageIESerializationTest::~Dect2020NetworkBeaconMessageIESerializationTest()
{
}

static Dect2020NetworkBeaconMessageIESerializationTest dect2020NetworkBeaconMessageIESerializationTest;

} // namespace ns3