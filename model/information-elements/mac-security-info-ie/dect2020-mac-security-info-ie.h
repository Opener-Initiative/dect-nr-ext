#ifndef DECT2020_MAC_SECURITY_INFO_IE_H
#define DECT2020_MAC_SECURITY_INFO_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020MacSecurityInfoIE : public Header
{
    public:
        Dect2020MacSecurityInfoIE();
        virtual ~Dect2020MacSecurityInfoIE();

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

#endif // DECT2020_MAC_SECURITY_INFO_IE_H 