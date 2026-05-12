#include "dect2020ext-cluster-beacon-message-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageTest : public TestSuite
{
    public:
        Dect2020ExtClusterBeaconMessageTest();
        virtual ~Dect2020ExtClusterBeaconMessageTest();
};

Dect2020ExtClusterBeaconMessageTest::Dect2020ExtClusterBeaconMessageTest()
    : TestSuite("dect2020ext-cluster-beacon-message-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtClusterBeaconMessageSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtClusterBeaconMessageTest::~Dect2020ExtClusterBeaconMessageTest()
{
}

static Dect2020ExtClusterBeaconMessageTest dect2020ExtClusterBeaconMessageTest;

} // namespace ns3

