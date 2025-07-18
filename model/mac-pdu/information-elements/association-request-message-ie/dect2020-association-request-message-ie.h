#ifndef DECT2020_ASSOCIATION_REQUEST_MESSAGE_IE_H
#define DECT2020_ASSOCIATION_REQUEST_MESSAGE_IE_H

#include "ns3/dect2020-network-beacon-message-ie.h"
#include "ns3/header.h"

namespace ns3
{

class Dect2020NetworkBeaconMessageIE;

typedef Dect2020NetworkBeaconMessageIE::NetworkBeaconPeriod NetworkBeaconPeriod;
typedef Dect2020NetworkBeaconMessageIE::ClusterBeaconPeriod ClusterBeaconPeriod;

class Dect2020AssociationRequestMessageIE : public Header
{
    public:
        Dect2020AssociationRequestMessageIE();
        virtual ~Dect2020AssociationRequestMessageIE();

        // ETSI TS 103 636-4 6.4.2.4-2
        enum SetupCause
        {
            InitialAssociation = 0,
            AssociationToRequestNewSetOfFlows = 1,
            AssociationDueToMobility = 2,
            ReassociationAfterError = 3,
            ChangeOfOwnOperatingChannel = 4,
            ChangeOperatingMode = 5,
            PagingResponse = 6
        };

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters

    private:
        SetupCause m_setupCause; // 3 bits relevant
        uint8_t m_numberOfFlows; // 3 bits relevant
        uint8_t m_powerConst; // 1 bit relevant
        bool m_ftMode; // 1 bit relevant
        bool m_current; // 1 bit relevant
        uint8_t m_harqProcessesTX; // 3 bits relevant
        uint8_t m_maxHarqReTX; // 5 bits relevant
        uint8_t m_harqProcessesRX; // 3 bits relevant
        uint8_t m_maxHarqReRX; // 5 bits relevant
        uint8_t m_flowID; // 6 bits relevant
        NetworkBeaconPeriod m_networkBeaconPeriod; // 4 bits relevant
        ClusterBeaconPeriod m_clusterBeaconPeriod; // 4 bits relevant
        uint16_t m_nextClusterChannel; // 13 bits relevant
        uint32_t m_timeToNext;
        uint16_t m_currentClusterChannel; // 13 bits relevant
};

} // namespace ns3

#endif // DECT2020_ASSOCIATION_REQUEST_MESSAGE_IE_H 