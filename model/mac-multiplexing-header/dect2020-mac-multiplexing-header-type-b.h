#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H

#include "dect2020-mac-multiplexing-header.h"
#include "ns3/header.h"
#include "ns3/byte-tag-list.h"
namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeB : public Dect2020MacMultiplexingHeader, public Header
{
    public:
        // Enabling operations on SDU
        void InsertSDU(const Header& header);
        uint32_t RemoveSDU(Header& header);

        // Virtual methods of header class
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        Buffer m_SDU; // 1 byte MAC SDU
        uint8_t m_macExtension; // 2 relevant bits
        uint8_t m_length; // 1 relevant bit
        uint8_t m_IEType; // 5 relevant bits
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H
