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

using namespace ns3;
using TermPointType = Dect2020NetDevice::TerminationPointType;

int
main(int argc, char* argv[])
{
    Simulator::Stop(Seconds(10));

    LogComponentEnable("Dect2020Phy", LOG_LEVEL_FUNCTION);
    LogComponentEnable("Dect2020Mac", LOG_LEVEL_FUNCTION);
    LogComponentEnableAll(LOG_PREFIX_TIME);
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