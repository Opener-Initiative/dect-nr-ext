#ifndef DECT2020EXT_MAC_SCHEDULER_TIME_TILL_NEXT_FRAME_TEST_CASE_H
#define DECT2020EXT_MAC_SCHEDULER_TIME_TILL_NEXT_FRAME_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacSchedulerTimeTillNextFrameTestCase : public TestCase
{
    public:
        Dect2020ExtMacSchedulerTimeTillNextFrameTestCase();
        virtual ~Dect2020ExtMacSchedulerTimeTillNextFrameTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_SCHEDULER_TIME_TILL_NEXT_FRAME_TEST_CASE_H
