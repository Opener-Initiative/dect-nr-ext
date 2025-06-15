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

        // IEType ETSI TS 103 636-4 6.3.4-3
        enum NoPayloadIEType : uint8_t 
        {
            PaddingIE = 0b00000,
            ConfigurationRequestIE = 0b00001,
            KeepAliveIE = 0b00010,
            // Reserved00011To01111 = 0b00011,
            MacSecurityInfoIE = 0b10000,
            // Reserved10001To11101 = 0b10001,
            Escape = 0b11110,
            // Reserved11111 = 0b11111
        };

        // IEType ETSI TS 103 636-4 6.3.4-2
        enum ExtendedIEType : uint8_t
        {
            PaddingIE = 0b000000,
            HigherLayerSignallingFlow1 = 0b000001,
            HigherLayerSignallingFlow2 = 0b000010,
            UserPlaneDataFlow1 = 0b000011,
            UserPlaneDataFlow2 = 0b000100,
            UserPlaneDataFlow3 = 0b000101,
            UserPlaneDataFlow4 = 0b000110,
            // Reserved = 0b000111,
            NetworkBeaconMessage = 0b001000,
            ClusterBeaconMessage = 0b001001,
            AssociationRequestMessage = 0b001010,
            AssociationResponseMessage = 0b001011,
            AssociationReleaseMessage = 0b001100,
            ReconfigurationRequest = 0b001101,
            ReconfigurationResponse = 0b001110,
            AdditionalMacMessages = 0b001111,
            MacSecurityInfoIE = 0b010000,
            RouteInfoIE = 0b010001,
            ResourceAllocationIE = 0b010010,
            RandomAccessResourceIE = 0b010011,
            RdCapabilityIE = 0b010100,
            NeighbouringIE = 0b010101,
            BroadcastIndicationIE = 0b010110,
            GroupAssignmentIE = 0b010111,
            LoadInfoIE = 0b011000,
            MeasurementReportIE = 0b011001,
            //Reserved = 0b011010,
            Escape = 0b111110,
            IETypeExtension = 0b111111
        };

        // IEType ETSI TS 103 636-4 6.3.4-4
        enum OneBytePayloadIEType : uint8_t
        {
            PaddingIE = 0b00000,
            RadioDeviceStatusIE = 0b00001,
            // Reserved00010to11101 = 0b00010,
            Escape = 0b11110,
            // Reserved11111 = 0b11111
        };
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_H 