#ifndef DECT2020_EXT_NETWORK_BEACON_PERIOD_TEST_CASE_H
#define DECT2020_EXT_NETWORK_BEACON_PERIOD_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtNetworkBeaconPeriodTestCase : public TestCase
{
  public:
    Dect2020ExtNetworkBeaconPeriodTestCase();
    virtual ~Dect2020ExtNetworkBeaconPeriodTestCase();
    
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif // DECT2020_EXT_NETWORK_BEACON_PERIOD_TEST_CASE_H