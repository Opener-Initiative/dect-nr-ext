#include "dect2020ext-mac-scheduler-test-case.h"

#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-config-values.h"
#include "ns3/dect2020ext-config.h"

namespace ns3
{

Dect2020ExtMacSchedulerTestCase::Dect2020ExtMacSchedulerTestCase()
    : TestCase("Dect2020ExtMacSchedulerTestCase")
{
}

Dect2020ExtMacSchedulerTestCase::~Dect2020ExtMacSchedulerTestCase()
{
}

void
Dect2020ExtMacSchedulerTestCase::DoSetup()
{
}

void
Dect2020ExtMacSchedulerTestCase::DoRun()
{
    Dect2020ExtConfig config;
    Ptr<Dect2020ExtChannelManager> channelManager = config.m_channelManager;
    Dect2020ExtMacScheduler scheduler;
    scheduler.SetBandNumber(1);
    scheduler.SetSubcarrierScalingFactor(1);
    scheduler.SetFourierTransformScalingFactor(1);
    scheduler.SetChannelManager(channelManager);
    scheduler.Initialize();
    scheduler.StartMacScheduler();

    auto grid = scheduler.GetMacScheduler();
    NS_TEST_ASSERT_MSG_EQ(grid.empty(), false, "Error: the MAC Scheduler should not be empty after initialization");
    NS_TEST_ASSERT_MSG_EQ(grid[1657].size(), SLOTS_PER_FRAME, "Error: MAC Scheduler channel should have all slots");
    NS_TEST_ASSERT_MSG_EQ(grid[1657][0].subslots.size(), 2, "Error: each slot should have 2 subslots");

    for (int i = 0; i < 24; i++)
    {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetSlotStartTimeOffsetNs(i), SLOT_DURATION_NS * i, "Error with slot start time");
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetSlotEndTimeOffsetNs(i), SLOT_DURATION_NS * (i + 1), "Error with slots end time");
    }

    for (int i = 0; i < 48; i++)
    {
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetSubslotStartTimeOffsetNs(i), (SLOT_DURATION_NS / 2) * i, "Error with subslot start time");
        NS_TEST_ASSERT_MSG_EQ(scheduler.GetSubslotEndTimeOffsetNs(i), (SLOT_DURATION_NS / 2) * (i + 1), "Error with subslot start time");
    }
}

} // namespace ns3
