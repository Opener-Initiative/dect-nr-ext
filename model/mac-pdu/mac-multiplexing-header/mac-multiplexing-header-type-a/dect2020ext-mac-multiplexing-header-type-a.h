#ifndef DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H
#define DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H

#include "ns3/dect2020ext-mac-multiplexing-header.h"

namespace ns3
{

class Dect2020ExtMacMultiplexingHeaderTypeA : public Dect2020ExtMacMultiplexingHeader, public Header
{

    public:
        Dect2020ExtMacMultiplexingHeaderTypeA();
        ~Dect2020ExtMacMultiplexingHeaderTypeA();

        void SetIEType(NoPayloadIEType ieType);
        uint8_t GetIEType() const;
    
        // Virtual methods of header class

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        uint8_t m_IEType;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H