#ifndef DECT2020EXT_UNICAST_HEADER_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_UNICAST_HEADER_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtUnicastHeaderSerializationTestCase : public TestCase
{
public:
    Dect2020ExtUnicastHeaderSerializationTestCase();
    virtual ~Dect2020ExtUnicastHeaderSerializationTestCase();
    virtual void DoSetup();
    virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020EXT_UNICAST_HEADER_SERIALIZATION_TEST_CASE_H
