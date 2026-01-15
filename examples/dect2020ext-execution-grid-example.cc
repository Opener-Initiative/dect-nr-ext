#include "ns3/dect2020ext-execution-grid.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExecutionGridExample");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtExecutionGrid", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
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

    // MAC
    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();
    mac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);

    Dect2020ExtExecutionGridOperation operation;
    operation.m_operationType = ns3::Dect2020ExtExecutionGridOperation::TEST;
    operation.m_channelNumber = 1657;
    operation.m_startSubslot = 10;
    operation.m_endSubslot = 10;

    // Using lambda function
    operation.m_operation = [mac](){
        mac->TestFunction();
    };

    // executionGrid->ScheduleOperation(operation);

    Simulator::Stop(MilliSeconds(20));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}