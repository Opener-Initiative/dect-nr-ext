#ifndef DECT2020EXT_CHANNEL_MANAGER_H
#define DECT2020EXT_CHANNEL_MANAGER_H

#include "dect2020ext-channel.h"

#include "ns3/object.h"
#include "ns3/spectrum-model.h"
#include "ns3/spectrum-value.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-analyzer.h"
#include "ns3/single-model-spectrum-channel.h"

#include <map>

namespace ns3
{

class Dect2020ExtChannel;

struct BandParameters
{
public:
    uint8_t bandNumber;
    double startFrequency;
    double stopFrequency;
    double frequencyStep;
    uint16_t nStart;
    uint16_t nEnd;
};

class Dect2020ExtChannelManager : public Object
{

public:
    Dect2020ExtChannelManager();
    virtual ~Dect2020ExtChannelManager();

    static TypeId GetTypeId(void);

    void InitializeChannels(uint8_t bandNumber,
                            uint8_t subcarrierScalingFactor);
    static BandParameters GetBandParameters(uint8_t bandNumber);
    double CalculateCenterFrequency(uint8_t bandNumber, uint32_t channelNumber);
    static std::vector<Ptr<Dect2020ExtChannel>> GetValidChannels(uint8_t bandNumber);
    static std::map<uint16_t, std::vector<Ptr<Dect2020ExtChannel>>> GetChannels(void);

    static uint32_t GetSubslotsPerSlot(void);
    static uint8_t GetBandNumber(uint16_t channelNumber);
    static uint16_t GetFirstValidChannelNumber(uint8_t bandNumber);

    static Ptr<SpectrumModel> GetSpectrumModel(uint8_t bandId);
    static void MeasureRssi(TimeValue duration);
    static double DbmToW(double dBm);
    static double WToDbm(double w);

    static void SpectrumAnalyzerCallbackWithAnalyzer(Ptr<SpectrumAnalyzer> analyzer, Ptr<const SpectrumValue> spectrumValue);
    static void DisplayRssiResults(void);
    static std::map<uint16_t, std::vector<double>> GetRssiResults(void);

    static void SetSpectrumChannel(Ptr<SingleModelSpectrumChannel> spectrumChannel);
    static Ptr<SingleModelSpectrumChannel> GetSpectrumChannel(void);

private:
    static std::map<uint16_t, std::vector<Ptr<Dect2020ExtChannel>>> m_channels;
    static std::map<uint8_t, Ptr<SpectrumValue>> m_channelOccupancy;
    static uint8_t m_subcarrierScalingFactor;

    // Spectrum
    static Ptr<SingleModelSpectrumChannel> m_spectrumChannel;

    static std::map<uint16_t, std::vector<double>> m_rssiResults; // uint16_t channelId -> double rssi

}; // class Dect2020ExtChannelManager

}; // namespace ns3

#endif // DECT2020EXT_CHANNEL_MANAGER_H