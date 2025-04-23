#ifndef DECT2020_MESH_HEADER_H
#define DECT2020_MESH_HEADER_H

#include "ns3/header.h"

namespace ns3
{

    class Dect2020MeshHeader : public Header
    {
        public:
            Dect2020MeshHeader();
            virtual ~Dect2020MeshHeader();
            
            // Setter und Getter
            void SetSinkAddress(uint32_t networkId);
            uint32_t GetSinkAddress() const;

            void SetRouteCost(uint32_t routeCost);
            uint32_t GetRouteCost() const;

            void SetApplicationSequenceNumber(uint32_t applicationSequenceNumber);
            uint32_t GetApplicationSequenceNumber() const;

            // Überladene Methoden von Header
            static TypeId GetTypeId(void);
            virtual TypeId GetInstanceTypeId(void) const;
            virtual void Print(std::ostream& os) const;
            virtual uint32_t GetSerializedSize(void) const;
            virtual void Serialize(Buffer::Iterator start) const;
            virtual uint32_t Deserialize(Buffer::Iterator start);

        private:
            uint32_t m_sinkAddress;
            uint32_t m_routeCost;
            uint32_t m_applicationSequenceNumber;
    };

} // namespace ns3

#endif // DECT2020_MESH_HEADER_H
