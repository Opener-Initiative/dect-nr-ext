#ifndef DECT2020EXT_MAC_SCHEDULER_TIME_TILL_SUBSLOT_TEST_CASE_H
#define DECT2020EXT_MAC_SCHEDULER_TIME_TILL_SUBSLOT_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacSchedulerTimeTillSubslotTestCase : public TestCase
{
    public:
        Dect2020ExtMacSchedulerTimeTillSubslotTestCase();
        virtual ~Dect2020ExtMacSchedulerTimeTillSubslotTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_SCHEDULER_TIME_TILL_SUBSLOT_TEST_CASE_H
