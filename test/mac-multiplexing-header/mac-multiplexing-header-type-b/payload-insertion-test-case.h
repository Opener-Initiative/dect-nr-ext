#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_PAYLOAD_INSERTION_TEST_CASE_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_PAYLOAD_INSERTION_TEST_CASE_H

#include "ns3/test.h"
#include "ns3/dect2020-mac-multiplexing-header-type-b.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase : public TestCase
{
public:
  Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase();
  virtual ~Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase();

  virtual void DoSetup();
  virtual void DoRun();
};

} // namespace ns3

#endif /* DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_PAYLOAD_INSERTION_TEST_CASE_H */