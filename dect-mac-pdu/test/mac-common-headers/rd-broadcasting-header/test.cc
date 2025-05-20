#include "test-case.cc"
#include "ns3/test.h"

namespace ns3
{

class RDBroadcastingHeaderTest : public TestSuite
{
    public:
        RDBroadcastingHeaderTest();
        virtual ~RDBroadcastingHeaderTest();
};

RDBroadcastingHeaderTest::RDBroadcastingHeaderTest() : TestSuite("dect2020-rd-broadcasting-header-test", Type::UNIT)
{
    AddTestCase(new SerializationTestCase(), TestCase::Duration::QUICK);
}

RDBroadcastingHeaderTest::~RDBroadcastingHeaderTest()
{
}

static RDBroadcastingHeaderTest rdBroadcastingHeaderTest;

} // namespace ns3