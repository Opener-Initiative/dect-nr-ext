#ifndef DECT2020EXT_MAC_PDU_SIZE_CALCULATION_TEST_CASE_H
#define DECT2020EXT_MAC_PDU_SIZE_CALCULATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacPduSizeCalculationTestCase : public TestCase
{
    public:
        Dect2020ExtMacPduSizeCalculationTestCase();
        virtual ~Dect2020ExtMacPduSizeCalculationTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_PDU_SIZE_CALCULATION_TEST_CASE_H
