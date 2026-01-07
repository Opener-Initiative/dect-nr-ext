#include "dect2020ext-physical-header-field-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtPhysicalHeaderFieldTest : public TestSuite
{
    public:
        Dect2020ExtPhysicalHeaderFieldTest();
        virtual ~Dect2020ExtPhysicalHeaderFieldTest();
};

Dect2020ExtPhysicalHeaderFieldTest::Dect2020ExtPhysicalHeaderFieldTest() 
    : TestSuite("dect2020ext-physical-header-field-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtPhysicalHeaderFieldSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtPhysicalHeaderFieldTest::~Dect2020ExtPhysicalHeaderFieldTest()
{
}

static Dect2020ExtPhysicalHeaderFieldTest dect2020ExtPhysicalHeaderFieldTest;

} // namespace ns3 