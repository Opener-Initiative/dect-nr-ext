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
    uint8_t bandNumber = 6;
    uint8_t subcarrierScalingFactor = 6;
    uint8_t fourierTransformScalingFactor = 6;

    channelManager->SetBandNumber(bandNumber);
    channelManager->SetSubcarrierScalingFactor(subcarrierScalingFactor);
    channelManager->SetFourierTransformScalingFactor(fourierTransformScalingFactor);
    channelManager->Configure();

    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();
    Ptr<Dect2020ExtPhy> phy = CreateObject<Dect2020ExtPhy>();

    NS_TEST_ASSERT_MSG_EQ(mac->GetBandNumber(), bandNumber, "MAC: Band Number mismatch after configuration");
    NS_TEST_ASSERT_MSG_EQ(mac->GetSubcarrierScalingFactor(), subcarrierScalingFactor, "MAC: Subcarrier Scaling Factor mismatch after configuration");
    NS_TEST_ASSERT_MSG_EQ(mac->GetFourierTransformScalingFactor(), fourierTransformScalingFactor, "MAC: Fourier Transform Scaling Factor mismatch after configuration");
    NS_TEST_ASSERT_MSG_EQ(phy->GetBandNumber(), bandNumber, "PHY: Band Number mismatch after configuration");
    NS_TEST_ASSERT_MSG_EQ(phy->GetSubcarrierScalingFactor(), subcarrierScalingFactor, "PHY: Subcarrier Scaling Factor mismatch after configuration");
    NS_TEST_ASSERT_MSG_EQ(phy->GetFourierTransformScalingFactor(), fourierTransformScalingFactor, "PHY: Fourier Transform Scaling Factor mismatch after configuration");
}

} // namespace ns3