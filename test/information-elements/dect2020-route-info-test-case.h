#ifndef DECT2020_ROUTE_INFO_TEST_CASE_H
#define DECT2020_ROUTE_INFO_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020RouteInfoTestCase : public TestCase
{
public:
    Dect2020RouteInfoTestCase();
    ~Dect2020RouteInfoTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_ROUTE_INFO_TEST_CASE_H */ 