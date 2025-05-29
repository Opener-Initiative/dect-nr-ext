#include "serialization-test-case.cc"
#include "ns3/test.h"

namespace ns3
{

class Dect2020RDBroadcastingHeaderTest : public TestSuite
{
    public:
        Dect2020RDBroadcastingHeaderTest();
        virtual ~Dect2020RDBroadcastingHeaderTest();
};

Dect2020RDBroadcastingHeaderTest::Dect2020RDBroadcastingHeaderTest() : TestSuite("dect2020-rd-broadcasting-header-test", Type::UNIT)
{
    AddTestCase(new SerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020RDBroadcastingHeaderTest::~Dect2020RDBroadcastingHeaderTest()
{
}

static Dect2020RDBroadcastingHeaderTest dect2020RDBroadcastingHeaderTest;

} // namespace ns3