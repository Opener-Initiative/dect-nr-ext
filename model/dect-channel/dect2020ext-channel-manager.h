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
    BandParameters GetBandParameters(uint8_t bandNumber);
    double CalculateCenterFrequency(uint8_t bandNumber, uint32_t channelNumber);
    std::vector<Ptr<Dect2020ExtChannel>> GetValidChannels(uint8_t bandNumber);
    std::vector<Ptr<Dect2020ExtChannel>> GetChannels(void);

    uint32_t GetSubslotsPerSlot(void);
    uint8_t GetBandNumber(uint16_t channelNumber);
    uint16_t GetFirstValidChannelNumber(uint8_t bandNumber);

    Ptr<SpectrumModel> GetSpectrumModel(uint8_t bandId);
    void MeasureRssi(uint16_t channelId, TimeValue duration);
    double DbmToW(double dBm);
    double WToDbm(double w);

    static void SpectrumAnalyzerCbThunk(Dect2020ExtChannelManager* self, uint16_t channelId, Ptr<SpectrumAnalyzer> analyzer, Ptr<const SpectrumValue> spectrumValue);
    void SpectrumAnalyzerCallbackWithAnalyzer(uint16_t channelId, Ptr<SpectrumAnalyzer> analyzer, Ptr<const SpectrumValue> spectrumValue);

    void DisplayRssiResults(void);
    std::map<uint16_t, std::vector<double>> GetRssiResults(void);

    void SetSpectrumChannel(Ptr<SingleModelSpectrumChannel> spectrumChannel);
    Ptr<SingleModelSpectrumChannel> GetSpectrumChannel(void);
    TracedCallback<uint16_t, double> m_rssiMeasured; // (channelId, rssiDbm)

private:
    std::map<uint16_t, std::vector<Ptr<Dect2020ExtChannel>>> m_channels;
    std::map<uint8_t, Ptr<SpectrumValue>> m_channelOccupancy;
    uint8_t m_subcarrierScalingFactor;
    uint8_t m_bandNumber;

    // Spectrum
    Ptr<SingleModelSpectrumChannel> m_spectrumChannel;

    std::map<uint16_t, std::vector<double>> m_rssiResults; // uint16_t channelId -> double rssi
    std::map<uint8_t, Ptr<SpectrumModel>> m_bandModels;

}; // class Dect2020ExtChannelManager

}; // namespace ns3

#endif // DECT2020EXT_CHANNEL_MANAGER_H