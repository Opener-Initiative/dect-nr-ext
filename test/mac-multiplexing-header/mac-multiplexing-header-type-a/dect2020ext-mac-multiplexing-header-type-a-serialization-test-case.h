#ifndef DECT2020_EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_SERIALIZATION_TEST_CASE_H
#define DECT2020_EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase : public TestCase
{
    public:
        Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase();
        virtual ~Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase();
        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif

