#include "dect2020ext-configuration-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtConfigurationTest : public TestSuite
{
    public:
        Dect2020ExtConfigurationTest();
        virtual ~Dect2020ExtConfigurationTest();
};

Dect2020ExtConfigurationTest::Dect2020ExtConfigurationTest()
    : TestSuite("dect2020ext-configuration-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtConfigurationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtConfigurationTest::~Dect2020ExtConfigurationTest()
{
}

static Dect2020ExtConfigurationTest dect2020ExtConfigurationTest;

} // namespace ns3