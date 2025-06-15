#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H

#include "ns3/dect2020-mac-multiplexing-header.h"
#include "ns3/buffer.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeC : public Dect2020MacMultiplexingHeader, public Header
{
    public:
        Dect2020MacMultiplexingHeaderTypeC();
        ~Dect2020MacMultiplexingHeaderTypeC();

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
        Buffer m_buffer; // variable length MAC SDU
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H