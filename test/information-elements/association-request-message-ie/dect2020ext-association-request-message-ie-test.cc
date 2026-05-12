#include "dect2020ext-association-request-message-ie-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtAssociationRequestMessageIeTest : public TestSuite
{
    public:
        Dect2020ExtAssociationRequestMessageIeTest();
        virtual ~Dect2020ExtAssociationRequestMessageIeTest();
};

Dect2020ExtAssociationRequestMessageIeTest::Dect2020ExtAssociationRequestMessageIeTest()
    : TestSuite("dect2020ext-association-request-message-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtAssociationRequestMessageIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtAssociationRequestMessageIeTest::~Dect2020ExtAssociationRequestMessageIeTest()
{
}

static Dect2020ExtAssociationRequestMessageIeTest dect2020ExtAssociationRequestMessageIeTest;

} // namespace ns3
