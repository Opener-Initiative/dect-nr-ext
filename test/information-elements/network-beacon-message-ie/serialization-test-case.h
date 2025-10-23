#ifndef DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H
#define DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtNetworkBeaconMessageIESerializationTestCase : public TestCase
{
public:
    Dect2020ExtNetworkBeaconMessageIESerializationTestCase();
    virtual ~Dect2020ExtNetworkBeaconMessageIESerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_TEST_CASE_H */ 