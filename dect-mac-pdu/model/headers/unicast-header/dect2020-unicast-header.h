#ifndef DECT2020_UNICAST_HEADER_H
#define DECT2020_UNICAST_HEADER_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020UnicastHeader : public Header
{
    public: 
        Dect2020UnicastHeader();
        ~Dect2020UnicastHeader();

        // Set MAC Sequence
        void SetMacSequence(uint8_t number);
        // Get MAC Sequence
        uint8_t GetMacSequence() const;

        // Set Sequence Number
        void SetSequenceNumber(uint8_t number);
        // Get Sequence Number
        uint8_t GetSequenceNumber() const;

        // Set Receiver Address
        void SetReceiverAddress(uint32_t);
        // Get Receiver Address
        uint32_t GetReceiverAddress() const;

        // Set Transmitter Address
        void SetTransmitterAddress(uint32_t);
        // Get Transmitter Address
        uint32_t GetTransmitterAddress() const;

        // Überladene Methoden von Header
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId(void) const;
        virtual void Print(std::ostream& os) const;
        virtual uint32_t GetSerializedSize(void) const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);

    private:
        uint8_t m_macSequence; // 4 bits
        uint8_t m_sequenceNumber; // 8 bits
        uint32_t m_receiverAddress; // 32 bits
        uint32_t m_transmitterAddress; // 32 bits
};

} // namespace ns3

#endif // DECT2020_UNICAST_HEADER_H