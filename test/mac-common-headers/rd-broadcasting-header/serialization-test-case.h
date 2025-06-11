#ifndef DECT2020_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H
#define DECT2020_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020RDBroadcastingHeaderSerializationTestCase : public TestCase
{
public:
    Dect2020RDBroadcastingHeaderSerializationTestCase();
    virtual ~Dect2020RDBroadcastingHeaderSerializationTestCase();
    virtual void DoSetup();
    virtual void DoRun();
};

} // namespace ns3

#endif // DECT2020_RD_BROADCASTING_HEADER_SERIALIZATION_TEST_CASE_H
