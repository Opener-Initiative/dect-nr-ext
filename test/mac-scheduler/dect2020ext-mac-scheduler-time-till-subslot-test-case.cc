#include "dect2020ext-mac-scheduler-time-till-subslot-test-case.h"

#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-config-values.h"
#include "ns3/dect2020ext-config.h"

#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtMacSchedulerTimeTillSubslotTestCase::Dect2020ExtMacSchedulerTimeTillSubslotTestCase()
    : TestCase("Dect2020ExtMacSchedulerTimeTillSubslotTestCase")
{
}

Dect2020ExtMacSchedulerTimeTillSubslotTestCase::~Dect2020ExtMacSchedulerTimeTillSubslotTestCase()
{
}

void
Dect2020ExtMacSchedulerTimeTillSubslotTestCase::DoSetup()
{
}

void
Dect2020ExtMacSchedulerTimeTillSubslotTestCase::DoRun()
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

    double subslotDurationNs = SLOT_DURATION_NS / scheduler.GetChannelManager()->GetSubslotsPerSlot();

    // At frame start (t=0), requesting subslot 0 → 0 ns remaining
    Simulator::Schedule(NanoSeconds(0), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(0), 0.0, "Time till subslot 0 at frame start should be 0");
    });

    // At frame start (t=0), requesting subslot 2 → 2 * subslotDuration remaining
    Simulator::Schedule(NanoSeconds(0), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(2), subslotDurationNs * 2, "Incorrect time till subslot 2 at frame start");
    });

    // At 100 ns into subslot 0, requesting subslot 1 → subslotDuration - 100 remaining
    Simulator::Schedule(NanoSeconds(100), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(1), subslotDurationNs - 100.0, "Incorrect time till subslot 1 at 100 ns into frame");
    });

    // At exact start of subslot 1, requesting subslot 1 → 0 ns remaining
    Simulator::Schedule(NanoSeconds(subslotDurationNs), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(scheduler.GetCurrentSubslotNumber()), 0.0, "Time till current subslot at its exact start should be 0");
    });

    // At exact start of subslot 1, requesting subslot 3 → 2 * subslotDuration remaining
    Simulator::Schedule(NanoSeconds(subslotDurationNs), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(scheduler.GetCurrentSubslotNumber() + 2), subslotDurationNs * 2, "Incorrect time till subslot 3 from exact start of subslot 1");
    });

    // At 500 ns into subslot 1, requesting subslot 3 → 2 * subslotDuration - 500 remaining
    Simulator::Schedule(NanoSeconds(subslotDurationNs + 500), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(scheduler.GetCurrentSubslotNumber() + 2), subslotDurationNs * 2 - 500.0, "Incorrect time till subslot 3 at 500 ns into subslot 1");
    });

    // ---- Subslot 20 ----

    // At frame start (t=0), requesting subslot 20
    Simulator::Schedule(NanoSeconds(0), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(20), subslotDurationNs * 20, "Incorrect time till subslot 20 at frame start");
    });

    // At 329 ns into subslot 1 (t = subslotDuration + 329), requesting subslot 20
    Simulator::Schedule(NanoSeconds(subslotDurationNs + 329), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(20), subslotDurationNs * 19 - 329.0, "Incorrect time till subslot 20 at 329 ns into subslot 1");
    });

    // At exact start of subslot 20, requesting subslot 20 → 0 ns remaining
    Simulator::Schedule(NanoSeconds(subslotDurationNs * 20), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(scheduler.GetCurrentSubslotNumber()), 0.0, "Time till current subslot at its exact start should be 0 (subslot 20)");
    });

    // ---- Subslot 35 ----

    // At frame start (t=0), requesting subslot 35
    Simulator::Schedule(NanoSeconds(0), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(35), subslotDurationNs * 35, "Incorrect time till subslot 35 at frame start");
    });

    // At 137 ns into subslot 10 (t = 10 * subslotDuration + 137), requesting subslot 35
    Simulator::Schedule(NanoSeconds(subslotDurationNs * 10 + 137), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(35), subslotDurationNs * 25 - 137.0, "Incorrect time till subslot 35 at 137 ns into subslot 10");
    });

    // At 329 ns into subslot 20 (t = 20 * subslotDuration + 329), requesting subslot 35
    Simulator::Schedule(NanoSeconds(subslotDurationNs * 20 + 329), [&scheduler, subslotDurationNs, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(35), subslotDurationNs * 15 - 329.0, "Incorrect time till subslot 35 at 329 ns into subslot 20");
    });

    // At exact start of subslot 35, requesting subslot 35 → 0 ns remaining
    Simulator::Schedule(NanoSeconds(subslotDurationNs * 35), [&scheduler, this]() {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetTimeTillSubslotStartNs(scheduler.GetCurrentSubslotNumber()), 0.0, "Time till current subslot at its exact start should be 0 (subslot 35)");
    });

    Simulator::Stop(MilliSeconds(10));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3