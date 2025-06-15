#include "serialization-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020RouteInfoIETest : public TestSuite
{
    public:
        Dect2020RouteInfoIETest();
        virtual ~Dect2020RouteInfoIETest();
};

Dect2020RouteInfoIETest::Dect2020RouteInfoIETest() 
    : TestSuite("dect2020-route-info-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020RouteInfoIETestCase(), TestCase::Duration::QUICK);
}

Dect2020RouteInfoIETest::~Dect2020RouteInfoIETest()
{
}

static Dect2020RouteInfoIETest dect2020RouteInfoIETest;

} // namespace ns3