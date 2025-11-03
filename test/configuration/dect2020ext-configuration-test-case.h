#ifndef DECT2020_EXT_CONFIGURATION_TEST_CASE_H
#define DECT2020_EXT_CONFIGURATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtConfigurationTestCase : public TestCase
{
    public:
        Dect2020ExtConfigurationTestCase();
        virtual ~Dect2020ExtConfigurationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif