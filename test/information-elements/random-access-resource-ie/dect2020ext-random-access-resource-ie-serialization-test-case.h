#ifndef DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_TEST_CASE_H
#define DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIeSerializationTestCase : public TestCase
{
public:
    Dect2020ExtRandomAccessResourceIeSerializationTestCase();
    virtual ~Dect2020ExtRandomAccessResourceIeSerializationTestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_TEST_CASE_H */

