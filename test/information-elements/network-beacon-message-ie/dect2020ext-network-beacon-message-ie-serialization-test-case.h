#ifndef DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtNetworkBeaconMessageIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtNetworkBeaconMessageIeSerializationTestCase();
    virtual ~Dect2020ExtNetworkBeaconMessageIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_SERIALIZATION_TEST_CASE_H */ 