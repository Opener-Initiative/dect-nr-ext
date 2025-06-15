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
        uint8_t RemoveSDU(Header &header); // should be like Packet::RemoveHeader
        uint8_t GetSDU() const; // should be like Packet::PeekHeader

        // IEType setter and getter

        void SetIEType(ExtendedIEType IEType);
        uint8_t GetIEType() const;

        // Virtual methods of header class
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        Buffer m_buffer; // variable length MAC SDU
        ExtendedIEType m_IEType;
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H