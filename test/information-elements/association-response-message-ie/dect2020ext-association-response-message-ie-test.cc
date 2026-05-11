#include "dect2020ext-association-response-message-ie-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtAssociationResponseMessageIeTest : public TestSuite
{
    public:
        Dect2020ExtAssociationResponseMessageIeTest();
        virtual ~Dect2020ExtAssociationResponseMessageIeTest();
};

Dect2020ExtAssociationResponseMessageIeTest::Dect2020ExtAssociationResponseMessageIeTest()
    : TestSuite("dect2020ext-association-response-message-ie-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtAssociationResponseMessageIeSerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtAssociationResponseMessageIeTest::~Dect2020ExtAssociationResponseMessageIeTest()
{
}

static Dect2020ExtAssociationResponseMessageIeTest dect2020ExtAssociationResponseMessageIeTest;

} // namespace ns3