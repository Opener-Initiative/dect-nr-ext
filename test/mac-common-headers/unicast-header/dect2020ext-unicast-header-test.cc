#include "dect2020ext-unicast-header-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtUnicastHeaderTest : public TestSuite
{
    public:
        Dect2020ExtUnicastHeaderTest();
        virtual ~Dect2020ExtUnicastHeaderTest();
};

Dect2020ExtUnicastHeaderTest::Dect2020ExtUnicastHeaderTest() 
    : TestSuite("dect2020ext-unicast-header-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtUnicastHeaderSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtUnicastHeaderTest::~Dect2020ExtUnicastHeaderTest()
{
}

static Dect2020ExtUnicastHeaderTest dect2020ExtUnicastHeaderTest;

} // namespace ns3 