// DECT 2020 Cluster Beacon Message
// ETSI TS 103 636-4 6.4.2.3

#ifndef DECT2020_CLUSTER_BEACON_MESSAGE_H
#define DECT2020_CLUSTER_BEACON_MESSAGE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ClusterBeaconMessage : public Header
{
    public:
        Dect2020ClusterBeaconMessage();
        virtual ~Dect2020ClusterBeaconMessage();

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

        // Setters and Getters

        // SFN
        void SetSFN(uint8_t sfn);
        uint8_t GetSFN() const;

        // TXPower
        void SetTXPower(uint8_t txPower);
        uint8_t GetTXPower() const;

        // PowerConst
        void SetPowerConst(uint8_t powerConst);
        uint8_t GetPowerConst() const;

        // FO
        void SetFO(uint8_t fo);
        uint8_t GetFO() const;

        // NextChannel
        void SetNextChannel(uint8_t nextChannel);
        uint8_t GetNextChannel() const;

        // TimeToNext (1-bit version)
        void SetTimeToNextBit(uint8_t timeToNext);
        uint8_t GetTimeToNextBit() const;

        // NetworkBeaconPeriod
        void SetNetworkBeaconPeriod(uint8_t period);
        uint8_t GetNetworkBeaconPeriod() const;

        // ClusterBeaconPeriod
        void SetClusterBeaconPeriod(uint8_t period);
        uint8_t GetClusterBeaconPeriod() const;

        // CountToTrigger
        void SetCountToTrigger(uint8_t count);
        uint8_t GetCountToTrigger() const;

        // RelQuality
        void SetRelQuality(uint8_t quality);
        uint8_t GetRelQuality() const;

        // MinQuality
        void SetMinQuality(uint8_t quality);
        uint8_t GetMinQuality() const;

        // ClusterMaxTXPower
        void SetClusterMaxTXPower(uint8_t power);
        uint8_t GetClusterMaxTXPower() const;

        // FrameOffset
        void SetFrameOffset(uint8_t offset);
        uint8_t GetFrameOffset() const;

        // NextClusterChannel
        void SetNextClusterChannel(uint16_t channel);
        uint16_t GetNextClusterChannel() const;

        // TimeToNext (4-byte version)
        void SetTimeToNext(uint32_t timeToNext);
        uint32_t GetTimeToNext() const;

    private:
        uint8_t m_SFN; // 1-byte SFN
        uint8_t m_powerConst; // 1 relevant bit;
        uint8_t m_FO; // 1 relevant bit
        uint8_t m_nextChannel; // 1 relevant bit;
        uint8_t m_timeToNext; // 1 relevant bit;
        uint8_t m_networkBeaconPeriod; // 4 relevant bits;
        uint8_t m_clusterBeaconPeriod; // 4 relevant bits;
        uint8_t m_countToTrigger; // 4 relevant bits;
        uint8_t m_relQuality; // 2 relevant bits;
        uint8_t m_minQuality; // 2 relevant bits;
        uint8_t m_clusterMaxTXPower; // 4 relevant bits
        uint8_t m_frameOffset; // 1-byte Frame Offset
        uint16_t m_nextClusterChannel; // 13 relevant bits
        uint32_t m_timeToNext; // 4-byte Time To Next
};

} // namespace ns3

#endif // DECT2020_CLUSTER_BEACON_MESSAGE_H 