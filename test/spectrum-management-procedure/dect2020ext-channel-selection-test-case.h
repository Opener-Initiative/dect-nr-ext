#ifndef DECT2020_EXT_CHANNEL_SELECTION_TEST_CASE_H
#define DECT2020_EXT_CHANNEL_SELECTION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtChannelSelectionTestCase : public TestCase
{
  public:
    Dect2020ExtChannelSelectionTestCase();
    virtual ~Dect2020ExtChannelSelectionTestCase();
    
    void DoSetup() override;
    void DoRun() override;
};

} // namespace ns3

#endif // DECT2020_EXT_CHANNEL_SELECTION_TEST_CASE_H