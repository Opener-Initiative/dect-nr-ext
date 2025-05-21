#ifndef RESOURCE_ALLOCATION_IE_H
#define RESOURCE_ALLOCATION_IE_H

#include "ns3/header.h"

namespace ns3
{

class ResourceAllocationIE : public Header
{
    public:
        ResourceAllocationIE();
        ~ResourceAllocationIE() override;

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

#endif // RESOURCE_ALLOCATION_IE_H 