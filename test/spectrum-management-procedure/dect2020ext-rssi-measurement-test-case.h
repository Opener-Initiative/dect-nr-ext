#ifndef DECT2020_EXT_RSSI_MEASUREMENT_TEST_CASE_H
#define DECT2020_EXT_RSSI_MEASUREMENT_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRssiMeasurementTestCase : public TestCase
{
  public:
    Dect2020ExtRssiMeasurementTestCase();
    virtual ~Dect2020ExtRssiMeasurementTestCase();
    
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif // DECT2020_EXT_RSSI_MEASUREMENT_TEST_CASE_H

