#include "dect2020ext-mac-scheduler-time-till-next-frame-test-case.h"

#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-config-values.h"
#include "ns3/dect2020ext-config.h"

#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtMacSchedulerTimeTillNextFrameTestCase::Dect2020ExtMacSchedulerTimeTillNextFrameTestCase()
    : TestCase("Dect2020ExtMacSchedulerTimeTillNextFrameTestCase")
{
}

Dect2020ExtMacSchedulerTimeTillNextFrameTestCase::~Dect2020ExtMacSchedulerTimeTillNextFrameTestCase()
{
}

void
Dect2020ExtMacSchedulerTimeTillNextFrameTestCase::DoSetup()
{
}

void
Dect2020ExtMacSchedulerTimeTillNextFrameTestCase::DoRun()
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

    // GetTimeTillNextFrame - Frame 0

    // At frame start (t=0), 10 ms = 1 frame remaining
    Simulator::Schedule(NanoSeconds(0), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS, "Time till next frame should be 10ms");
    });

    // 1234 microseconds into frame 0, 1 frame - 1234 microseconds remaining
    Simulator::Schedule(MicroSeconds(1234), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS - (1234 * 1000), "Incorrect time till next frame");
    });

    // 6789 microseconds into frame 0, 1 frame - 6789 microseconds remaining
    Simulator::Schedule(MicroSeconds(6789), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS - (6789 * 1000), "Incorrect time till next frame");
    });

    // GetTimeTillNextFrame - Frame 3 (starts at t=30ms)
    // Note: exact frame boundaries cannot be tested in frames 1+ — StartMacScheduler is
    // scheduled during simulation and gets a higher event ID than lambdas queued before
    // Simulator::Run(), so the lambda fires before m_currentFrameStartTime is updated.

    // 100 ns into frame 3 (t=30ms + 100ns), ~1 frame remaining
    Simulator::Schedule(NanoSeconds(30000000 + 100), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS - 100, "Incorrect time till next frame at 100 ns into frame 3");
    });

    // 1234 microseconds into frame 2, 1 frame - 1234 microseconds remaining
    Simulator::Schedule(MicroSeconds(31234), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS - (1234 * 1000), "Incorrect time till next frame");
    });

    // 6789 microseconds into frame 2, 1 frame - 6789 microseconds remaining
    Simulator::Schedule(MicroSeconds(36789), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillNextFrameNs(), FRAME_DURATION_NS - (6789 * 1000), "Incorrect time till next frame");
    });

    // Run for 5 frames
    Simulator::Stop(MilliSeconds(50));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3