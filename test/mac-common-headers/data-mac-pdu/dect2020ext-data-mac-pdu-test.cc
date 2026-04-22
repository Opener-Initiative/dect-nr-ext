#include "ns3/test.h"
#include "dect2020ext-data-mac-pdu-serialization-test-case.h"

namespace ns3
{

class Dect2020ExtDataMacPduTest : public TestSuite
{
    public:
        Dect2020ExtDataMacPduTest();
        virtual ~Dect2020ExtDataMacPduTest();
};

Dect2020ExtDataMacPduTest::Dect2020ExtDataMacPduTest()
    : TestSuite("dect2020ext-data-mac-pdu-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtDataMacPduSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtDataMacPduTest::~Dect2020ExtDataMacPduTest()
{
}

static Dect2020ExtDataMacPduTest dect2020ExtDataMacPduTest;

} // namespace ns3