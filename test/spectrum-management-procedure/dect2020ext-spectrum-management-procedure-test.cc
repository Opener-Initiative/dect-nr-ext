#include "dect2020ext-scan-measurement-duration-test-case.h"
#include "dect2020ext-rssi-measurement-test-case.h"
#include "dect2020ext-network-beacon-period-test-case.h"

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtScanMeasurementDurationTest : public TestSuite
{
    public:
        Dect2020ExtScanMeasurementDurationTest();
        virtual ~Dect2020ExtScanMeasurementDurationTest();
};

Dect2020ExtScanMeasurementDurationTest::Dect2020ExtScanMeasurementDurationTest()
    : TestSuite("dect2020ext-scan-measurement-duration-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtScanMeasurementDurationTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtScanMeasurementDurationTest::~Dect2020ExtScanMeasurementDurationTest()
{
}

static Dect2020ExtScanMeasurementDurationTest dect2020ExtScanMeasurementDurationTest;

class Dect2020ExtRssiMeasurementTest : public TestSuite
{
    public:
        Dect2020ExtRssiMeasurementTest();
        virtual ~Dect2020ExtRssiMeasurementTest();
};

Dect2020ExtRssiMeasurementTest::Dect2020ExtRssiMeasurementTest()
    : TestSuite("dect2020ext-rssi-measurement-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtRssiMeasurementTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtRssiMeasurementTest::~Dect2020ExtRssiMeasurementTest()
{
}

static Dect2020ExtRssiMeasurementTest dect2020ExtRssiMeasurementTest;

class Dect2020ExtNetworkBeaconPeriodTest : public TestSuite
{
    public:
        Dect2020ExtNetworkBeaconPeriodTest();
        virtual ~Dect2020ExtNetworkBeaconPeriodTest();
};

Dect2020ExtNetworkBeaconPeriodTest::Dect2020ExtNetworkBeaconPeriodTest()
    : TestSuite("dect2020ext-network-beacon-period-test", Type::UNIT)
{
    AddTestCase(new Dect2020ExtNetworkBeaconPeriodTestCase(), TestCase::Duration::QUICK);
}

Dect2020ExtNetworkBeaconPeriodTest::~Dect2020ExtNetworkBeaconPeriodTest()
{
}

static Dect2020ExtNetworkBeaconPeriodTest dect2020ExtNetworkBeaconPeriodTest;

// class Dect2020ExtSpectrumManagementProcedureTest : public TestSuite
// {
//     public:
//         Dect2020ExtSpectrumManagementProcedureTest();
//         virtual ~Dect2020ExtSpectrumManagementProcedureTest();
// };

// Dect2020ExtSpectrumManagementProcedureTest::Dect2020ExtSpectrumManagementProcedureTest()
//     : TestSuite("dect2020ext-spectrum-management-procedure-test", Type::UNIT)
// {
//     AddTestCase(new Dect2020ExtScanMeasurementDurationTestCase(), TestCase::Duration::QUICK);
//     AddTestCase(new Dect2020ExtRssiMeasurementTestCase(), TestCase::Duration::QUICK);
// }

// Dect2020ExtSpectrumManagementProcedureTest::~Dect2020ExtSpectrumManagementProcedureTest()
// {
// }

// static Dect2020ExtSpectrumManagementProcedureTest dect2020ExtSpectrumManagementProcedureTest;

} // namespace ns3