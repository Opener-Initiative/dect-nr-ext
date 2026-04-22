#include "dect2020ext-rd-broadcasting-header-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRdBroadcastingHeaderTest : public TestSuite
{
    public:
        Dect2020ExtRdBroadcastingHeaderTest();
        virtual ~Dect2020ExtRdBroadcastingHeaderTest();
};

Dect2020ExtRdBroadcastingHeaderTest::Dect2020ExtRdBroadcastingHeaderTest()
    : TestSuite("dect2020ext-rd-broadcasting-header-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRdBroadcastingHeaderSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRdBroadcastingHeaderTest::~Dect2020ExtRdBroadcastingHeaderTest()
{
}

static Dect2020ExtRdBroadcastingHeaderTest dect2020ExtRdBroadcastingHeaderTest;

} // namespace ns3