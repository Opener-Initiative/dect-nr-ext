#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020BeaconHeaderTest : public TestSuite
{
    public:
        Dect2020BeaconHeaderTest();
        virtual ~Dect2020BeaconHeaderTest();
};

Dect2020BeaconHeaderTest::Dect2020BeaconHeaderTest() 
    : TestSuite("dect2020-beacon-header-test", Type::UNIT)
{
    AddTestCase(new Dect2020BeaconHeaderSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020BeaconHeaderTest::~Dect2020BeaconHeaderTest()
{
}

static Dect2020BeaconHeaderTest dect2020BeaconHeaderTest;

} // namespace ns3