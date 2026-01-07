#include "dect2020ext-beacon-header-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtBeaconHeaderTest : public TestSuite
{
    public:
        Dect2020ExtBeaconHeaderTest();
        virtual ~Dect2020ExtBeaconHeaderTest();
};

Dect2020ExtBeaconHeaderTest::Dect2020ExtBeaconHeaderTest() 
    : TestSuite("dect2020ext-beacon-header-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtBeaconHeaderSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtBeaconHeaderTest::~Dect2020ExtBeaconHeaderTest()
{
}

static Dect2020ExtBeaconHeaderTest dect2020ExtBeaconHeaderTest;

} // namespace ns3