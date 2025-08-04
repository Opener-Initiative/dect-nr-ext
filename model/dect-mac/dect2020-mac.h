#ifndef DECT2020_MAC_H
#define DECT2020_MAC_H

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/dect2020-net-device.h"

namespace ns3
{

class Dect2020NetDevice;

class Dect2020Mac : public Object
{
    public:
        Dect2020Mac();
        ~Dect2020Mac();

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const;

        void ProcessPacket(Ptr<Packet> packet);
        void StartNetworkBeaconTransmission();
        void StopNetworkBeaconTransmission();
        void StartClusterBeaconTransmission();
        void StopClusterBeaconTransmission();

        // Setters and Getters

        void SetNetDevice(Ptr<Dect2020NetDevice> netDevice);
        Ptr<Dect2020NetDevice> GetNetDevice() const;

    private:
        Ptr<Dect2020NetDevice> m_netDevice;
};

} // namespace ns3

#endif // DECT2020_MAC_H