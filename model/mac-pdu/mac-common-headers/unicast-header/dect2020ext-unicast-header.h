#ifndef DECT2020EXT_UNICAST_HEADER_H
#define DECT2020EXT_UNICAST_HEADER_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtUnicastHeader : public Header
{
    public: 
        Dect2020ExtUnicastHeader();
        virtual ~Dect2020ExtUnicastHeader();

        // Overloaded methods from Header base class

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        void Print(std::ostream& os) const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;

        // Setters and Getters

        void SetReset(bool reset);
        bool GetReset() const;

        void SetMacSequence(uint8_t number);
        uint8_t GetMacSequence() const;

        void SetSequenceNumber(uint8_t number);
        uint8_t GetSequenceNumber() const;

        void SetReceiverAddress(uint32_t);
        uint32_t GetReceiverAddress() const;

        void SetTransmitterAddress(uint32_t);
        uint32_t GetTransmitterAddress() const;

    private:
        bool m_reset; // 1 bit
        uint8_t m_macSequence; // 4 bits
        uint8_t m_sequenceNumber; // 8 bits
        uint32_t m_receiverAddress; // 32 bits
        uint32_t m_transmitterAddress; // 32 bits
};

} // namespace ns3

#endif // DECT2020EXT_UNICAST_HEADER_H