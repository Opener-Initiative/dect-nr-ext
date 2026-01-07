#ifndef DECT2020_EXT_NETWORK_BEACON_MESSAGE_SERIALIZATION_TEST_CASE_H
#define DECT2020_EXT_NETWORK_BEACON_MESSAGE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtNetworkBeaconMessageSerializationTestCase : public TestCase
{
    public:
        Dect2020ExtNetworkBeaconMessageSerializationTestCase();
        virtual ~Dect2020ExtNetworkBeaconMessageSerializationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif

