/*
This test case verifies the measurement duration for the backgound scan.
According to the DECT-2020 NR standard TS 103 636-4 Section 7.2 Variables:
SCAN_MEAS_DURATION = 24 slots (band 1, 2, 9, or 22)
*/

#include "dect2020ext-scan-measurement-duration-test-case.h"

#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-net-device.h"

#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/node.h"
#include "ns3/waveform-generator.h"
#include "ns3/spectrum-value.h"
#include "ns3/spectrum-model.h"
#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtScanMeasurementDurationTestCase::Dect2020ExtScanMeasurementDurationTestCase()
    : TestCase("Dect2020ExtScanMeasurementDurationTestCase")
{
}

Dect2020ExtScanMeasurementDurationTestCase::~Dect2020ExtScanMeasurementDurationTestCase()
{
}

void
Dect2020ExtScanMeasurementDurationTestCase::DoSetup()
{
}

static void DisableLogging()
{
    LogComponentDisable("Dect2020ExtMac", LOG_LEVEL_DEBUG);
}

void
Dect2020ExtScanMeasurementDurationTestCase::DoRun()
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_DEBUG | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));

    Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();
    channel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());
    channel->AddPropagationLossModel(CreateObject<LogDistancePropagationLossModel>());

    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->SetSpectrumChannel(channel);
    channelManager->Configure();
    channelManager->InitializeChannels();

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    ftPhy->SetChannel(channel);

    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
    ftMac->SetPhy(ftPhy);
    ftMac->SetTerminationPointType(Dect2020ExtNetDevice::FT);

    ftPhy->SetMac(ftMac);

    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMac);
    ftNetDevice->SetPhy(ftPhy);

    Ptr<Node> ftNode = CreateObject<Node>();
    ftNode->AddDevice(ftNetDevice);

    // Band 1 Spectrum Model
    Ptr<SpectrumModel> spectrumModel = channelManager->GetSpectrumModel(1);

    // Waveform Generator
    Ptr<WaveformGenerator> waveformGenerator = CreateObject<WaveformGenerator>();
    waveformGenerator->SetChannel(channel);

    // Create a PSD for transmission - simulate traffic on DIFFERENT channels
    Ptr<SpectrumValue> txPsd = Create<SpectrumValue>(spectrumModel);

    // Initialize all 21 channels to zero power (no traffic)
    for (size_t i = 0; i < 21; i++) {
        (*txPsd)[i] = 0.0;
    }

    // Add traffic on specific channels to simulate real-world scenario
    (*txPsd)[3] = 0.01;   // 10 mW on Channel 3  (1884.384 MHz) - Light traffic
    (*txPsd)[7] = 0.05;   // 50 mW on Channel 7  (1888.704 MHz) - Medium traffic  
    (*txPsd)[12] = 0.1;   // 100 mW on Channel 12 (1892.160 MHz) - Heavy traffic
    (*txPsd)[18] = 0.02;  // 20 mW on Channel 18 (1897.344 MHz) - Light traffic

    waveformGenerator->SetTxPowerSpectralDensity(txPsd);

    // Configure waveform generator transmission parameters
    waveformGenerator->SetAttribute("Period", TimeValue(MilliSeconds(1))); // Transmit every 1ms
    waveformGenerator->SetAttribute("DutyCycle", DoubleValue(0.8));         // 80% duty cycle

    Simulator::Schedule(Seconds(1), &WaveformGenerator::Start, waveformGenerator); // Start transmission when MAC backgound scan starts
    Simulator::Schedule(Seconds(1.22), &DisableLogging);
    ftMac->Start();

    Simulator::Stop(Seconds(2));
    Simulator::Run();
    Simulator::Destroy();
}

} // namespace ns3