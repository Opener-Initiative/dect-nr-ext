#ifndef DECT2020EXT_CHANNEL_MANAGER_H
#define DECT2020EXT_CHANNEL_MANAGER_H

#include "ns3/dect2020ext-net-device.h"

#include "ns3/object.h"
#include "ns3/spectrum-model.h"
#include "ns3/spectrum-value.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-analyzer.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/uinteger.h"

#include <map>

namespace ns3
{

class Dect2020ExtNetDevice;

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

        static TypeId GetTypeId();

        BandParameters GetBandParameters(uint8_t bandNumber);
        double CalculateCenterFrequency(uint8_t bandNumber, uint32_t channelNumber);
        uint32_t GetSubslotsPerSlot();
        uint8_t GetBandNumber(uint16_t channelNumber);
        uint16_t GetFirstValidChannelNumber(uint8_t bandNumber);
        Ptr<SpectrumModel> GetSpectrumModel(uint8_t bandId);

        // Calculation

        double DbmToW(double dBm);
        double WToDbm(double w);
        double CalculateRssiFromPower(double bandPower_W);

        // Configuration

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber() const;
        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor() const;
        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor() const;

    private:
        std::map<uint8_t, Ptr<SpectrumModel>> m_bandModels; // uint8_t bandNumber -> Ptr<SpectrumModel>

        // Configuration 

        uint8_t m_bandNumber;
        uint8_t m_subcarrierScalingFactor;
        uint8_t m_fourierTransformScalingFactor;
};

}; // namespace ns3

#endif // DECT2020EXT_CHANNEL_MANAGER_H