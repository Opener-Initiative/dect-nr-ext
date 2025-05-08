#ifndef DECT2020_UNICAST_HEADER_TEST_CASE_H
#define DECT2020_UNICAST_HEADER_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020UnicastHeaderTestCase : public TestCase
{
public:
    Dect2020UnicastHeaderTestCase();
    ~Dect2020UnicastHeaderTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_UNICAST_HEADER_TEST_CASE_H */ 