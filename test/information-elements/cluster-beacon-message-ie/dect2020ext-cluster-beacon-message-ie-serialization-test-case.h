#ifndef DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H
#define DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageIESerializationTestCase : public TestCase
{
public:
    Dect2020ExtClusterBeaconMessageIESerializationTestCase();
    virtual ~Dect2020ExtClusterBeaconMessageIESerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H */

