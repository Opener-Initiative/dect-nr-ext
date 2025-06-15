#ifndef DECT2020_BEACON_MESSAGE_TEST_CASE_H
#define DECT2020_BEACON_MESSAGE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020BeaconMessageTestCase : public TestCase
{
public:
    Dect2020BeaconMessageTestCase();
    ~Dect2020BeaconMessageTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_BEACON_MESSAGE_TEST_CASE_H */ 