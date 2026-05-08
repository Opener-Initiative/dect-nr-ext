#ifndef DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H
#define DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H

#include "ns3/dect2020ext-network-beacon-message-ie.h"

#include "ns3/header.h"

namespace ns3
{

// class Dect2020ExtNetworkBeaconPeriod;

class Dect2020ExtClusterBeaconMessageIe : public Header
{

    public:

        Dect2020ExtClusterBeaconMessageIe();
        virtual ~Dect2020ExtClusterBeaconMessageIe();

        // Virtual Header base class methods

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        void Print(std::ostream &os) const override;

        // Setters and Getters

        void SetSFN(uint8_t sfn);
        uint8_t GetSFN() const;

        void SetTxPowerIncluded(bool txPower);
        bool GetTxPowerIncluded() const;

        void SetPowerConstraints(bool powerConstraints);
        bool GetPowerConstraints() const;

        void SetFrameOffsetIncluded(bool frameOffsetIncluded);
        bool GetFrameOffsetIncluded() const;

        void SetNextChannelIncluded(bool nextChannel);
        bool GetNextChannelIncluded() const;

        void SetTimeToNextFieldPresent(bool timeToNextFieldPresent);
        bool GetTimeToNextFieldPresent() const;

        void SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod period);
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod GetNetworkBeaconPeriod() const;

        void SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod period);
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod GetClusterBeaconPeriod() const;

        void SetCountToTrigger(uint8_t count);
        uint8_t GetCountToTrigger() const;

        void SetRelQuality(uint8_t quality);
        uint8_t GetRelQuality() const;

        void SetMinQuality(uint8_t quality);
        uint8_t GetMinQuality() const;

        void SetClusterMaxTXPower(uint8_t power);
        uint8_t GetClusterMaxTXPower() const;

        void SetFrameOffset(uint8_t offset);
        uint8_t GetFrameOffset() const;

        void SetNextClusterChannel(uint16_t channel);
        uint16_t GetNextClusterChannel() const;

        void SetTimeToNext(uint32_t timeToNext);
        uint32_t GetTimeToNext() const;

    private:
        uint8_t m_SFN;                                                                           // 8 bits
        bool m_txPowerIncluded;                                                                  // 1 bit
        bool m_powerConstraints;                                                                 // 1 bit
        bool m_frameOffsetIncluded;                                                              // 1 bit
        bool m_nextChannelIncluded;                                                              // 1 bit
        bool m_timeToNextFieldPresent;                                                           // 1 bit
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod m_networkBeaconPeriod; // 4 bits
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod m_clusterBeaconPeriod; // 4 bits
        uint8_t m_countToTrigger;                                                                // 4 bits
        uint8_t m_relQuality;                                                                    // 2 bits
        uint8_t m_minQuality;                                                                    // 2 bits
        uint8_t m_clusterMaxTxPower;                                                             // 4 bits
        uint8_t m_frameOffset;                                                                   // 1-byte Frame Offset (if subcarrierscaling factor = <= 4 --> frameOffset 8 bit, otherwise 16 bit)
        uint16_t m_nextClusterChannel;                                                           // 13 bits
        uint32_t m_timeToNext;                                                                   // 32 bits
};

} // namespace ns3

#endif // DECT2020EXT_CLUSTER_BEACON_MESSAGE_IE_H 