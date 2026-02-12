#ifndef DECT2020EXT_INTERFERENCE_H
#define DECT2020EXT_INTERFERENCE_H

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/spectrum-value.h"

namespace ns3
{

class Dect2020ExtMac;

class Dect2020ExtInterference : public Object
{
    public:
        Dect2020ExtInterference();
        virtual ~Dect2020ExtInterference();

        static TypeId GetTypeId();

        void Initialize(Ptr<const SpectrumModel> spectrumModel);
        void AddSignal(Ptr<const SpectrumValue> psd, const Time duration, uint16_t channelIndex);
        void DoAddSignal(Ptr<const SpectrumValue> psd, uint16_t channelIndex);
        void DoSubtractSignal(double powerToSubtract, uint16_t channelIndex);

        Ptr<SpectrumValue> m_rxSignal{nullptr};
        Ptr<SpectrumValue> m_allSignals{nullptr};
        Ptr<const SpectrumValue> m_noise{nullptr};

        Ptr<Dect2020ExtMac> m_mac;
};

} // namespace ns3

#endif // DECT2020EXT_INTERFERENCE_H