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

        // Add payload pointer to the object
        void AddPayload(Header* payload, int payloadSize);

        // Remove the payload pointer from the object
        void RemovePayload();

        // Get payload as a specific type
        template<class T>
        T* GetPayloadAs() const { return dynamic_cast<T*>(m_payload); }

        // Get the payload pointer
        Header* GetPayload() const { return m_payload; }

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
        Header* m_payload; // Pointer to store different types of IEs
        int m_payloadSize;
        uint8_t m_IEType;
};

} // namespace ns3

#endif // DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_H