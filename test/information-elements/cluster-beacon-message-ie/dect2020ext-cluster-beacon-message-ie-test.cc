#include "dect2020ext-cluster-beacon-message-ie-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageIESerializationTest : public TestSuite
{
    public:
        Dect2020ExtClusterBeaconMessageIESerializationTest();
        virtual ~Dect2020ExtClusterBeaconMessageIESerializationTest();
};

Dect2020ExtClusterBeaconMessageIESerializationTest::Dect2020ExtClusterBeaconMessageIESerializationTest() 
    : TestSuite("dect2020ext-cluster-beacon-message-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtClusterBeaconMessageIESerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtClusterBeaconMessageIESerializationTest::~Dect2020ExtClusterBeaconMessageIESerializationTest()
{
}

static Dect2020ExtClusterBeaconMessageIESerializationTest dect2020ExtClusterBeaconMessageIESerializationTest;

} // namespace ns3

