#ifndef DECT2020EXT_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtDataMacPduSerializationTestCase : public TestCase
{
    public:
        Dect2020ExtDataMacPduSerializationTestCase();
        ~Dect2020ExtDataMacPduSerializationTestCase();
        virtual void DoSetup();
        virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020EXT_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H