#ifndef DECT2020EXT_APP_H
#define DECT2020EXT_APP_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/buffer.h"
#include "ns3/nstime.h"
#include "ns3/dect2020ext-mac.h"

namespace ns3
{

class Dect2020ExtApp : public Object
{
    public:
        Dect2020ExtApp();
        virtual ~Dect2020ExtApp();

        static TypeId GetTypeId();

        void Start();
        void Stop();

        void GeneratePacket();

        // Configuration
        void SetTxPeriod(Time period);
        void SetMac(Ptr<Dect2020ExtMac> mac);

    private:
        // Configuration
        Time m_txPeriod;
        bool m_txPeriodSet = false;
        Ptr<Dect2020ExtMac> m_mac;

        EventId m_event;
};

}; // namespace ns3

#endif // DECT2020EXT_APP_H