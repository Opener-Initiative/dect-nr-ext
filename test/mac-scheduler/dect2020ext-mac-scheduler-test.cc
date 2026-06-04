#include "dect2020ext-mac-scheduler-test-case.h"
#include "dect2020ext-mac-scheduler-sfn-test-case.h"
#include "dect2020ext-mac-scheduler-time-till-subslot-test-case.h"
#include "dect2020ext-mac-scheduler-time-till-next-frame-test-case.h"

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtMacSchedulerTest : public TestSuite
{
    public:
        Dect2020ExtMacSchedulerTest();
        virtual ~Dect2020ExtMacSchedulerTest();
};

Dect2020ExtMacSchedulerTest::Dect2020ExtMacSchedulerTest()
    : TestSuite("dect2020ext-mac-scheduler-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtMacSchedulerTestCase(), TestCase::Duration::QUICK);
    AddTestCase(new Dect2020ExtMacSchedulerSfnTestCase(), TestCase::Duration::QUICK);
    AddTestCase(new Dect2020ExtMacSchedulerTimeTillSubslotTestCase(), TestCase::Duration::QUICK);
    AddTestCase(new Dect2020ExtMacSchedulerTimeTillNextFrameTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtMacSchedulerTest::~Dect2020ExtMacSchedulerTest()
{
}

static Dect2020ExtMacSchedulerTest dect2020ExtMacSchedulerTest;

} // namespace ns3
