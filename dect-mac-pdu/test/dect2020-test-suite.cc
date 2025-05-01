#include "ns3/test.h"
#include "dect2020-test-case.h"

namespace ns3 {

class Dect2020TestSuite : public TestSuite
{
    public:
        Dect2020TestSuite();
        ~Dect2020TestSuite() override;
};

Dect2020TestSuite::Dect2020TestSuite() : TestSuite("dect2020-test-suite", Type::UNIT)
{
    AddTestCase(new Dect2020TestCase(), TestCase::Duration::QUICK);
}

Dect2020TestSuite::~Dect2020TestSuite()
{
}

static Dect2020TestSuite dect2020TestSuite;

} // namespace ns3