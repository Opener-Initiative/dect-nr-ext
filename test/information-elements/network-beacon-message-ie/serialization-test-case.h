#ifndef DECT2020_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H
#define DECT2020_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020NetworkBeaconMessageIESerializationTestCase : public TestCase
{
public:
    Dect2020NetworkBeaconMessageIESerializationTestCase();
    virtual ~Dect2020NetworkBeaconMessageIESerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H */ 