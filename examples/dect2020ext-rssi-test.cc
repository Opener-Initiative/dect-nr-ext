#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/spectrum-value.h"
#include "ns3/double.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtRssiTest");

void
OnRssiMeasurement(uint16_t channelId, double rssiDbm)
{
    NS_LOG_FUNCTION(channelId << rssiDbm);
    std::cout << "RSSI Callback Fired! Channel " << channelId << ": " << rssiDbm << " dBm" << std::endl;
}

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtRssiTest", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020ExtRssiTest", LOG_PREFIX_TIME);
    LogComponentEnable("Dect2020ExtRssiTest", LOG_PREFIX_FUNC);
    
    // LogComponentEnable("Dect2020ExtChannelManager", LOG_LEVEL_ALL);
    // LogComponentEnable("Dect2020ExtChannelManager", LOG_PREFIX_TIME);
    // LogComponentEnable("Dect2020ExtChannelManager", LOG_PREFIX_FUNC);
    
    // Also enable SpectrumAnalyzer logging to see if measurements happen
    // LogComponentEnable("SpectrumAnalyzer", LOG_LEVEL_ALL);
    // LogComponentEnable("SpectrumAnalyzer", LOG_PREFIX_TIME);
    // LogComponentEnable("SpectrumAnalyzer", LOG_PREFIX_FUNC);

    LogComponentEnable("Dect2020ExtMac", LOG_LEVEL_ALL);
    LogComponentEnable("Dect2020ExtMac", LOG_PREFIX_TIME);
    LogComponentEnable("Dect2020ExtMac", LOG_PREFIX_FUNC);

    // Channel
    Ptr<SingleModelSpectrumChannel> spectrumChannel = CreateObject<SingleModelSpectrumChannel>();
    Ptr<ConstantSpeedPropagationDelayModel> propagationDelay = CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LogDistancePropagationLossModel> propagationLoss = CreateObject<LogDistancePropagationLossModel>();
    spectrumChannel->SetPropagationDelayModel(propagationDelay);
    spectrumChannel->AddPropagationLossModel(propagationLoss);

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetSpectrumChannel(spectrumChannel);
    
    // Initialize channels for band 1 with subcarrier scaling factor 1
    channelManager->InitializeChannels(1, 1);

    // MAC
    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();
    mac->SetChannelManager(channelManager);

    mac->ScheduleBackgroundScan(MilliSeconds(0));

    Simulator::Stop(MilliSeconds(211));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}