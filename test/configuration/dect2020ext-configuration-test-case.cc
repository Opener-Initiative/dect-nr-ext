#include "dect2020ext-configuration-test-case.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"

namespace ns3
{

Dect2020ExtConfigurationTestCase::Dect2020ExtConfigurationTestCase()
    : TestCase("Dect2020ExtConfigurationTestCase")
{
}

Dect2020ExtConfigurationTestCase::~Dect2020ExtConfigurationTestCase()
{
}

void
Dect2020ExtConfigurationTestCase::DoSetup()
{
}

void
Dect2020ExtConfigurationTestCase::DoRun()
{
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();

    // Configuration
    channelManager->SetBandNumber(6);
    channelManager->SetSubcarrierScalingFactor(6);
    channelManager->SetFourierTransformScalingFactor(6);
    // channelManager->Configure();

    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();
    Ptr<Dect2020ExtPhy> phy = CreateObject<Dect2020ExtPhy>();

    mac->PrintConfiguration();
}

} // namespace ns3