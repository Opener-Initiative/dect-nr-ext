#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_SERIALIZATION_TEST_CASE_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_SERIALIZATION_TEST_CASE_H

#include "ns3/test.h"
#include "ns3/dect2020-mac-multiplexing-header-type-b.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeBSerializationTestCase : public TestCase
{
public:
  Dect2020MacMultiplexingHeaderTypeBSerializationTestCase();
  virtual ~Dect2020MacMultiplexingHeaderTypeBSerializationTestCase();

  virtual void DoSetup(void);
  virtual void DoRun(void);
};

} // namespace ns3

#endif /* DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_SERIALIZATION_TEST_CASE_H */