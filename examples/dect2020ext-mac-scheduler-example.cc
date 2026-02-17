#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacSchedulerExample");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtMacScheduler", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtMacSchedulerExample", LOG_LEVEL_INFO);

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->Configure();

    // Mac Scheduler
    Ptr<Dect2020ExtMacScheduler> macScheduler = CreateObject<Dect2020ExtMacScheduler>();
    macScheduler->Initialize();
    macScheduler->StartMacScheduler();

    Simulator::Schedule(MilliSeconds(1), &Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs, macScheduler, 4);

    Simulator::Stop(MilliSeconds(20));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
