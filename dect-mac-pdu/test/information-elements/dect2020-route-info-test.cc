#include "dect2020-route-info-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020RouteInfoTest : public TestSuite
{
    public:
        Dect2020RouteInfoTest();
        ~Dect2020RouteInfoTest() override;
};

Dect2020RouteInfoTest::Dect2020RouteInfoTest() 
    : TestSuite("dect2020-route-info-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020RouteInfoTestCase(), TestCase::Duration::QUICK);
}

Dect2020RouteInfoTest::~Dect2020RouteInfoTest()
{
}

static Dect2020RouteInfoTest dect2020RouteInfoTest;

} // namespace ns3