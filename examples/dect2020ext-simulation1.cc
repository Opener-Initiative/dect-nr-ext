#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"

#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSimulation1");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtSimulation1", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtChannelManager", LOG_LEVEL_INFO);

    Dect2020ExtChannelManager channelManager;
    Ptr<SingleModelSpectrumChannel> spectrumChannel = CreateObject<SingleModelSpectrumChannel>();
    Ptr<ConstantSpeedPropagationDelayModel> propagationDelay = CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LogDistancePropagationLossModel> propagationLoss = CreateObject<LogDistancePropagationLossModel>();

    spectrumChannel->SetPropagationDelayModel(propagationDelay);
    spectrumChannel->AddPropagationLossModel(propagationLoss);

    channelManager.SetSpectrumChannel(spectrumChannel);
    channelManager.InitializeChannels(1, 1);

    channelManager.MeasureRssi();
    Simulator::Schedule(MicroSeconds(416), &Dect2020ExtChannelManager::DisplayRssiResults);

    Simulator::Stop(Seconds(1.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}