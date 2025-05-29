// MAC multiplexing header
// ETSI TS 103 636-4 6.3.4

#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020MacMultiplexingHeader : public Header
{
    public:
        Dect2020MacMultiplexingHeader();
        ~Dect2020MacMultiplexingHeader() override;

        // Type of the MAC multiplexing header
        enum Type
        {
            A, // Short SDU, no payload
            B, // Short SDU, 1 byte payload
            C, // without length indication
            D, // Medium SDU
            E, // Large SDU
            F  // Medium or Large SDU with IE type extension
        };

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters

    private:
        Type m_type;
        uint8_t m_macExtension; // 2 relevant bits
        uint8_t m_length; // 1 relevant bit
        uint8_t m_IEType; // 5 relevant bits
        uint8_t m_1ByteMacSdu; // 1-byte MAC SDU
        uint8_t m_fixedSizeMacSdu; // 1-byte fixed size MAC SDU, size defined by type
        // TODO: Add necessary variables and complete the class
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_H 