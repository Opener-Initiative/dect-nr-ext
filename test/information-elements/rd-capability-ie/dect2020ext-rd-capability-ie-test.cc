#include "dect2020ext-rd-capability-ie-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRdCapabilityIeTest : public TestSuite
{
    public:
        Dect2020ExtRdCapabilityIeTest();
        virtual ~Dect2020ExtRdCapabilityIeTest();
};

Dect2020ExtRdCapabilityIeTest::Dect2020ExtRdCapabilityIeTest()
    : TestSuite("dect2020ext-rd-capability-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRdCapabilityIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRdCapabilityIeTest::~Dect2020ExtRdCapabilityIeTest()
{
}

static Dect2020ExtRdCapabilityIeTest dect2020ExtRdCapabilityIeTest;

} // namespace ns3
