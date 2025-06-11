#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020MacHeaderTypeTest : public TestSuite
{
    public:
        Dect2020MacHeaderTypeTest();
        virtual ~Dect2020MacHeaderTypeTest();
};

Dect2020MacHeaderTypeTest::Dect2020MacHeaderTypeTest() 
    : TestSuite("dect2020-mac-header-type-test", Type::UNIT)
{
    AddTestCase(new Dect2020MacHeaderTypeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020MacHeaderTypeTest::~Dect2020MacHeaderTypeTest()
{
}

static Dect2020MacHeaderTypeTest dect2020MacHeaderTypeTest;

} // namespace ns3