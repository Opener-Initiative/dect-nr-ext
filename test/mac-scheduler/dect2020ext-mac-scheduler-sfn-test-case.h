#ifndef DECT2020EXT_MAC_SCHEDULER_SFN_TEST_CASE_H
#define DECT2020EXT_MAC_SCHEDULER_SFN_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacSchedulerSfnTestCase : public TestCase
{
    public:
        Dect2020ExtMacSchedulerSfnTestCase();
        virtual ~Dect2020ExtMacSchedulerSfnTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_SCHEDULER_SFN_TEST_CASE_H
