#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/spectrum-analyzer.h"
#include "ns3/spectrum-model.h"
#include "ns3/simulator.h"
#include "ns3/waveform-generator.h"
#include "ns3/spectrum-value.h"
#include "ns3/mobility-module.h"
#include "ns3/double.h"

#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannelSelection");

static int reportCount = 0;
static Ptr<SpectrumAnalyzer> globalSpectrumAnalyzer; // Global reference to stop the analyzer

void
SpectrumAnalyzerReport(Ptr<const SpectrumValue> spectrumValue)
{
    reportCount++;
    NS_LOG_INFO("SpectrumAnalyzerReport #" << reportCount);
    
    // Get the spectrum model to access center frequencies
    Ptr<const SpectrumModel> spectrumModel = spectrumValue->GetSpectrumModel();
    
    std::cout << "Report #" << reportCount << " at time " << Simulator::Now().GetSeconds() << "s:" << std::endl;
    
    // Calculate RSSI for each frequency band separately
    size_t bandIndex = 0;
    
    auto modelIt = spectrumModel->Begin();
    auto valueIt = spectrumValue->ConstValuesBegin();
    
    for (; modelIt != spectrumModel->End() && valueIt != spectrumValue->ConstValuesEnd(); 
         ++modelIt, ++valueIt, ++bandIndex)
    {
        double bandPower_W = *valueIt;
        double centerFreq_MHz = modelIt->fc / 1e6; // Convert Hz to MHz
        
        // Calculate RSSI for this specific frequency band
        double rssi_dBm;
        if (bandPower_W > 0) {
            double bandPower_mW = bandPower_W * 1000.0;
            rssi_dBm = 10.0 * std::log10(bandPower_mW);
        } else {
            rssi_dBm = -std::numeric_limits<double>::infinity();
        }
        
        std::cout << "  Channel " << std::setfill(' ') << std::setw(2) << bandIndex 
                  << " - Freq: " << std::fixed << std::setprecision(3) << centerFreq_MHz 
                  << " MHz, RSSI: " << std::setprecision(1) << rssi_dBm << " dBm";
        
        // Highlight channels with significant traffic (above thermal noise)
        if (bandPower_W > 1e-15) { // Much higher than thermal noise (~4e-21)
            std::cout << " <-- TRAFFIC DETECTED!";
            if (bandPower_W > 5e-15) {
                std::cout << " [HIGH TRAFFIC]";
            } else if (bandPower_W > 1e-15) {
                std::cout << " [MEDIUM TRAFFIC]";
            } else {
                std::cout << " [LOW TRAFFIC]";
            }
        }
        std::cout << std::endl;
    }
    
    // Stop the analyzer after the first report
    if (reportCount == 1 && globalSpectrumAnalyzer) {
        std::cout << "Stopping SpectrumAnalyzer after first report..." << std::endl;
        globalSpectrumAnalyzer->Stop();
    }
}

int
main(int argc, char *argv[])
{

    LogComponentEnable("SpectrumAnalyzer", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020ExtChannelSelection", LOG_LEVEL_INFO);

    // Create spectrum channel
    Ptr<ConstantSpeedPropagationDelayModel> propagationDelay = CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LogDistancePropagationLossModel> propagationLoss = CreateObject<LogDistancePropagationLossModel>();
    Ptr<SingleModelSpectrumChannel> spectrumChannel = CreateObject<SingleModelSpectrumChannel>();

    spectrumChannel->SetPropagationDelayModel(propagationDelay);
    spectrumChannel->AddPropagationLossModel(propagationLoss);

    // Create spectrum analyzer 
    Ptr<SpectrumAnalyzer> spectrumAnalyzer = CreateObject<SpectrumAnalyzer>();
    globalSpectrumAnalyzer = spectrumAnalyzer; // Store global reference for callback
    
    // Create spectrum model with 21 DECT-NR channels (real DECT-NR frequency plan)
    // Starting from 1881.792 MHz with 0.864 MHz spacing
    std::vector<double> channelFrequencies;
    double startFrequency = 1881.792e6;  // First channel frequency  
    double frequencyStep = 0.864e6;      // Channel spacing
    
    std::cout << "Creating 21 DECT-NR channels:" << std::endl;
    for (int i = 0; i < 21; i++) {
        double freq = startFrequency + i * frequencyStep;
        channelFrequencies.push_back(freq);
        std::cout << "  Channel " << i << ": " << std::fixed << std::setprecision(3) 
                  << freq / 1e6 << " MHz" << std::endl;
    }
    std::cout << std::endl;
    
    Ptr<SpectrumModel> spectrumModel = Create<SpectrumModel>(channelFrequencies);
    spectrumAnalyzer->SetRxSpectrumModel(spectrumModel);
    
    // Set resolution to 0.41667ms for very frequent reports
    spectrumAnalyzer->SetAttribute("Resolution", TimeValue(MicroSeconds(416.67)));
    
    // Connect trace callback
    spectrumAnalyzer->TraceConnectWithoutContext("AveragePowerSpectralDensityReport",
                                                  MakeCallback(&SpectrumAnalyzerReport));
    
    // Create nodes with mobility for transmitter and receiver
    NodeContainer nodes;
    nodes.Create(2);
    
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);
    
    // Position transmitter at (0,0,0) and receiver at (100,0,0) - 100m apart
    Ptr<MobilityModel> txMobility = nodes.Get(0)->GetObject<MobilityModel>();
    Ptr<MobilityModel> rxMobility = nodes.Get(1)->GetObject<MobilityModel>();
    txMobility->SetPosition(Vector(0.0, 0.0, 0.0));
    rxMobility->SetPosition(Vector(100.0, 0.0, 0.0));
    
    // Create transmitter - simulates traffic on multiple channels
    Ptr<WaveformGenerator> transmitter = CreateObject<WaveformGenerator>();
    transmitter->SetMobility(txMobility);
    transmitter->SetChannel(spectrumChannel);
    
    // Create PSD for transmission - simulate traffic on DIFFERENT channels
    Ptr<SpectrumValue> txPsd = Create<SpectrumValue>(spectrumModel);
    
    // Initialize all 21 channels to zero power (no traffic)
    for (size_t i = 0; i < 21; i++) {
        (*txPsd)[i] = 0.0;
    }
    
    // Add traffic on specific channels to simulate real-world scenario
    (*txPsd)[3] = 0.01;   // 10 mW on Channel 3  (1884.384 MHz) - Light traffic
    (*txPsd)[7] = 0.05;   // 50 mW on Channel 7  (1888.704 MHz) - Medium traffic  
    (*txPsd)[12] = 0.1;   // 100 mW on Channel 12 (1892.160 MHz) - Heavy traffic (BEST CHANNEL!)
    (*txPsd)[18] = 0.02;  // 20 mW on Channel 18 (1897.344 MHz) - Light traffic
    
    transmitter->SetTxPowerSpectralDensity(txPsd);
    
    // Configure transmission parameters
    transmitter->SetAttribute("Period", TimeValue(MilliSeconds(1))); // Transmit every 1ms
    transmitter->SetAttribute("DutyCycle", DoubleValue(0.8));         // 80% duty cycle
    
    // Add spectrum analyzer to channel and start
    spectrumChannel->AddRx(spectrumAnalyzer);
    spectrumAnalyzer->SetMobility(rxMobility);
    spectrumAnalyzer->Start();
    
    // Start transmitter 
    transmitter->Start();
    
    // Run simulation for longer to prove the analyzer actually stopped
    Simulator::Stop(Seconds(1));

    std::cout << "=== MULTI-CHANNEL TRAFFIC SIMULATION ===" << std::endl;
    std::cout << "One SpectrumChannel, 21 frequency bands, traffic on channels 3,7,12,18" << std::endl;
    std::cout << "Transmitter at (0,0,0), Receiver at (100,0,0)" << std::endl;
    std::cout << "Expected: Different RSSI values per channel based on traffic" << std::endl;
    std::cout << std::endl;

    Simulator::Run();
    
    std::cout << "Simulation finished. Total reports received: " << reportCount << std::endl;
    if (reportCount == 0) {
        std::cout << "ERROR: No reports received - callback not connected properly!" << std::endl;
    } else {
        std::cout << "SUCCESS: Callback is connected and working!" << std::endl;
    }
    
    Simulator::Destroy();
    return 0;
}