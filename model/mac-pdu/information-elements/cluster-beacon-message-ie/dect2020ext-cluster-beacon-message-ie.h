// DECT 2020 Cluster Beacon Message
// ETSI TS 103 636-4 6.4.2.3

#ifndef DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H
#define DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H

#include "ns3/dect2020ext-network-beacon-message-ie.h"

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtNetworkBeaconPeriod;

class Dect2020ExtClusterBeaconMessageIE : public Header
{

    public:

        Dect2020ExtClusterBeaconMessageIE();
        virtual ~Dect2020ExtClusterBeaconMessageIE();

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

        // TxPower
        void SetTxPowerIncluded(bool txPower);
        bool GetTxPowerIncluded() const;

        // PowerConst
        void SetPowerConstraints(bool powerConstraints);
        bool GetPowerConstraints() const;

        // FO
        void SetFrameOffsetIncluded(bool frameOffsetIncluded);
        bool GetFrameOffsetIncluded() const;

        // NextChannel
        void SetNextChannelIncluded(bool nextChannel);
        bool GetNextChannelIncluded() const;

        // TimeToNext (1-bit version)
        void SetTimeToNextFieldPresent(bool timeToNextFieldPresent);
        bool GetTimeToNextFieldPresent() const;

        // NetworkBeaconPeriod
        void SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod period);
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod GetNetworkBeaconPeriod() const;

        // ClusterBeaconPeriod
        void SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod period);
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod GetClusterBeaconPeriod() const;

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
        bool m_txPowerIncluded; // 1 relevant bit;
        bool m_powerConstraints; // 1 relevant bit;
        bool m_frameOffsetIncluded; // 1 relevant bit
        bool m_nextChannelIncluded; // 1 relevant bit;
        bool m_timeToNextFieldPresent; // 1 relevant bit;
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod m_networkBeaconPeriod; // 4 relevant bits;
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod m_clusterBeaconPeriod; // 4 relevant bits;
        uint8_t m_countToTrigger; // 4 relevant bits;
        uint8_t m_relQuality; // 2 relevant bits;
        uint8_t m_minQuality; // 2 relevant bits;
        uint8_t m_clusterMaxTXPower; // 4 relevant bits
        uint8_t m_frameOffset; // 1-byte Frame Offset (if subcarrierscaling factor = <= 4 --> frameOffset 8 bit, otherwise 16 bit)
        uint16_t m_nextClusterChannel; // 13 relevant bits
        uint32_t m_timeToNext; // 4-byte Time To Next
};

} // namespace ns3

#endif // DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H 