#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H

#include "ns3/dect2020-mac-multiplexing-header.h"
#include "ns3/buffer.h"
#include "ns3/header.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeC : public Dect2020MacMultiplexingHeader, public Header
{
    public:
        Dect2020MacMultiplexingHeaderTypeC();
        ~Dect2020MacMultiplexingHeaderTypeC();

        // Enabling operations on SDU

        void InsertSDU(const Header &header); // should be like Packet::AddHeader
        uint8_t RemoveSDU(); // should be like Packet::RemoveHeader
        uint8_t GetSDU() const; // should be like Packet::PeekHeader

        // Virtual methods of header class
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        Buffer m_buffer; // variable length MAC SDU
        enum IEType : uint8_t
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
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H