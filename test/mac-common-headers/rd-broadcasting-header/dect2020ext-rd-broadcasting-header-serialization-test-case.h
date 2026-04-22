#ifndef DECT2020EXT_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020ExtRdBroadcastingHeaderSerializationTestCase : public TestCase
{
public:
    Dect2020ExtRdBroadcastingHeaderSerializationTestCase();
    virtual ~Dect2020ExtRdBroadcastingHeaderSerializationTestCase();
    virtual void DoSetup();
    virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020EXT_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H
