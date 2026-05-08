#ifndef DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H
#define DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtClusterBeaconMessageIeSerializationTestCase();
    virtual ~Dect2020ExtClusterBeaconMessageIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_TEST_CASE_H */
