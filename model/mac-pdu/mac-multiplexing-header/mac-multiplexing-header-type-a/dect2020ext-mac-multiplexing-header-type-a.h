#ifndef DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H
#define DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtMacMultiplexingHeaderTypeA : public Header
{
    public:
        enum IeTypeFieldEncoding
        {
            NETWORK_BEACON_MESSAGE = 8,
            CLUSTER_BEACON_MESSAGE = 9,
            ASSOCIATION_REQUEST_MESSAGE = 10,
            ASSOCIATION_RESPONSE_MESSAGE = 11,
            RANDOM_ACCESS_RESOURCE_IE = 19
        };

        Dect2020ExtMacMultiplexingHeaderTypeA();
        virtual ~Dect2020ExtMacMultiplexingHeaderTypeA();

        // Virtual methods of Header class

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        void Print(std::ostream &os) const override;

        // Setters and Getters

        void SetMacExtensionFieldEncoding(uint8_t encoding);
        uint8_t GetMacExtensionFieldEncoding() const;

        void SetLengthField(bool length);
        bool GetLengthField() const;

        IeTypeFieldEncoding GetIeTypeFieldEncoding() const;
        void SetIeTypeFieldEncoding(IeTypeFieldEncoding type);

    private:
        uint8_t m_macExtensionFieldEncoding; // 2 bits
        bool m_length;                       // 1 bit
        IeTypeFieldEncoding m_ieTypeFieldEncoding;       // 5 bits
};

} // namespace ns3

#endif // DECT2020EXT_MAC_MULTIPLEXING_HEADER_TYPE_A_H