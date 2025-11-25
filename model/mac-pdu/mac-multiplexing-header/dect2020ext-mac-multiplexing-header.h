// MAC multiplexing header
// ETSI TS 103 636-4 6.3.4

#ifndef DECT2020EXT_MAC_MULTIPLEXING_HEADER_H
#define DECT2020EXT_MAC_MULTIPLEXING_HEADER_H

#include "ns3/header.h"

namespace ns3
{

// Forward declarations
class Dect2020ExtMacMultiplexingHeaderTypeA;
class Dect2020ExtMacMultiplexingHeaderTypeB;
class Dect2020ExtMacMultiplexingHeaderTypeC;
class Dect2020ExtMacMultiplexingHeader
{
    public:
        Dect2020ExtMacMultiplexingHeader();
        virtual ~Dect2020ExtMacMultiplexingHeader();

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

    public:
        // IEType ETSI TS 103 636-4 6.3.4-3
        enum NoPayloadIEType : uint8_t 
        {
            NoPayloadPaddingIE = 0b00000,
            ConfigurationRequestIE = 0b00001,
            KeepAliveIE = 0b00010,
            // Reserved00011To01111 = 0b00011,
            NoPayloadMacSecurityInfoIE = 0b10000,
            // Reserved10001To11101 = 0b10001,
            NoPayloadEscape = 0b11110,
            // Reserved11111 = 0b11111
        };

        // IEType ETSI TS 103 636-4 6.3.4-2
        enum ExtendedIEType : uint8_t
        {
            EXTENDED_PADDING_IE = 0b000000,
            HIGHER_LAYER_SIGNALLING_FLOW_1 = 0b000001,
            HIGHER_LAYER_SIGNALLING_FLOW_2 = 0b000010,
            USER_PLANE_DATA_FLOW_1 = 0b000011,
            USER_PLANE_DATA_FLOW_2 = 0b000100,
            USER_PLANE_DATA_FLOW_3 = 0b000101,
            USER_PLANE_DATA_FLOW_4 = 0b000110,
            // Reserved = 0b000111,
            NETWORK_BEACON_MESSAGE = 0b001000,
            CLUSTER_BEACON_MESSAGE = 0b001001,
            ASSOCIATION_REQUEST_MESSAGE = 0b001010,
            ASSOCIATION_RESPONSE_MESSAGE = 0b001011,
            ASSOCIATION_RELEASE_MESSAGE = 0b001100,
            RECONFIGURATION_REQUEST = 0b001101,
            RECONFIGURATION_RESPONSE = 0b001110,
            ADDITIONAL_MAC_MESSAGES = 0b001111,
            EXTENDED_MAC_SECURITY_INFO_IE = 0b010000,
            ROUTE_INFO_IE = 0b010001,
            RESOURCE_ALLOCATION_IE = 0b010010,
            RANDOM_ACCESS_RESOURCE_IE = 0b010011,
            RD_CAPABILITY_IE = 0b010100,
            NEIGHBOURING_IE = 0b010101,
            BROADCAST_INDICATION_IE = 0b010110,
            GROUP_ASSIGNMENT_IE = 0b010111,
            LOAD_INFO_IE = 0b011000,
            MEASUREMENT_REPORT_IE = 0b011001,
            //Reserved = 0b011010,
            EXTENDED_ESCAPE = 0b111110,
            IE_TYPE_EXTENSION = 0b111111
        };

        // IEType ETSI TS 103 636-4 6.3.4-4
        enum OneBytePayloadIEType : uint8_t
        {
            OneBytePaddingIE = 0b00000,
            RadioDeviceStatusIE = 0b00001,
            // Reserved00010to11101 = 0b00010,
            OneByteEscape = 0b11110,
            // Reserved11111 = 0b11111
        };
};

} // namespace ns3

#endif // DECT2020EXT_MAC_MULTIPLEXING_HEADER_H 