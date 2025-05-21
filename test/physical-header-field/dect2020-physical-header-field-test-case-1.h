#ifndef DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_1_H
#define DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_1_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020PhysicalHeaderFieldTestCase1 : public TestCase
{
public:
    Dect2020PhysicalHeaderFieldTestCase1();
    ~Dect2020PhysicalHeaderFieldTestCase1() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_PHYSICAL_HEADER_FIELD_TEST_CASE_1_H */ 