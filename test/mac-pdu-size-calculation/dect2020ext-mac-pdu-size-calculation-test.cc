#include "dect2020ext-mac-pdu-size-calculation-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtMacPduSizeCalculationTest : public TestSuite
{
    public:
        Dect2020ExtMacPduSizeCalculationTest();
        virtual ~Dect2020ExtMacPduSizeCalculationTest();
};

Dect2020ExtMacPduSizeCalculationTest::Dect2020ExtMacPduSizeCalculationTest()
    : TestSuite("dect2020ext-mac-pdu-size-calculation-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtMacPduSizeCalculationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtMacPduSizeCalculationTest::~Dect2020ExtMacPduSizeCalculationTest()
{
}

static Dect2020ExtMacPduSizeCalculationTest dect2020ExtMacPduSizeCalculationTest;

} // namespace ns3
