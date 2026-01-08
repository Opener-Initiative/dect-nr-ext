#ifndef DECT2020_EXT_CLUSTER_BEACON_MESSAGE_SERIALIZATION_TEST_CASE_H
#define DECT2020_EXT_CLUSTER_BEACON_MESSAGE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtClusterBeaconMessageSerializationTestCase : public TestCase
{
    public:
        Dect2020ExtClusterBeaconMessageSerializationTestCase();
        virtual ~Dect2020ExtClusterBeaconMessageSerializationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif

