#ifndef DECT2020EXT_BEACON_HEADER_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_BEACON_HEADER_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtBeaconHeaderSerializationTestCase : public TestCase
{
public:
    Dect2020ExtBeaconHeaderSerializationTestCase();
    virtual ~Dect2020ExtBeaconHeaderSerializationTestCase();
    virtual void DoSetup();
    virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020EXT_BEACON_HEADER_SERIALIZATION_TEST_CASE_H
