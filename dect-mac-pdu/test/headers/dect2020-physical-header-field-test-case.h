#ifndef DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_H
#define DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020PhysicalHeaderFieldTestCase : public TestCase
{
public:
    Dect2020PhysicalHeaderFieldTestCase();
    ~Dect2020PhysicalHeaderFieldTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_H */ 