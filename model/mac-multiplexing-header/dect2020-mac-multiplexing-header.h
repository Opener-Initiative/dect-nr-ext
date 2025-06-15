// MAC multiplexing header
// ETSI TS 103 636-4 6.3.4

#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_H

#include "ns3/header.h"

namespace ns3
{

// Forward declarations
class Dect2020MacMultiplexingHeaderTypeA;
class Dect2020MacMultiplexingHeaderTypeB;
class Dect2020MacMultiplexingHeaderTypeC;
class Dect2020MacMultiplexingHeader
{
    public:
        Dect2020MacMultiplexingHeader();
        virtual ~Dect2020MacMultiplexingHeader();

        // Type of the MAC multiplexing header
        enum Type
        {
            A = 1, // Short SDU, no payload, 1 byte long
            B = 2, // Short SDU, 1 byte payload, 2 bytes long
            C = 3, // without length indication
            D = 4, // Medium SDU
            E = 5, // Large SDU
            F = 6 // Medium or Large SDU with IE type extension
        };

        // Setters and Getters

        void SetType(Type type);
        Type GetType() const;
        
        void SetMacExtension(uint8_t macExtension);
        uint8_t GetMacExtension() const;
        
        void SetLength(uint8_t length);
        uint8_t GetLength() const;

    protected:
        Type m_type;
        uint8_t m_macExtension; // 2 relevant bits
        uint8_t m_length; // 1 relevant bit
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_H 