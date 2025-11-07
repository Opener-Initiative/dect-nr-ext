#include "dect2020ext-random-access-resource-ie-serialization-test-case.h"
#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIESerializationTest : public TestSuite
{
    public:
        Dect2020ExtRandomAccessResourceIESerializationTest();
        virtual ~Dect2020ExtRandomAccessResourceIESerializationTest();
};

Dect2020ExtRandomAccessResourceIESerializationTest::Dect2020ExtRandomAccessResourceIESerializationTest() 
    : TestSuite("dect2020ext-random-access-resource-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRandomAccessResourceIESerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRandomAccessResourceIESerializationTest::~Dect2020ExtRandomAccessResourceIESerializationTest()
{
}

static Dect2020ExtRandomAccessResourceIESerializationTest dect2020ExtRandomAccessResourceIESerializationTest;

} // namespace ns3

