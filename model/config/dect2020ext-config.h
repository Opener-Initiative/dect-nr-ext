#ifndef DECT2020EXT_CONFIG_H
#define DECT2020EXT_CONFIG_H

#include "dect2020ext-config-values.h"

#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/ptr.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/isotropic-antenna-model.h"

namespace ns3
{

class Dect2020ExtConfig
{
    public:    
        Dect2020ExtConfig();
        ~Dect2020ExtConfig();

        Ptr<SPECTRUM_CHANNEL_TYPE> CreateConfiguredSpectrumChannel();
        Ptr<Dect2020ExtChannelManager> CreateConfiguredChannelManager();
        Ptr<IsotropicAntennaModel> CreateConfiguredIsotropicAntennaModel();
        void ApplyToPhy(Ptr<Dect2020ExtPhy> phy);
        void ApplyToMac(Ptr<Dect2020ExtMac> mac);

    private:
        Ptr<SpectrumChannel> m_spectrumChannel;
        Ptr<Dect2020ExtChannelManager> m_channelManager;
        Ptr<IsotropicAntennaModel> m_antenna;
};

} // namespace ns3

#endif