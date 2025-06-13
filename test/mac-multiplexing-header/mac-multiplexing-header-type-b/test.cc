#include "ns3/test.h"

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
    // Add test cases here
}

Dect2020MacMultiplexingHeaderTypeBTest::~Dect2020MacMultiplexingHeaderTypeBTest()
{
}

static Dect2020MacMultiplexingHeaderTypeBTest dect2020MacMultiplexingHeaderTypeBTest;

} // namespace ns3 