#ifndef DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H
#define DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtMacMultiplexingHeaderTypeA : public Header
{
    public:

    enum Dect2020ExtIETypeFieldEncoding
    {
        // # ETSI TS 103 636-4 V2.1.1 Table 6.3.4.2
        // Rest to be implemented
        NETWORK_BEACON_MESSAGE = 8,
        CLUSTER_BEACON_MESSAGE = 9,
        ASSOCIATION_REQUEST_MESSAGE = 10,
        ASSOCIATION_RESPONSE_MESSAGE = 11,
        RANDOM_ACCESS_RESOURCE_IE = 19
    };

        Dect2020ExtMacMultiplexingHeaderTypeA();
        virtual ~Dect2020ExtMacMultiplexingHeaderTypeA();

        // Setters and Getters

        uint8_t GetMacExtensionFieldEncoding() const;
        void SetMacExtensionFieldEncoding(uint8_t encoding);

        uint8_t GetLengthField() const;
        void SetLengthField(uint8_t length);

        Dect2020ExtIETypeFieldEncoding GetIeTypeFieldEncoding() const;
        void SetIeTypeFieldEncoding(Dect2020ExtIETypeFieldEncoding type);

        // Virtual methods of header class

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const;
        virtual uint32_t GetSerializedSize() const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;

    private:
        uint8_t m_macExtensionFieldEncoding; // 2 bit
        uint8_t m_length; // 1 bit
        uint8_t m_ieTypeFieldEncoding; // 5 bit
};

} // namespace ns3

#endif // DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H