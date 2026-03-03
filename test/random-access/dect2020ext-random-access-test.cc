#include "dect2020ext-random-access-test-case1.h"
#include "dect2020ext-random-access-test-case2.h"
#include "dect2020ext-random-access-test-case3.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessTest : public TestSuite
{
    public:
        Dect2020ExtRandomAccessTest();
        ~Dect2020ExtRandomAccessTest() override;
};

Dect2020ExtRandomAccessTest::Dect2020ExtRandomAccessTest()
    : TestSuite("dect2020ext-random-access-test", Type::UNIT)
{
    // AddTestCase(new Dect2020ExtRandomAccessTestCase1(), TestCase::Duration::QUICK);
    // AddTestCase(new Dect2020ExtRandomAccessTestCase2(), TestCase::Duration::QUICK);
    AddTestCase(new Dect2020ExtRandomAccessTestCase3(), TestCase::Duration::QUICK);
}

Dect2020ExtRandomAccessTest::~Dect2020ExtRandomAccessTest()
{
}

static Dect2020ExtRandomAccessTest g_dect2020ExtRandomAccessTest;

} // namespace ns3
