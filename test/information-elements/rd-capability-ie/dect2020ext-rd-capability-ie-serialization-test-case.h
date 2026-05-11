#ifndef DECT2020EXT_RD_CAPABILITY_IE_SERIALIZATION_TEST_CASE_H
#define DECT2020EXT_RD_CAPABILITY_IE_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRdCapabilityIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtRdCapabilityIeSerializationTestCase();
    virtual ~Dect2020ExtRdCapabilityIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_RD_CAPABILITY_IE_SERIALIZATION_TEST_CASE_H */
