#include "dect2020ext-mac-header-type-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtMacHeaderTypeTest : public TestSuite
{
    public:
        Dect2020ExtMacHeaderTypeTest();
        virtual ~Dect2020ExtMacHeaderTypeTest();
};

Dect2020ExtMacHeaderTypeTest::Dect2020ExtMacHeaderTypeTest() 
    : TestSuite("dect2020ext-mac-header-type-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtMacHeaderTypeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtMacHeaderTypeTest::~Dect2020ExtMacHeaderTypeTest()
{
}

static Dect2020ExtMacHeaderTypeTest dect2020ExtMacHeaderTypeTest;

} // namespace ns3