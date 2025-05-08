#ifndef DECT2020_MAC_HEADER_TYPE_TEST_CASE_H
#define DECT2020_MAC_HEADER_TYPE_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020MacHeaderTypeTestCase : public TestCase
{
public:
    Dect2020MacHeaderTypeTestCase();
    ~Dect2020MacHeaderTypeTestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_MAC_HEADER_TYPE_TEST_CASE_H */ 