#include "dect2020ext-cluster-beacon-message-ie-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageIeSerializationTest : public TestSuite
{
    public:
        Dect2020ExtClusterBeaconMessageIeSerializationTest();
        virtual ~Dect2020ExtClusterBeaconMessageIeSerializationTest();
};

Dect2020ExtClusterBeaconMessageIeSerializationTest::Dect2020ExtClusterBeaconMessageIeSerializationTest() 
    : TestSuite("dect2020ext-cluster-beacon-message-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtClusterBeaconMessageIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtClusterBeaconMessageIeSerializationTest::~Dect2020ExtClusterBeaconMessageIeSerializationTest()
{
}

static Dect2020ExtClusterBeaconMessageIeSerializationTest dect2020ExtClusterBeaconMessageIeSerializationTest;

} // namespace ns3
