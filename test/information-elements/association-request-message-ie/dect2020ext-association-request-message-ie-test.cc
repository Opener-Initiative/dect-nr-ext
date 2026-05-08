#include "dect2020ext-association-request-message-ie-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtAssociationRequestMessageIeSerializationTest : public TestSuite
{
    public:
        Dect2020ExtAssociationRequestMessageIeSerializationTest();
        virtual ~Dect2020ExtAssociationRequestMessageIeSerializationTest();
};

Dect2020ExtAssociationRequestMessageIeSerializationTest::Dect2020ExtAssociationRequestMessageIeSerializationTest()
    : TestSuite("dect2020ext-association-request-message-ie-serialization-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtAssociationRequestMessageIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtAssociationRequestMessageIeSerializationTest::~Dect2020ExtAssociationRequestMessageIeSerializationTest()
{
}

static Dect2020ExtAssociationRequestMessageIeSerializationTest dect2020ExtAssociationRequestMessageIeSerializationTest;

} // namespace ns3
