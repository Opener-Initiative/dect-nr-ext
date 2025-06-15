#include "ns3/test.h"
#include "payload-insertion-test-case.h"
#include "serialization-test-case.h"

using namespace ns3;

class Dect2020MacMultiplexingHeaderTypeCTestSuite : public TestSuite
{
public:
  Dect2020MacMultiplexingHeaderTypeCTestSuite()
      : TestSuite("dect2020-mac-multiplexing-header-type-c-test", UNIT)
  {
    AddTestCase(new Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase(), TestCase::QUICK);
    AddTestCase(new Dect2020MacMultiplexingHeaderTypeCSerializationTestCase(), TestCase::QUICK);
  }
};

static Dect2020MacMultiplexingHeaderTypeCTestSuite dect2020MacMultiplexingHeaderTypeCTestSuite;
