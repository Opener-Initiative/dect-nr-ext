#include "ns3/dect2020ext-execution-grid.h"
#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExecutionGridExample");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtExecutionGrid", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtExecutionGridExample", LOG_LEVEL_INFO);

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->Configure();

    // Execution Grid
    Ptr<Dect2020ExtExecutionGrid> executionGrid = CreateObject<Dect2020ExtExecutionGrid>();
    executionGrid->Initialize();
    executionGrid->StartExecutionGrid();
    double s = executionGrid->GetSubslotStartTimeOffsetNs(1);
    double e = executionGrid->GetSubslotEndTimeOffsetNs(1);

    NS_LOG_INFO("Subslot start time: " << s << " ns, end time: " << e << " ns");

    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}