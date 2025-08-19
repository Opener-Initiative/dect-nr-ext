#ifndef DECT2020EXT_GROUP_ASSIGNMENT_IE_H
#define DECT2020EXT_GROUP_ASSIGNMENT_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtGroupAssignmentIE : public Header
{
    public:
        Dect2020ExtGroupAssignmentIE();
        virtual ~Dect2020ExtGroupAssignmentIE();

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

#endif // DECT2020EXT_GROUP_ASSIGNMENT_IE_H 