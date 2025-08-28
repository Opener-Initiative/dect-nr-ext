#ifndef DECT2020EXT_CHANNEL_MANAGER_H
#define DECT2020EXT_CHANNEL_MANAGER_H

#include "ns3/object.h"
#include "dect2020ext-channel.h"
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
                            uint8_t subcarrierScalingFactor,
                            uint8_t transmissionScalingFactor);
    BandParameters GetBandParameters(uint8_t bandNumber);
    double CalculateCenterFrequency(uint8_t bandNumber, uint32_t channelNumber);

private:
    std::map<uint16_t, std::vector<Ptr<Dect2020ExtChannel>>> m_channels;

}; // class Dect2020ExtChannelManager

}; // namespace ns3

#endif // DECT2020EXT_CHANNEL_MANAGER_H