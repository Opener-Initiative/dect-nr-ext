#include "test-case.cc"
#include "ns3/test.h"

namespace ns3 {

class Dect2020UnicastHeaderTest : public TestSuite
{
    public:
        Dect2020UnicastHeaderTest();
        ~Dect2020UnicastHeaderTest() override;
};

Dect2020UnicastHeaderTest::Dect2020UnicastHeaderTest() 
    : TestSuite("dect2020-unicast-header-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020UnicastHeaderTestCase(), TestCase::Duration::QUICK);
}

Dect2020UnicastHeaderTest::~Dect2020UnicastHeaderTest()
{
}

static Dect2020UnicastHeaderTest dect2020UnicastHeaderTest;

} // namespace ns3 