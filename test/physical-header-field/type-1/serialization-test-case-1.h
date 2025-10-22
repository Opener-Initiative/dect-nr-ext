#ifndef DECT2020_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_1_H
#define DECT2020_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_1_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtPhysicalHeaderFieldSerializationTestCase1 : public TestCase
{
public:
    Dect2020ExtPhysicalHeaderFieldSerializationTestCase1();
    virtual ~Dect2020ExtPhysicalHeaderFieldSerializationTestCase1();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_1_H */ 