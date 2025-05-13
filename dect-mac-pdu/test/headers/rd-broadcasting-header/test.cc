#include "test-case.cc"
#include "ns3/test.h"

namespace ns3
{

class Test : public TestSuite
{
    public:
        Test();
        ~Test() override;
};

Test::Test() : TestSuite("dect2020-rd-broadcasting-header-test", Type::UNIT)
{
    AddTestCase(new SerializationTestCase(), TestCase::Duration::QUICK);
}

Test::~Test()
{
}

static Test test;

} // namespace ns3