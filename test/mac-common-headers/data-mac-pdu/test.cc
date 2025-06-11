#include "ns3/test.h"
#include "serialization-test-case.h"

namespace ns3
{

class Dect2020DataMacPduTest : public TestSuite
{
    public:
        Dect2020DataMacPduTest();
        virtual ~Dect2020DataMacPduTest();
};

Dect2020DataMacPduTest::Dect2020DataMacPduTest()
    : TestSuite("dect2020-data-mac-pdu-test", Type::UNIT)
{
    AddTestCase(new Dect2020DataMacPduSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020DataMacPduTest::~Dect2020DataMacPduTest()
{
}

static Dect2020DataMacPduTest dect2020DataMacPduTest;

} // namespace ns3