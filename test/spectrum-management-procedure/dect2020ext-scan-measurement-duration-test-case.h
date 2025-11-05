#ifndef DECT2020_EXT_SCAN_MEASUREMENT_DURATION_TEST_CASE_H
#define DECT2020_EXT_SCAN_MEASUREMENT_DURATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtScanMeasurementDurationTestCase : public TestCase
{
    public:
        Dect2020ExtScanMeasurementDurationTestCase();
        virtual ~Dect2020ExtScanMeasurementDurationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif