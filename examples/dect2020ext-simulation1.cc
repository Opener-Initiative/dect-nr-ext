#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/dect2020-channel-manager.h"
#include "ns3/dect2020-mac-header-type.h"
#include "ns3/dect2020-mac-information-elements.h"
#include "ns3/dect2020-mac-messages.h"
#include "ns3/dect2020-mac.h"
#include "ns3/dect2020-net-device.h"
#include "ns3/dect2020-phy.h"
#include "ns3/dect2020-physical-header-field.h"
#include "ns3/dect2020-spectrum-signal-parameters.h"
#include "ns3/internet-module.h"
#include "ns3/isotropic-antenna-model.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/spectrum-analyzer-helper.h"
#include "ns3/spectrum-analyzer.h"
#include "ns3/spectrum-module.h"

#include <fstream>
#include <string>
#include <cmath>

using namespace ns3;
using TermPointType = Dect2020NetDevice::TerminationPointType;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSimulation1");

/**
 * \brief Calculate path loss using log-distance model
 * \param distance Distance between nodes in meters
 * \param frequency Operating frequency in Hz
 * \return Path loss in dB
 */
double CalculatePathLoss(double distance, double frequency)
{
    // Free space path loss model: FSPL(d) = 20*log10(d) + 20*log10(f) + 20*log10(4π/c)
    // Where d is distance in meters, f is frequency in Hz, c is speed of light
    
    if (distance <= 1.0)
    {
        distance = 1.0; // Minimum distance to avoid log(0)
    }
    
    // Free space path loss in dB
    double pathLoss = 20 * log10(distance) + 20 * log10(frequency) + 20 * log10(4 * M_PI / 3e8);
    
    return pathLoss;
}

/**
 * \brief Function to monitor RSSI for all channels in a band
 * \param channelManager Reference to the channel manager
 * \param bandNumber The band number to monitor
 * \param devices Container of devices to get realistic RSSI measurements from
 */
void MonitorRssi(Dect2020ChannelManager& channelManager, uint8_t bandNumber, NetDeviceContainer devices)
{
    // Get band parameters to determine channel range
    BandParameters bandParams = Dect2020ChannelManager::GetBandParameters(bandNumber);
    
    // Monitor RSSI for each channel in the band
    for (uint16_t channelId = bandParams.nStart; channelId <= bandParams.nEnd; ++channelId)
    {
        double rssiDbm = Dect2020ChannelManager::GetRssiDbm(channelId);
        NS_LOG_INFO("Channel " << channelId << " Channel Occupancy: " << rssiDbm << " dBm");
    }
    
    // Additionally, get realistic RSSI measurements by calculating path loss between nodes
    if (devices.GetN() >= 2)
    {
        Ptr<Node> node1 = devices.Get(0)->GetNode();
        Ptr<Node> node2 = devices.Get(1)->GetNode();
        
        Ptr<MobilityModel> mob1 = node1->GetObject<MobilityModel>();
        Ptr<MobilityModel> mob2 = node2->GetObject<MobilityModel>();
        
        if (mob1 && mob2)
        {
            double distance = mob1->GetDistanceFrom(mob2);
            
            // Calculate realistic RSSI using log-distance path loss model
            // Reference: ETSI TS 103 636-2 for DECT-2020 NR
            double txPowerDbm = 10.0;  // Typical DECT TX power
            double pathLossDb = CalculatePathLoss(distance, 1881.792e6); // Band 1 center frequency
            double realisticRssiDbm = txPowerDbm - pathLossDb;
            
            NS_LOG_INFO("Realistic RSSI between nodes: " << realisticRssiDbm 
                       << " dBm (distance: " << distance << " m, path loss: " << pathLossDb << " dB)");
        }
    }
    
    // Schedule the next RSSI monitoring call in 1 millisecond
    Simulator::Schedule(MilliSeconds(1), &MonitorRssi, std::ref(channelManager), bandNumber, devices);
}

int
main(int argc, char* argv[])
{
    Simulator::Stop(Seconds(10));

    LogComponentEnable("Dect2020Mac", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtSimulation1", LOG_LEVEL_INFO);
    // Enable logging for RSSI monitoring
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnableAll(LOG_PREFIX_FUNC);
    uint8_t bandNumber = 1;

    Dect2020ChannelManager channelManager;
    channelManager.InitializeChannels(bandNumber, 1); // Band 1, Scaling Factor 1

    // 1 FT and 1 PT
    NodeContainer ftNodes;
    ftNodes.Create(1);
    NodeContainer ptNodes;
    ptNodes.Create(1);

    MobilityHelper ftMobility;
    Ptr<ListPositionAllocator> ftPos = CreateObject<ListPositionAllocator>();
    ftPos->Add(Vector(0.0, 0.0, 0.0));
    ftMobility.SetPositionAllocator(ftPos);
    ftMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ftMobility.Install(ftNodes);

    MobilityHelper ptMobility;
    ptMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ptMobility.Install(ptNodes);

    Ptr<MobilityModel> mob = ptNodes.Get(0)->GetObject<MobilityModel>();
    mob->SetPosition(Vector(0.0, 50.0, 0.0));

    // Channel
    Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();
    Ptr<ConstantSpeedPropagationDelayModel> delayModel =
        CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LogDistancePropagationLossModel> lossModel =
        CreateObject<LogDistancePropagationLossModel>();
    channel->SetPropagationDelayModel(delayModel);
    channel->AddPropagationLossModel(lossModel);

    NetDeviceContainer devices;

    // FT-Devices
    for (uint32_t i = 0; i < ftNodes.GetN(); ++i)
    {
        Ptr<Dect2020NetDevice> dev = CreateObject<Dect2020NetDevice>();
        dev->SetBandNumber(bandNumber);
        dev->SetTerminationPointType(TermPointType::FT);

        Ptr<Dect2020Mac> mac = CreateObject<Dect2020Mac>();
        Ptr<Dect2020Phy> phy = CreateObject<Dect2020Phy>();

        dev->SetMac(mac);
        dev->SetPhy(phy);
        mac->SetNetDevice(dev);
        mac->SetPhy(phy);
        phy->SetMac(mac);
        ftNodes.Get(i)->AddDevice(dev);
        phy->SetDevice(dev);
        phy->SetMobility(ftNodes.Get(i)->GetObject<MobilityModel>());
        phy->SetChannel(channel);
        channel->AddRx(phy);

        // Antenna
        Ptr<IsotropicAntennaModel> antenna = CreateObject<IsotropicAntennaModel>();
        DoubleValue rxGain = mac->GetRxGainFromIndex(dev->m_rxGain);
        antenna->SetAttribute("Gain", rxGain);
        phy->SetAntenna(antenna);

        dev->SetAddress(Mac48Address::Allocate());
        dev->SetLinkUp();
        devices.Add(dev);
        phy->Start();
        mac->Start();
    }

    // PT-Devices
    for (uint32_t i = 0; i < ptNodes.GetN(); ++i)
    {
        Ptr<Dect2020NetDevice> dev = CreateObject<Dect2020NetDevice>();
        dev->SetBandNumber(bandNumber);
        dev->SetTerminationPointType(TermPointType::PT);

        Ptr<Dect2020Mac> mac = CreateObject<Dect2020Mac>();
        Ptr<Dect2020Phy> phy = CreateObject<Dect2020Phy>();

        dev->SetMac(mac);
        dev->SetPhy(phy);
        mac->SetNetDevice(dev);
        mac->SetPhy(phy);
        phy->SetMac(mac);
        ptNodes.Get(i)->AddDevice(dev);
        phy->SetDevice(dev);
        phy->SetMobility(ptNodes.Get(i)->GetObject<MobilityModel>());
        phy->SetChannel(channel);
        channel->AddRx(phy);

        // Antenna
        Ptr<IsotropicAntennaModel> antenna = CreateObject<IsotropicAntennaModel>();
        DoubleValue rxGain = mac->GetRxGainFromIndex(dev->m_rxGain);
        antenna->SetAttribute("Gain", rxGain); // z. B. 5 dBi RX Gain
        phy->SetAntenna(antenna);

        dev->SetAddress(Mac48Address::Allocate());
        dev->SetLinkUp();
        devices.Add(dev);
        phy->Start();
        mac->Start();
    }

    // Start RSSI monitoring for all channels in the band every millisecond
    Simulator::Schedule(MilliSeconds(0), &MonitorRssi, std::ref(channelManager), bandNumber, devices);

    Simulator::Run();

    NS_LOG_UNCOND("Simulation finished. Evaluating successfull sent Packets");
    NS_LOG_UNCOND("All Packets sent Count: " << Dect2020Statistics::GetSumOfAllPacketsSent());
    NS_LOG_UNCOND(
        "Dropped Packets due to low RSSI: " << Dect2020Statistics::GetPacketsDroppedLowRssiCount());
    NS_LOG_UNCOND("Dropped Packets due to Collision: "
                  << Dect2020Statistics::GetPacketsDroppedCollisionCount());

    Simulator::Destroy();
    return 0;
}