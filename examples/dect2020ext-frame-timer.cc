#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtFrameTimerExample");

// Store the exact callback instance so we can disconnect it later
static Callback<void, uint32_t, uint32_t> g_sfnCb;

static void
DisconnectSfn(Ptr<Dect2020ExtPhy> phy)
{
    phy->TraceDisconnectWithoutContext("SFN", g_sfnCb);
}

void
SFNChanged(Ptr<Dect2020ExtPhy> phy, uint32_t oldValue, uint32_t newValue)
{
    NS_LOG_INFO("SFN changed from " << oldValue << " to " << newValue);
    // Example of one-shot behavior: disconnect after first notification
    // Schedule disconnection to avoid modifying trace list during iteration
    Simulator::ScheduleNow(&DisconnectSfn, phy);
}

int
main(int argc, char* argv[])
{
    LogComponentEnable("Dect2020ExtFrameTimerExample", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020ExtPhy", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020ExtPhy", LOG_PREFIX_TIME);

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->InitializeChannels(1, 1);

    Ptr<Dect2020ExtPhy> phy = CreateObject<Dect2020ExtPhy>();
    phy->StartFrameTimer();

    // Connect to the SFN traced value via the object's trace source API
    g_sfnCb = MakeBoundCallback(&SFNChanged, phy);
    phy->TraceConnectWithoutContext("SFN", g_sfnCb);

    Simulator::Schedule(MilliSeconds(9), &Dect2020ExtPhy::GetCurrentSubslot, phy);

    Simulator::Stop(Seconds(0.03));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}