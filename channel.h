#ifndef DECT_CHANNEL_H
#define DECT_CHANNEL_H

#include "dect-phy.h"

#include "ns3/channel.h"
#include "ns3/net-device.h"

using namespace ns3;

class DectChannel : public Channel
{
    public:
        DectChannel();
        ~DectChannel() override;

        Ptr<NetDevice> GetDevice(std::size_t i) const override;
        std::size_t GetNDevices() const;

        // Adds the given DectPhy to the PHY list
        void Add();

        void Send() const;
        static void Receive();

    private:
        typedef std::vector<Ptr<DectPhy>> PhyList;
        PhyList m_phyList;
};

#endif