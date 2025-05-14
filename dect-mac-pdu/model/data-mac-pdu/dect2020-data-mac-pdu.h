#ifndef DECT2020_DATA_MAC_PDU_H
#define DECT2020_DATA_MAC_PDU_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020DataMacPdu : public Header
{
    public:
        Dect2020DataMacPdu();
        virtual ~Dect2020DataMacPdu();

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters
        void SetReset(uint8_t value);
        uint8_t GetReset() const;
        void SetSequenceNumber(uint16_t sequenceNumber);
        uint16_t GetSequenceNumber() const;
        
    private:
        uint8_t m_reset;
        uint16_t m_sequenceNumber; // 12 relevant bits
};

} // namespace ns3

#endif // DECT2020_DATA_MAC_PDU_H 