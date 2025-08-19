#ifndef DECT2020EXT_ROUTE_INFO_IE_H
#define DECT2020EXT_ROUTE_INFO_IE_H

#include "ns3/header.h"

namespace ns3
{

    class Dect2020ExtRouteInfoIE : public Header
    {
        public:
            Dect2020ExtRouteInfoIE();
            virtual ~Dect2020ExtRouteInfoIE();
            
            // Setter und Getter
            void SetSinkAddress(uint32_t networkId);
            uint32_t GetSinkAddress() const;

            void SetRouteCost(uint8_t routeCost);
            uint8_t GetRouteCost() const;

            void SetApplicationSequenceNumber(uint8_t applicationSequenceNumber);
            uint8_t GetApplicationSequenceNumber() const;

            // Überladene Methoden von Header
            static TypeId GetTypeId(void);
            virtual TypeId GetInstanceTypeId(void) const;
            virtual void Print(std::ostream& os) const;
            virtual uint32_t GetSerializedSize(void) const;
            virtual void Serialize(Buffer::Iterator start) const;
            virtual uint32_t Deserialize(Buffer::Iterator start);

        private:
            //  ETSI 103 636-4 
            uint32_t m_sinkAddress; // 32 bits
            uint8_t m_routeCost; // 8 bits
            uint8_t m_applicationSequenceNumber; // 8 bits
    };

} // namespace ns3

#endif // DECT2020EXT_ROUTE_INFO_IE_H
