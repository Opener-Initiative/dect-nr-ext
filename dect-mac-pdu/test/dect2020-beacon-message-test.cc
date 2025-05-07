#include "dect2020-beacon-header-test-case.h"
#include "dect2020-beacon-message-test-case.h"
#include "ns3/test.h"

namespace ns3 {

class Dect2020BeaconMessageTest : public TestSuite
{
    public:
        Dect2020BeaconMessageTest();
        ~Dect2020BeaconMessageTest() override;
};

Dect2020BeaconMessageTest::Dect2020BeaconMessageTest() 
    : TestSuite("dect2020-beacon-message-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020BeaconMessageTestCase(), TestCase::Duration::QUICK);
}

Dect2020BeaconMessageTest::~Dect2020BeaconMessageTest()
{
}

static Dect2020BeaconMessageTest dect2020BeaconMessageTest;

} // namespace ns3