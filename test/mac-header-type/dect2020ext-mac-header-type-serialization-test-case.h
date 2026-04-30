#ifndef DECT2020EXT_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H 

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacHeaderTypeSerializationTestCase : public TestCase
{
    public:
        Dect2020ExtMacHeaderTypeSerializationTestCase();
        virtual ~Dect2020ExtMacHeaderTypeSerializationTestCase();

        void DoSetup() override;
        void DoRun() override;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H