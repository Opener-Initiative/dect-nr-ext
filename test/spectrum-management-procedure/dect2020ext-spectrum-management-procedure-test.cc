#include "dect2020ext-scan-measurement-duration-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtSpectrumManagementProcedureTest : public TestSuite
{
    public:
        Dect2020ExtSpectrumManagementProcedureTest();
        virtual ~Dect2020ExtSpectrumManagementProcedureTest();
};

Dect2020ExtSpectrumManagementProcedureTest::Dect2020ExtSpectrumManagementProcedureTest()
    : TestSuite("dect2020ext-spectrum-management-procedure-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtScanMeasurementDurationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtSpectrumManagementProcedureTest::~Dect2020ExtSpectrumManagementProcedureTest()
{
}

static Dect2020ExtSpectrumManagementProcedureTest dect2020ExtSpectrumManagementProcedureTest;

} // namespace ns3