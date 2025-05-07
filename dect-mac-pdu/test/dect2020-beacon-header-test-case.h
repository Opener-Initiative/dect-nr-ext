#ifndef DECT2020_BEACON_HEADER_TEST_CASE_H
#define DECT2020_BEACON_HEADER_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020BeaconHeaderTestCase : public TestCase
{
public:
    Dect2020BeaconHeaderTestCase();
    ~Dect2020BeaconHeaderTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_BEACON_HEADER_TEST_CASE_H */ 