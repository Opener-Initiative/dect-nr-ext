#ifndef DECT2020_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H
#define DECT2020_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020DataMacPduSerializationTestCase : public TestCase
{
    public:
        Dect2020DataMacPduSerializationTestCase();
        ~Dect2020DataMacPduSerializationTestCase();
        virtual void DoSetup();
        virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020_DATA_MAC_PDU_SERIALIZATION_TEST_CASE_H