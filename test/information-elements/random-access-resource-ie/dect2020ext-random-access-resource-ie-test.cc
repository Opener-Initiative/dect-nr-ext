#include "dect2020ext-random-access-resource-ie-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIeTest : public TestSuite
{
    public:
        Dect2020ExtRandomAccessResourceIeTest();
        virtual ~Dect2020ExtRandomAccessResourceIeTest();
};

Dect2020ExtRandomAccessResourceIeTest::Dect2020ExtRandomAccessResourceIeTest() 
    : TestSuite("dect2020ext-random-access-resource-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRandomAccessResourceIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRandomAccessResourceIeTest::~Dect2020ExtRandomAccessResourceIeTest()
{
}

static Dect2020ExtRandomAccessResourceIeTest dect2020ExtRandomAccessResourceIeTest;

} // namespace ns3

