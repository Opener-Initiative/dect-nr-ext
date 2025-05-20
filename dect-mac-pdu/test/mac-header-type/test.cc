#include "test-case.cc"
#include "ns3/test.h"

namespace ns3 {

class MacHeaderTypeTest : public TestSuite
{
    public:
        MacHeaderTypeTest();
        virtual ~MacHeaderTypeTest();
};

MacHeaderTypeTest::MacHeaderTypeTest() 
    : TestSuite("dect2020-mac-header-type-serialization-test", Type::UNIT)
{
    AddTestCase(new SerializationTestCase(), TestCase::Duration::QUICK);
}

MacHeaderTypeTest::~MacHeaderTypeTest()
{
}

static MacHeaderTypeTest macHeaderTypeTest;

} // namespace ns3