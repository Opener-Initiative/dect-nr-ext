#ifndef DECT2020_EXT_RANDOM_ACCESS_TEST_CASE2_H
#define DECT2020_EXT_RANDOM_ACCESS_TEST_CASE2_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessTestCase2 : public TestCase
{
  public:
    Dect2020ExtRandomAccessTestCase2();
    ~Dect2020ExtRandomAccessTestCase2() override;

    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif // DECT2020_EXT_RANDOM_ACCESS_TEST_CASE2_H