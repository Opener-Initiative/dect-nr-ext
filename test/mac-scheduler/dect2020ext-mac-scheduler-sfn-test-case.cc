#include "dect2020ext-mac-scheduler-sfn-test-case.h"

#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-config-values.h"
#include "ns3/dect2020ext-config.h"

#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtMacSchedulerSfnTestCase::Dect2020ExtMacSchedulerSfnTestCase()
    : TestCase("Dect2020ExtMacSchedulerSfnTestCase")
{
}

Dect2020ExtMacSchedulerSfnTestCase::~Dect2020ExtMacSchedulerSfnTestCase()
{
}

void
Dect2020ExtMacSchedulerSfnTestCase::DoSetup()
{
}

void
Dect2020ExtMacSchedulerSfnTestCase::DoRun()
{
    Dect2020ExtConfig config;

    Dect2020ExtMacScheduler scheduler;
    scheduler.Dummy(true);
    scheduler.SetBandNumber(BAND_NUMBER);
    scheduler.SetSubcarrierScalingFactor(SUBCARRIER_SCALING_FACTOR);
    scheduler.SetFourierTransformScalingFactor(FOURIER_TRANSFORM_SCALING_FACTOR);
    scheduler.SetChannelManager(config.m_channelManager);
    scheduler.Initialize();
    scheduler.StartMacScheduler();

    // After 10 milliseconds (1 frame) check if SFN = 1
    Simulator::Schedule(MilliSeconds(10), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetSfn(), (uint32_t)1, "SFN should be 1 after 1 frame (10ms)");
    });

    Simulator::Stop(Seconds(1));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3
