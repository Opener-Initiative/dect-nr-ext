#ifndef DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H
#define DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIE : public Header
{
    public:
        Dect2020ExtRandomAccessResourceIE();
        virtual ~Dect2020ExtRandomAccessResourceIE();

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

#endif // DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H 