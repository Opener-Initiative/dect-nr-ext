#ifndef ASSOCIATION_RESPONSE_MESSAGE_H
#define ASSOCIATION_RESPONSE_MESSAGE_H

#include "ns3/header.h"

namespace ns3
{

class AssociationResponseMessage : public Header
{
    public:
        AssociationResponseMessage();
        ~AssociationResponseMessage() override;

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

#endif // ASSOCIATION_RESPONSE_MESSAGE_H 