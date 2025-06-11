#ifndef DECT2020_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H
#define DECT2020_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H 

#include "ns3/test.h"

namespace ns3
{

class Dect2020MacHeaderTypeSerializationTestCase : public TestCase
{
    public:
        Dect2020MacHeaderTypeSerializationTestCase();
        virtual ~Dect2020MacHeaderTypeSerializationTestCase();

        virtual void DoSetup();
        virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020_MAC_HEADER_TYPE_SERIALIZATION_TEST_CASE_H