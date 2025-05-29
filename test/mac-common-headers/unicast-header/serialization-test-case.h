#ifndef UNICAST_SERIALIZATION_TEST_CASE_H
#define UNICAST_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class SerializationTestCase : public TestCase
{
public:
    SerializationTestCase();
    virtual ~SerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif // UNICAST_SERIALIZATION_TEST_CASE_H
