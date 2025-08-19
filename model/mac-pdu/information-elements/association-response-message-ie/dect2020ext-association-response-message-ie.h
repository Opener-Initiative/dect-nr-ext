#ifndef DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H
#define DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtAssociationResponseMessageIE : public Header
{
    public:
        Dect2020ExtAssociationResponseMessageIE();
        virtual ~Dect2020ExtAssociationResponseMessageIE();

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

#endif // DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H 