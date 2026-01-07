#include "dect2020ext-mac-multiplexing-header-type-a-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtMacMultiplexingHeaderTypeATest : public TestSuite
{
    public:
        Dect2020ExtMacMultiplexingHeaderTypeATest();
        virtual ~Dect2020ExtMacMultiplexingHeaderTypeATest();
};

Dect2020ExtMacMultiplexingHeaderTypeATest::Dect2020ExtMacMultiplexingHeaderTypeATest()
    : TestSuite("dect2020ext-mac-multiplexing-header-type-a-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtMacMultiplexingHeaderTypeASerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtMacMultiplexingHeaderTypeATest::~Dect2020ExtMacMultiplexingHeaderTypeATest()
{
}

static Dect2020ExtMacMultiplexingHeaderTypeATest dect2020ExtMacMultiplexingHeaderTypeATest;

} // namespace ns3

