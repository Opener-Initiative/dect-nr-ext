#ifndef DECT2020EXT_MAC_H
#define DECT2020EXT_MAC_H

#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include <map>
#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtNetDevice;
class Dect2020ExtPhy;
class Dect2020ExtPHYControlFieldType1;

struct Dect2020ExtChannelEvaluation
{
    uint16_t channelId;
    uint32_t free = 0;
    uint32_t possible = 0;
    uint32_t busy = 0;

    uint32_t Total() const
    {
        return free + possible + busy;
    }
};

struct Dect2020ExtSubslotScanContext
{
    uint32_t channelId;
    Dect2020ExtChannelEvaluation evaluation;
    uint32_t subslotCount;
    std::function<void(const Dect2020ExtChannelEvaluation&)> onComplete;
};

class Dect2020ExtMac : public Object
{

    public:
        Dect2020ExtMac();
        virtual ~Dect2020ExtMac();

        static TypeId GetTypeId(void);

        // Initialization

        void InitializeDevice(void);
        void Start(void);
        void InitializeNetwork(void);
        uint32_t GenerateValidNetworkID(void);
        uint32_t GenerateLongRadioDeviceId(void);
        uint16_t GenerateShortRadioDeviceId(void);

        // Spectrum Management Procedure

        void EvaluateAllChannels(void);
        void SelectOperatingChannel(void);
        void ScheduleBackgroundScan(Time delay);
        void OnRssiMeasurement(uint16_t channelId, double rssiDbm);
        void MeasureRSSIOnChannel(Ptr<Dect2020ExtChannel> channel, int64_t frameDurationNs, uint32_t numSubslots);
        void BackgroundScan(uint32_t numSubslots, std::vector<Ptr<Dect2020ExtChannel>> validChannels, uint16_t numOfValidChannels);

        // Beacon Transmission

        void StartBeaconTransmission(void);
        void StartNetworkBeaconTransmission(void);
        void StartClusterBeaconTransmission(void);
        void SendNetworkBeaconOnChannel(uint16_t channelId);
        void SendClusterBeaconOnChannel(uint16_t channelId);
        Ptr<Packet> BuildNetworkBeacon(void);
        Ptr<Packet> BuildClusterBeacon(void);
        Dect2020ExtPHYControlFieldType1 CreatePhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes);

        // Dect2020ExtMac Setters and Getters

        void SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType);
        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType GetTerminationPointType(void) const;
        void SetNetDevice(Ptr<Dect2020ExtNetDevice> device);
        Ptr<Dect2020ExtNetDevice> GetNetDevice(void) const;
        void SetPhy(Ptr<Dect2020ExtPhy> phy);
        Ptr<Dect2020ExtPhy> GetPhy(void) const;
        void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
        void SetNetworkId(uint32_t networkId);
        uint32_t GetNetworkId(void) const;
        void SetLongRadioDeviceId(uint32_t rdId);
        uint32_t GetLongRadioDeviceId(void) const;
        void SetShortRadioDeviceId(uint16_t rdId);
        uint16_t GetShortRadioDeviceId(void) const;
        uint32_t GetOperatingChannelId(void) const;

        // Configuration

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber(void) const;
        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor(void) const;
        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor(void) const;
        void PrintConfiguration(void) const;

        uint16_t m_currentChannelId;

    private:
        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType m_terminationPointType;
        Ptr<Dect2020ExtNetDevice> m_netDevice;
        Ptr<Dect2020ExtPhy> m_phy;
        std::map<uint16_t, std::vector<double>> m_rssiByChannel;
        Ptr<Dect2020ExtChannelManager> m_channelManager;
        uint32_t m_operatingChannelId;
        uint32_t m_networkBeaconChannelId;
        uint32_t m_networkId;
        uint32_t m_longRadioDeviceId;
        uint16_t m_shortRadioDeviceId;

        // Configuration Attributes
        uint8_t m_bandNumber;
        uint8_t m_subcarrierScalingFactor;
        uint8_t m_fourierTransformScalingFactor;

        Time m_networkBeaconPeriodTime = MilliSeconds(100); // Network Beacon Period
        Time m_clusterBeaconPeriodTime = MilliSeconds(100); // Cluster Beacon Period

        uint8_t m_mcs = 0; // Modulation and Coding Scheme

        // std::map<uint32_t, ChannelEvaluation> m_scanEvaluations;

        // ####################
        // ##### Constants ####
        // ####################

        // ETSI TS 103 636-4 V2.1.1 #7.2-1
        const double RSSI_THRESHOLD_MIN = -85; // dBm
        const double RSSI_THRESHOLD_MAX = -50; // dBm TODO: RSSI_THRESHOLDMAX = -52 - max TX Power of RD
        const int SCAN_MEAS_DURATION = 24;     // slots
        const double SCAN_SUITABLE = 0.75;     // Threshold when an operating channel can be considered
                                            // fulfilling the operating conditions.
        const int SCAN_STATUS_VALID = 300;     // seconds
};

} // namespace ns3

#endif /* DECT2020EXT_MAC_H */