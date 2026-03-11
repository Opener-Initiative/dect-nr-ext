#include "dect2020ext-phy-test-case-1.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtPhyTest : public TestSuite
{
  public:
    Dect2020ExtPhyTest();
    ~Dect2020ExtPhyTest() override;
};

Dect2020ExtPhyTest::Dect2020ExtPhyTest()
    : TestSuite("dect2020ext-phy-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtPhyTestCase1(), TestCase::Duration::QUICK);
}

Dect2020ExtPhyTest::~Dect2020ExtPhyTest()
{
}

static Dect2020ExtPhyTest g_dect2020ExtPhyTest;

} // namespace ns3
