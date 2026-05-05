#include "dect2020ext-phy-control-field-type-1-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtPhyControlFieldType1Test : public TestSuite
{
    public:
        Dect2020ExtPhyControlFieldType1Test();
        virtual ~Dect2020ExtPhyControlFieldType1Test();
};

Dect2020ExtPhyControlFieldType1Test::Dect2020ExtPhyControlFieldType1Test() 
    : TestSuite("dect2020ext-phy-control-field-type-1-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtPhyControlFieldType1SerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtPhyControlFieldType1Test::~Dect2020ExtPhyControlFieldType1Test()
{
}

static Dect2020ExtPhyControlFieldType1Test dect2020ExtPhyControlFieldType1Test;

} // namespace ns3 