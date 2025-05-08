#include "dect2020-physical-header-field-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020PhysicalHeaderFieldTest : public TestSuite
{
    public:
        Dect2020PhysicalHeaderFieldTest();
        ~Dect2020PhysicalHeaderFieldTest() override;
};

Dect2020PhysicalHeaderFieldTest::Dect2020PhysicalHeaderFieldTest() 
    : TestSuite("dect2020-physical-header-field-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020PhysicalHeaderFieldTestCase(), TestCase::Duration::QUICK);
}

Dect2020PhysicalHeaderFieldTest::~Dect2020PhysicalHeaderFieldTest()
{
}

static Dect2020PhysicalHeaderFieldTest dect2020PhysicalHeaderFieldTest;

} // namespace ns3 