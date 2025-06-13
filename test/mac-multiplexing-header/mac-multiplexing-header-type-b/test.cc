#include "ns3/test.h"
#include "serialization-test-case.h"
#include "payload-insertion-test-case.h"

namespace ns3 {

class Dect2020MacMultiplexingHeaderTypeBTest : public TestSuite
{
    public:
        Dect2020MacMultiplexingHeaderTypeBTest();
        virtual ~Dect2020MacMultiplexingHeaderTypeBTest();
};

Dect2020MacMultiplexingHeaderTypeBTest::Dect2020MacMultiplexingHeaderTypeBTest() 
    : TestSuite("dect2020-mac-multiplexing-header-type-b-test", Type::UNIT)
{
    AddTestCase(new Dect2020MacMultiplexingHeaderTypeBPayloadInsertionTestCase(), Duration::QUICK);
    AddTestCase(new Dect2020MacMultiplexingHeaderTypeBSerializationTestCase(), Duration::QUICK);
}

Dect2020MacMultiplexingHeaderTypeBTest::~Dect2020MacMultiplexingHeaderTypeBTest()
{
}

static Dect2020MacMultiplexingHeaderTypeBTest dect2020MacMultiplexingHeaderTypeBTest;

} // namespace ns3 