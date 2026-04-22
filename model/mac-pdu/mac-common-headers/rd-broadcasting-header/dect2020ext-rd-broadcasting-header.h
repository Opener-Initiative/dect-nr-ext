#ifndef DECT2020EXT_RD_BROADCASTING_HEADER_H
#define DECT2020EXT_RD_BROADCASTING_HEADER_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtRdBroadcastingHeader : public Header
{
    public:
        Dect2020ExtRdBroadcastingHeader();
        ~Dect2020ExtRdBroadcastingHeader() override;

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

        void SetTransmitterAddress(uint32_t transmitterAddress);
        uint32_t GetTransmitterAddress() const;

    private:
        uint8_t m_reset; // 1 relevant bit
        uint16_t m_sequenceNumber; // 12 relevant bits
        uint32_t m_transmitterAddress; // all relevant bits
};

} // namespace ns3

#endif // DECT2020EXT_RD_BROADCASTING_HEADER_H_H