#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020UnicastHeaderTest : public TestSuite
{
    public:
        Dect2020UnicastHeaderTest();
        virtual ~Dect2020UnicastHeaderTest();
};

Dect2020UnicastHeaderTest::Dect2020UnicastHeaderTest() 
    : TestSuite("dect2020-unicast-header-test", Type::UNIT)
{
    AddTestCase(new Dect2020UnicastHeaderSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020UnicastHeaderTest::~Dect2020UnicastHeaderTest()
{
}

static Dect2020UnicastHeaderTest dect2020UnicastHeaderTest;

} // namespace ns3 