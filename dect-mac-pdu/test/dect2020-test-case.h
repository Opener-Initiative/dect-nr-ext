#ifndef DECT2020_TEST_CASE_H
#define DECT2020_TEST_CASE_H

#include "ns3/test.h"

namespace ns3 {

class Dect2020TestCase : public TestCase
{
public:
    Dect2020TestCase();
    ~Dect2020TestCase() override;
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif /* DECT2020_TEST_CASE_H */ 