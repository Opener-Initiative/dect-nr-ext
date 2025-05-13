#include "dect2020-mac-header-type-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020MacHeaderTypeTest : public TestSuite
{
    public:
        Dect2020MacHeaderTypeTest();
        ~Dect2020MacHeaderTypeTest() override;
};

Dect2020MacHeaderTypeTest::Dect2020MacHeaderTypeTest() 
    : TestSuite("dect2020-mac-header-type-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020MacHeaderTypeTestCase(), TestCase::Duration::QUICK);
}

Dect2020MacHeaderTypeTest::~Dect2020MacHeaderTypeTest()
{
}

static Dect2020MacHeaderTypeTest dect2020MacHeaderTypeTest;

} // namespace ns3