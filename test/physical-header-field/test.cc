#include "serialization-test-case-1.h"
#include "serialization-test-case-2.h"
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
    AddTestCase(new Dect2020PhysicalHeaderFieldSerializationTestCase1(), TestCase::Duration::QUICK);
    AddTestCase(new Dect2020PhysicalHeaderFieldSerializationTestCase2(), TestCase::Duration::QUICK);
}

Dect2020PhysicalHeaderFieldTest::~Dect2020PhysicalHeaderFieldTest()
{
}

static Dect2020PhysicalHeaderFieldTest dect2020PhysicalHeaderFieldTest;

} // namespace ns3 