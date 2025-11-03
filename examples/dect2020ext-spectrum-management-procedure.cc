#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/node.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtSpectrumManagementProcedure");

int
main(int argc, char *argv[])
{
    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->InitializeChannels();

    // FT PHY
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();

    // FT MAC
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetPhy(ftPHY);
    ftPHY->SetMac(ftMAC);

    // FT Net Device
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::FT);

    Simulator::Run();
    Simulator::Stop();
    Simulator::Destroy();
    return 0;
}