#ifndef DECT2020_BEACON_HEADER_SERIALIZATION_TEST_CASE_H
#define DECT2020_BEACON_HEADER_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020BeaconHeaderSerializationTestCase : public TestCase
{
public:
    Dect2020BeaconHeaderSerializationTestCase();
    virtual ~Dect2020BeaconHeaderSerializationTestCase();
    virtual void DoSetup();
    virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020_BEACON_HEADER_SERIALIZATION_TEST_CASE_H
