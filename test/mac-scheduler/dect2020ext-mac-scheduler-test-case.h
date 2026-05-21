#ifndef DECT2020EXT_MAC_SCHEDULER_TEST_CASE_H
#define DECT2020EXT_MAC_SCHEDULER_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacSchedulerTestCase : public TestCase
{
    public:
        Dect2020ExtMacSchedulerTestCase();
        virtual ~Dect2020ExtMacSchedulerTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_SCHEDULER_TEST_CASE_H
