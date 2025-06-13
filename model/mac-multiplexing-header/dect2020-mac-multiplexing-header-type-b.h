#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H

#include "dect2020-mac-multiplexing-header.h"
#include "ns3/buffer.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeB : public Dect2020MacMultiplexingHeader, public Header
{
    public:
        Dect2020MacMultiplexingHeaderTypeB();
        ~Dect2020MacMultiplexingHeaderTypeB();

        // Enabling 1-byte operations on SDU
        void InsertSDU(const uint8_t payload);
        uint8_t RemoveSDU();
        uint8_t GetSDU() const;

        // Virtual methods of header class
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        Buffer m_buffer; // 1 byte MAC SDU
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_B_H
