#ifndef DECT2020EXT_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtPhysicalHeaderFieldSerializationTestCase : public TestCase
{
public:
    Dect2020ExtPhysicalHeaderFieldSerializationTestCase();
    virtual ~Dect2020ExtPhysicalHeaderFieldSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_PHYSICAL_HEADER_FIELD_SERIALIZATION_TEST_CASE_H */ 