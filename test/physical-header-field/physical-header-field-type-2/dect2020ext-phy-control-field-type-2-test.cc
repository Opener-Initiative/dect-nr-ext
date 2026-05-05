#include "dect2020ext-phy-control-field-type-2-serialization-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtPhyControlFieldType2Test : public TestSuite
{
    public:
        Dect2020ExtPhyControlFieldType2Test();
        virtual ~Dect2020ExtPhyControlFieldType2Test();
};

Dect2020ExtPhyControlFieldType2Test::Dect2020ExtPhyControlFieldType2Test()
    : TestSuite("dect2020ext-phy-control-field-type-2-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtPhyControlFieldType2SerializationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtPhyControlFieldType2Test::~Dect2020ExtPhyControlFieldType2Test()
{
}

static Dect2020ExtPhyControlFieldType2Test dect2020ExtPhyControlFieldType2Test;

} // namespace ns3