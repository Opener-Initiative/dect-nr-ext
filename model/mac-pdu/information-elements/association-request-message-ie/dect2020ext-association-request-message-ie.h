#ifndef DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_H
#define DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_H

#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtNetworkBeaconMessageIE;

typedef Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod NetworkBeaconPeriod;
typedef Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod ClusterBeaconPeriod;

class Dect2020ExtAssociationRequestMessageIE : public Header
{
    public:
        Dect2020ExtAssociationRequestMessageIE();
        virtual ~Dect2020ExtAssociationRequestMessageIE();

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters

        void SetSetupCause(uint8_t setupCause);
        uint8_t GetSetupCause() const;

        void SetNumberOfFlows(uint8_t numberOfFlows);
        uint8_t GetNumberOfFlows() const;

        void SetPowerConstraints(bool powerConstraints);
        bool GetPowerConstraints() const;

        void SetFtMode(bool ftMode);
        bool GetFtMode() const;

        void SetCurrent(bool current);
        bool GetCurrent() const;

        void SetHarqProcessesTx(uint8_t harqProcessesTx);
        uint8_t GetHarqProcessesTx() const;

        void SetMaxHarqReTxDelay(uint8_t maxHarqReTxDelay);
        uint8_t GetMaxHarqReTxDelay() const;

        void SetHarqProcessesRx(uint8_t harqProcessesRx);
        uint8_t GetHarqProcessesRx() const;

        void SetMaxHarqReRxDelay(uint8_t maxHarqReRxDelay);
        uint8_t GetMaxHarqReRxDelay() const;

        void SetFlowId(uint8_t flowId);
        uint8_t GetFlowId() const;

        void SetNetworkBeaconPeriod(NetworkBeaconPeriod networkBeaconPeriod);
        NetworkBeaconPeriod GetNetworkBeaconPeriod() const;

        void SetClusterBeaconPeriod(ClusterBeaconPeriod clusterBeaconPeriod);
        ClusterBeaconPeriod GetClusterBeaconPeriod() const;

        void SetNextClusterChannel(uint16_t nextClusterChannel);
        uint16_t GetNextClusterChannel() const;

        void SetTimeToNext(uint32_t timeToNext);
        uint32_t GetTimeToNext() const;

        void SetCurrentClusterChannel(uint16_t currentClusterChannel);
        uint16_t GetCurrentClusterChannel() const;

    private:
        uint8_t m_setupCause;    // 3 Bit
        uint8_t m_numberOfFlows; // 3 Bit
        bool m_powerConstraints; // 1 Bit
        bool m_ftMode;           // 1 Bit
        bool m_current; // 1 Bit. 0 = current cluster channel is the same as the next cluster channel.
                        //        1 = not the same --> current cluster channel field included
        uint8_t m_harqProcessesTx;                 // 3 Bit
        uint8_t m_maxHarqReTxDelay;                // 5 Bit
        uint8_t m_harqProcessesRx;                 // 3 Bit
        uint8_t m_maxHarqReRxDelay;                // 5 Bit
        uint8_t m_flowId;                          // 6 Bit
        NetworkBeaconPeriod m_networkBeaconPeriod; // 4 Bit
        ClusterBeaconPeriod m_clusterBeaconPeriod; // 4 Bit
        uint16_t m_nextClusterChannel;             // 13 Bit
        uint32_t m_timeToNext;                     // 32 Bit
        uint16_t m_currentClusterChannel;          // 13 Bit
};

} // namespace ns3

#endif // DECT2020EXT_ASSOCIATION_REQUEST_MESSAGE_IE_H 