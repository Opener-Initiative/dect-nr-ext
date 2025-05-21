#ifndef NEIGHBOURING_IE_H
#define NEIGHBOURING_IE_H

#include "ns3/header.h"

namespace ns3
{

class NeighbouringIE : public Header
{
    public:
        NeighbouringIE();
        ~NeighbouringIE() override;

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

#endif // NEIGHBOURING_IE_H 