#ifndef DECT2020EXT_DATA_MAC_PDU_H
#define DECT2020EXT_DATA_MAC_PDU_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtDataMacPduHeader : public Header
{
    public:
        Dect2020ExtDataMacPduHeader();
        virtual ~Dect2020ExtDataMacPduHeader();

        // Overloaded methods from Header base class

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        void Print(std::ostream &os) const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;

        // Setters and Getters
        void SetReset(bool value);
        bool GetReset() const;

        void SetSequenceNumber(uint16_t sequenceNumber);
        uint16_t GetSequenceNumber() const;
        
    private:
        bool m_reset; // 1 relevant bit
        uint16_t m_sequenceNumber; // 12 relevant bits
};

} // namespace ns3

#endif // DECT2020EXT_DATA_MAC_PDU_H 