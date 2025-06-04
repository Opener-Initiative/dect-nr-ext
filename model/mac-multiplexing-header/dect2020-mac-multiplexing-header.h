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
        virtual ~Dect2020MacMultiplexingHeader();

        // Type of the MAC multiplexing header
        enum Type
        {
            A = 1, // Short SDU, no payload, 1 byte long
            B = 2, // Short SDU, 1 byte payload, 2 bytes long
            C, // without length indication
            D, // Medium SDU
            E, // Large SDU
            F  // Medium or Large SDU with IE type extension
        };

        // IE type

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters
        void SetType(Type type);
        Type GetType() const;
        
        void SetMacExtension(uint8_t macExtension);
        uint8_t GetMacExtension() const;
        
        void SetLength(uint8_t length);
        uint8_t GetLength() const;
        
        void SetIEType(uint8_t ieType);
        uint8_t GetIEType() const;

    private:
        Type m_type;
        uint8_t m_macExtension; // 2 relevant bits
        uint8_t m_length; // 1 relevant bit
        uint8_t m_IEType; // 5 relevant bits
        // TODO: Add necessary variables and complete the class
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_H 