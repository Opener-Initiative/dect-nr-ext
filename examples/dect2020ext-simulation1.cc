#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/spectrum-value.h"
#include "ns3/double.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSimulation1");

void
CreateRSSIMatrix(void)
{
    auto channels = Dect2020ExtChannelManager::GetValidChannels(1);
    std::cout << "\n========== RSSI Matrix ==========" << std::endl;
    
    for (auto channel : channels)
    {
        std::cout << "Channel " << channel->m_channelId << " (Freq: " << channel->m_centerFrequency << " Hz):" << std::endl;
        
        for (const auto& slot : channel->m_slots)  // Use const reference for reading
        {
            std::cout << "  Slot " << slot.slotId << ":" << std::endl;
            for (const auto& subslot : slot.subslots)  // Use const reference for reading
            {
                std::cout << "    Subslot " << subslot.subslotId << ": RSSI = " << subslot.rssi << " dBm" << " Status = " << subslot.status << std::endl;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "=================================" << std::endl;
}

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtSimulation1", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtMac", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtChannelManager", LOG_LEVEL_INFO);

    // Channel
    Ptr<SingleModelSpectrumChannel> spectrumChannel = CreateObject<SingleModelSpectrumChannel>();
    Ptr<ConstantSpeedPropagationDelayModel> propagationDelay = CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LogDistancePropagationLossModel> propagationLoss = CreateObject<LogDistancePropagationLossModel>();
    spectrumChannel->SetPropagationDelayModel(propagationDelay);
    spectrumChannel->AddPropagationLossModel(propagationLoss);

    // Channel Manager
    Dect2020ExtChannelManager channelManager;
    channelManager.SetSpectrumChannel(spectrumChannel);
    
    // Initialize channels for band 1 with subcarrier scaling factor 1
    channelManager.InitializeChannels(1, 1);

    // Spectrum Model
    Ptr<SpectrumModel> spectrumModel = channelManager.GetSpectrumModel(1);

    // MAC
    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();

    mac->ScheduleBackgroundScan(Seconds(0));

    Simulator::Schedule(Seconds(9), &CreateRSSIMatrix);

    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}