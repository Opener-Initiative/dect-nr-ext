#ifndef CLUSTER_BEACON_MESSAGE_H
#define CLUSTER_BEACON_MESSAGE_H

#include "ns3/header.h"

namespace ns3
{

class ClusterBeaconMessage : public Header
{
    public:
        ClusterBeaconMessage();
        ~ClusterBeaconMessage() override;

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters


    private:
        
};

} // namespace ns3

#endif // CLUSTER_BEACON_MESSAGE_H 