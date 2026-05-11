#include "dect2020ext-random-access-resource-ie-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIeSerializationTest : public TestSuite
{
    public:
        Dect2020ExtRandomAccessResourceIeSerializationTest();
        virtual ~Dect2020ExtRandomAccessResourceIeSerializationTest();
};

Dect2020ExtRandomAccessResourceIeSerializationTest::Dect2020ExtRandomAccessResourceIeSerializationTest() 
    : TestSuite("dect2020ext-random-access-resource-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRandomAccessResourceIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRandomAccessResourceIeSerializationTest::~Dect2020ExtRandomAccessResourceIeSerializationTest()
{
}

static Dect2020ExtRandomAccessResourceIeSerializationTest dect2020ExtRandomAccessResourceIeSerializationTest;

} // namespace ns3

