#ifndef DECT2020_ROUTE_INFO_IE_TEST_CASE_H
#define DECT2020_ROUTE_INFO_IE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020RouteInfoIETestCase : public TestCase
{
public:
    Dect2020RouteInfoIETestCase();
    virtual ~Dect2020RouteInfoIETestCase();
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_ROUTE_INFO_IE_TEST_CASE_H */ 