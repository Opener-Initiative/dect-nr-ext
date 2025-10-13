// Note: for RSSI measurements use completion callbacks or signals/events:
// Add to header:
// static std::function<void(double)> m_onMeasurementComplete;

// // In callback:
// if (m_onMeasurementComplete) {
//     m_onMeasurementComplete(rssi_dBm);
// }

// // Usage:
// Dect2020ExtChannelManager::m_onMeasurementComplete = [](double rssi) {
//     std::cout << "Measurement complete: " << rssi << " dBm" << std::endl;
// };

// Add to header:
// static TracedCallback<double> m_measurementTrace;

// // In callback:
// m_measurementTrace(rssi_dBm);

// // Usage: Connect your own callback to get notified

#include "dect2020ext-channel-manager.h"

#include "ns3/spectrum-analyzer.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/simulator.h"
#include "ns3/double.h"

#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannelManager");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtChannelManager);

// Define static member variables

Dect2020ExtChannelManager::Dect2020ExtChannelManager()
{
    m_spectrumChannel = nullptr;
}

Dect2020ExtChannelManager::~Dect2020ExtChannelManager()
{
}

TypeId
Dect2020ExtChannelManager::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtChannelManager")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtChannelManager>()
                                .AddTraceSource("RssiMeasured",
                                                "RSSI Measurement",
                                                MakeTraceSourceAccessor(&Dect2020ExtChannelManager::m_rssiMeasured),
                                                "ns3::TracedCallback::Uint16Double");
    return tid;
}



// Initializes logic channels
void
Dect2020ExtChannelManager::InitializeChannels(uint8_t bandNumber,
                                              uint8_t subcarrierScalingFactor)
{
    NS_LOG_FUNCTION(this);

    // ###################################
    // #### Initialize logic channels ####
    // ###################################

    // Check if band number is valid (currently only 1 band is supported)
    if (bandNumber != 1)
    {
        NS_FATAL_ERROR("Invalid band number. Currently only band 1 is supported.");
    }
    m_bandNumber = bandNumber;

    // Check if subcarrier scaling factor is valid
    if (!(subcarrierScalingFactor == 1 || subcarrierScalingFactor == 2 ||
          subcarrierScalingFactor == 4 || subcarrierScalingFactor == 8))
    {
        NS_FATAL_ERROR("Invalid subcarrier scaling factor");
    }

    m_subcarrierScalingFactor = subcarrierScalingFactor;
    const uint32_t slotsPerFrame = 24; // #ETSI TS 103 636-3 V1.5.1, Section 4.4

    BandParameters bandParams = GetBandParameters(bandNumber); // Get current band parameters
    uint32_t numChannels = (bandParams.nEnd - bandParams.nStart) + 1; // Number of Channels

    // Compute the subslot duration in nanoseconds
    uint32_t numSubslotsPerSlot = GetSubslotsPerSlot();
    const double slotDurationNs = 10000000 / slotsPerFrame;         // 0,41667 milliseconds in nanoseconds
    double subslotDurationNs = slotDurationNs / numSubslotsPerSlot; // Subslot Duration in nanoseconds

    // Initialize the logical channels
    m_channels.clear();
    for (uint32_t ch = 0; ch < numChannels; ch++)
    {
        int channelFrequencyNumbering = bandParams.nStart + ch;

        Ptr<Dect2020ExtChannel> dect2020ExtChannel = Create<Dect2020ExtChannel>();
        dect2020ExtChannel->m_channelId = channelFrequencyNumbering;
        dect2020ExtChannel->m_centerFrequency =
            CalculateCenterFrequency(bandNumber, channelFrequencyNumbering);

        for (uint32_t slot = 0; slot < slotsPerFrame; slot++)
        {
            Slot slotObj;
            slotObj.slotId = slot;

            for (uint32_t ss = 0; ss < numSubslotsPerSlot; ss++)
            {
                Subslot subslot;
                subslot.subslotId = ss;
                subslot.status = SubslotStatus::FREE;
                subslot.rssi = 0.0;
                subslot.subslotDurationNs = subslotDurationNs;
                slotObj.subslots.push_back(subslot);
            }

            dect2020ExtChannel->AddSlot(slotObj); // Add the current Slot to the Channel
        }

        m_channels[bandNumber].push_back(dect2020ExtChannel);
    }
}

BandParameters
Dect2020ExtChannelManager::GetBandParameters(uint8_t bandNumber)
{
    // Note: Currently only Bands 1, 2, 20, and 21 are implemented.
    BandParameters bp;
    bp.bandNumber = bandNumber;

    switch (bandNumber)
    {
    case 1:
        bp.startFrequency = 1881.792e6;
        bp.stopFrequency = 1899.072e6;
        bp.frequencyStep = 0.864e6;
        bp.nStart = 1657;
        bp.nEnd = 1677;

        break;

    case 2:
        bp.startFrequency = 1901.664e6;
        bp.stopFrequency = 1918.994e6;
        bp.frequencyStep = 0.864e6;
        bp.nStart = 1680;
        bp.nEnd = 1700;

        break;

    case 20:
        bp.startFrequency = 3802.388e6;
        bp.stopFrequency = 4198.1e6;
        bp.frequencyStep = 1.728e6;
        bp.nStart = 3416;
        bp.nEnd = 3645;

        break;

    case 21:
        bp.startFrequency = 3700.436e6;
        bp.stopFrequency = 3798.932e6;
        bp.frequencyStep = 1.728e6;
        bp.nStart = 3357;
        bp.nEnd = 3414;

        break;

    default:
        NS_FATAL_ERROR("Invalid band number. Supported bands are 1, 2, 20, and 21.");
    }

    return bp;
}

double
Dect2020ExtChannelManager::CalculateCenterFrequency(uint8_t bandNumber, uint32_t channelNumber)
{
    double centerFrequency = 0;

    if ((bandNumber >= 1 && bandNumber <= 12) || bandNumber == 22)
    {
        double f0 = 450.144e6; // F0 in Hz
        double channelRaster = 0.864e6;

        centerFrequency = f0 + (channelNumber * channelRaster);
    }
    // TODO: Add more Bands

    return centerFrequency;
}

std::vector<Ptr<Dect2020ExtChannel>>
Dect2020ExtChannelManager::GetValidChannels(uint8_t bandNumber)
{
    std::vector<Ptr<Dect2020ExtChannel>> validChannels;

    auto it = m_channels.find(bandNumber);
    if (it != m_channels.end())
    {
        validChannels = it->second;
    }
    return validChannels;
}

std::vector<Ptr<Dect2020ExtChannel>>
Dect2020ExtChannelManager::GetChannels(void)
{
    return m_channels[m_bandNumber];
}

uint32_t
Dect2020ExtChannelManager::GetSubslotsPerSlot(void)
{    
    uint32_t numSubslotsPerSlot = (m_subcarrierScalingFactor == 1)   ? 2
                                  : (m_subcarrierScalingFactor == 2) ? 4
                                  : (m_subcarrierScalingFactor == 4) ? 8
                                                                   : 16;
    return numSubslotsPerSlot;
}

uint8_t
Dect2020ExtChannelManager::GetBandNumber(uint16_t channelNumber)
{
    struct BandChannelRange
    {
        uint8_t bandNumber;
        uint16_t nStart;
        uint16_t nEnd;
    };

    static const std::vector<BandChannelRange> bandMap = {{1, 1657, 1677},
                                                          {2, 1680, 1700},
                                                          {20, 3416, 3645},
                                                          {21, 3357, 3414}};

    for (const auto& band : bandMap)
    {
        if (channelNumber >= band.nStart && channelNumber <= band.nEnd)
        {
            return band.bandNumber;
        }
    }

    return -1; // Invalid band number
}

uint16_t
Dect2020ExtChannelManager::GetFirstValidChannelNumber(uint8_t bandNumber)
{
    switch (bandNumber)
    {
    case 1:

        return 1657;
        break;

    case 2:

        return 1680;
        break;

    case 20:

        return 3416;
        break;

    case 21:

        return 3357;
        break;

    default:
        NS_FATAL_ERROR("Invalid band number. Supported bands are 1, 2, 20, and 21.");
    }
}

Ptr<SpectrumModel>
Dect2020ExtChannelManager::GetSpectrumModel(uint8_t bandId)
{
    // Check if a SpectrumModel for this band already exists
    auto iterator = m_bandModels.find(bandId);
    if (iterator != m_bandModels.end())
    {
        return iterator->second;
    }

    // Spectrum Model for this band does not exist --> create it
    BandParameters bp = GetBandParameters(bandId);

    // Generate a list of center frequencies, one per DECT channel
    std::vector<double> frequencies;
    for (int i = 0; i < bp.nEnd - bp.nStart + 1; i++)
    {
        frequencies.push_back(bp.startFrequency + i * bp.frequencyStep);
    }

    // Create the SpectrumModel from the calculated frequencies
    Ptr<SpectrumModel> model = Create<SpectrumModel>(frequencies);
    m_bandModels[bandId] = model;

    return model;
}

// Global variables to store spectrum analyzer results
static std::map<uint16_t, double> g_channelRssiResults;
static std::map<uint16_t, bool> g_channelMeasurementComplete;

void
Dect2020ExtChannelManager::DisplayRssiResults(void)
{
    if (m_rssiResults.empty())
    {
        std::cout << "No RSSI results available" << std::endl;
        return;
    }
    else
    {
        for (const auto& channelPair : m_rssiResults)
        {
            uint16_t channelId = channelPair.first;
            const std::vector<double>& rssiValues = channelPair.second;
            
            std::cout << "Channel " << channelId << " RSSI values:" << rssiValues.size() << std::endl;
        }
    }
}

std::map<uint16_t, std::vector<double>>
Dect2020ExtChannelManager::GetRssiResults(void)
{
    return m_rssiResults;
}

void
Dect2020ExtChannelManager::SpectrumAnalyzerCbThunk(Dect2020ExtChannelManager* self, uint16_t channelId, Ptr<SpectrumAnalyzer> analyzer, Ptr<const SpectrumValue> spectrumValue)
{
    self->SpectrumAnalyzerCallbackWithAnalyzer(channelId, analyzer, spectrumValue);
}

void
Dect2020ExtChannelManager::SpectrumAnalyzerCallbackWithAnalyzer(uint16_t channelId, Ptr<SpectrumAnalyzer> analyzer, Ptr<const SpectrumValue> spectrumValue)
{
    NS_LOG_INFO("RSSI Measured at: " << Simulator::Now().GetNanoSeconds() << " ns");
    static int reportCount = 0;
    reportCount++;
    
    // Get the spectrum model to access center frequencies
    Ptr<const SpectrumModel> spectrumModel = spectrumValue->GetSpectrumModel();
    
    // NS_LOG_INFO("Report #" << reportCount << " at time " << Simulator::Now().GetSeconds() << "s:");
    
    auto modelIt = spectrumModel->Begin();
    auto valueIt = spectrumValue->ConstValuesBegin();
    auto logicChannelIt = m_channels[1].begin();
    
    for (; modelIt != spectrumModel->End() && valueIt != spectrumValue->ConstValuesEnd(); 
         ++modelIt, ++valueIt, ++logicChannelIt)
    {
        double bandPower_W = *valueIt;

        // Calculate RSSI
        double rssi_dBm;
        
        if (bandPower_W > 0) {
            double bandPower_mW = bandPower_W * 1000.0;
            rssi_dBm = 10.0 * std::log10(bandPower_mW);
        } else {
            // Handle very small or zero power
            rssi_dBm = -std::numeric_limits<double>::infinity();
            std::cout << "  WARNING: Very small or zero power detected: " << bandPower_W << " W" << std::endl;
        }

        uint16_t currentChannelId = (*logicChannelIt)->m_channelId;
        
        if (currentChannelId == channelId)
        {
            m_rssiMeasured(channelId, rssi_dBm);
        }
        m_rssiResults[currentChannelId].push_back(rssi_dBm);
    }
    
    // NS_LOG_INFO("Stopping SpectrumAnalyzer after first report...");
    analyzer->Stop();
}

void
Dect2020ExtChannelManager::MeasureRssi(uint16_t channelId, TimeValue duration)
{
    if (m_spectrumChannel == nullptr)
    {
        NS_FATAL_ERROR("Spectrum channel is not set for Dect2020ExtChannelManager");
    }

    // Create a NEW spectrum analyzer for each measurement
    Ptr<SpectrumAnalyzer> spectrumAnalyzer = CreateObject<SpectrumAnalyzer>();

    spectrumAnalyzer->SetRxSpectrumModel(GetSpectrumModel(1));

    spectrumAnalyzer->SetAttribute("Resolution", duration);

    // Use MakeBoundCallback with static thunk to bind channelId and analyzer parameters
    spectrumAnalyzer->TraceConnectWithoutContext("AveragePowerSpectralDensityReport",
                                                  MakeBoundCallback(&Dect2020ExtChannelManager::SpectrumAnalyzerCbThunk, this, channelId, spectrumAnalyzer));
                                
    m_spectrumChannel->AddRx(spectrumAnalyzer);
    spectrumAnalyzer->Start();
}

void 
Dect2020ExtChannelManager::SetSpectrumChannel(Ptr<SingleModelSpectrumChannel> spectrumChannel)
{
    m_spectrumChannel = spectrumChannel;
}

Ptr<SingleModelSpectrumChannel>
Dect2020ExtChannelManager::GetSpectrumChannel(void)
{
    return m_spectrumChannel;
}

double
Dect2020ExtChannelManager::DbmToW(double dBm)
{
    return std::pow(10.0, 0.1 * (dBm - 30.0));
}

double
Dect2020ExtChannelManager::WToDbm(double w)
{
    return 10.0 * std::log10(w) + 30.0;
}

}; // namespace ns3