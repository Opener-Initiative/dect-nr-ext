#include "dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/spectrum-analyzer.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-module.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/pointer.h"

#include <vector>
#include <cmath>
#include <limits>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannelManager");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtChannelManager);

Dect2020ExtChannelManager::Dect2020ExtChannelManager()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtChannelManager::~Dect2020ExtChannelManager()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtChannelManager::GetTypeId()
{
    NS_LOG_FUNCTION_NOARGS();
    static TypeId tid = TypeId("ns3::Dect2020ExtChannelManager")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtChannelManager>();
    return tid;
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
    NS_LOG_FUNCTION(this << static_cast<uint32_t>(bandNumber) << channelNumber);
    double centerFrequency = 0;

    if ((bandNumber >= 1 && bandNumber <= 12) || bandNumber == 22)
    {
        double f0 = 450.144e6; // F0 in Hz
        double channelRaster = 0.864e6;

        centerFrequency = f0 + (channelNumber * channelRaster);
    }
    // TODO: Add more bands if required

    return centerFrequency;
}

uint32_t
Dect2020ExtChannelManager::GetSubslotsPerSlot()
{
    NS_LOG_FUNCTION(this);
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
    NS_LOG_FUNCTION(this << static_cast<uint32_t>(bandId));

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

double
Dect2020ExtChannelManager::DbmToW(double dBm)
{
    NS_LOG_FUNCTION(this << dBm);
    return std::pow(10.0, 0.1 * (dBm - 30.0));
}

double
Dect2020ExtChannelManager::WToDbm(double w)
{
    NS_LOG_FUNCTION(this << w);
    return 10.0 * std::log10(w) + 30.0;
}

double
Dect2020ExtChannelManager::CalculateRssiFromPower(double bandPower_W)
{
    NS_LOG_FUNCTION(this << bandPower_W);
    
    if (bandPower_W > 0)
    {
        double bandPower_mW = bandPower_W * 1000.0;
        return 10.0 * std::log10(bandPower_mW);
    }
    else
    {
        // Handle very small or zero power
        NS_LOG_WARN("Very small or zero power detected: " << bandPower_W << " W");
        return -std::numeric_limits<double>::infinity();
    }
}

void
Dect2020ExtChannelManager::SetBandNumber(uint8_t bandNumber)
{
    NS_LOG_FUNCTION(this << static_cast<uint32_t>(bandNumber));
    m_bandNumber = bandNumber;
}

uint8_t
Dect2020ExtChannelManager::GetBandNumber() const
{
    NS_LOG_FUNCTION(this);
    return m_bandNumber;
}   

void
Dect2020ExtChannelManager::SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor)
{
    NS_LOG_FUNCTION(this << static_cast<uint32_t>(subcarrierScalingFactor));
    m_subcarrierScalingFactor = subcarrierScalingFactor;
}

uint8_t
Dect2020ExtChannelManager::GetSubcarrierScalingFactor() const
{
    NS_LOG_FUNCTION(this);
    return m_subcarrierScalingFactor;
}

void
Dect2020ExtChannelManager::SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor)
{
    NS_LOG_FUNCTION(this << static_cast<uint32_t>(fourierTransformScalingFactor));
    m_fourierTransformScalingFactor = fourierTransformScalingFactor;
}

uint8_t
Dect2020ExtChannelManager::GetFourierTransformScalingFactor() const
{
    NS_LOG_FUNCTION(this);
    return m_fourierTransformScalingFactor;
}

}; // namespace ns3