#ifndef DECT_PHY_H
#define DECT_PHY_H

#include "ns3/yans-wifi-channel.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-net-device.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/wifi-helper.h"

class DectPhy : public ns3::WifiPhyHelper
{
    public:
        DectPhy();

        void SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel);

    private:
        //std::vector<ns3::Ptr<ns3::WifiPhy>> Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const override;
        ns3::Ptr<ns3::YansWifiChannel> m_channel;
};

#endif //DECT_PHY_H