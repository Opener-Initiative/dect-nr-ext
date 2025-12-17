#ifndef DECT2020EXT_MAC_H
#define DECT2020EXT_MAC_H

#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-unicast-header.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-association-request-message-ie.h"
#include "ns3/dect2020ext-association-response-message-ie.h"

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/node.h"

#include <map>
#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtNetDevice;
class Dect2020ExtPhy;
class Dect2020ExtPHYControlFieldType1;
class Dect2020ExtRandomAccessResourceIE;

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
    uint32_t totalSubslots;
    // std::function<void(const Dect2020ExtChannelEvaluation&)> onComplete;
};

struct Dect2020ExtFtCandidateInfo
{
    // uint8_t shortNetworkId; // Short Network ID of the FT candidate
    uint32_t networkId;     // Network ID of the FT candidate
    // uint32_t shortFtId;     // Short RD ID of the FT candidate
    uint32_t longFtId;      // Long RD ID of the FT candidate

    uint16_t clusterChannelId;
    uint8_t sfn;
    Time receptionTime;
    double rssiDbm;

    Dect2020ExtPHYControlFieldType1 ftPhyHeaderField;       // Last received Physical Header Field
    Dect2020ExtBeaconHeader ftBeaconHeader;                 // Last received Beacon Header
    // Dect2020ExtUnicastHeader ftUnicastHeader;               // Last received Unicast Header
    Dect2020ExtNetworkBeaconMessageIE ftNetworkBeaconMessage; // Last received Network Beacon Message
    Dect2020ExtClusterBeaconMessageIE ftClusterBeaconMessage; // Last received Cluster Beacon Message
    Dect2020ExtRandomAccessResourceIE ftRandomAccessResourceIE; // Last received Random Access Resource IE

    // Constructor to initialize all members to zero/default values
    Dect2020ExtFtCandidateInfo()
        : networkId(0),
          longFtId(0),
          clusterChannelId(0),
          receptionTime(Seconds(0)),
          rssiDbm(-150.0), 
          ftPhyHeaderField(),
          ftBeaconHeader(),
          ftNetworkBeaconMessage(),
          ftClusterBeaconMessage(),
          ftRandomAccessResourceIE()
    {
    }
};

struct Dect2020ExtAssociatedPtInfo
{
    Time associationEstablished;
    uint32_t longRdId; // Long Radio Device ID of the PT
    // uint8_t numberOfFlows;
    // bool powerConstraints;
    // bool ftMode;
    // uint8_t harqProcessesTx;
    // uint8_t maxHarqReTxDelay;
    // uint8_t harqProcessesRx;
    // uint8_t maxHarqReRxDelay;
    // uint8_t flowId;
    // bool cb_m;
    // Dect2020RdCapabilityIE rdCapabilityIe;

    // double lastRssi;
};

class Dect2020ExtMac : public Object
{

    public:
        Dect2020ExtMac();
        virtual ~Dect2020ExtMac();

        static TypeId GetTypeId(void);

        enum Dect2020ExtAssociationStatus
        {
            NOT_ASSOCIATED,
            ASSOCIATION_PREPARING,
            WAITING_FOR_SELECTED_FT,
            ASSOCIATION_PENDING,
            ASSOCIATED
        };

        // Initialization

        void InitializeDevice(void);
        void Start(void);
        void DiscoverNetworks(void);
        void InitializeNetwork(void);
        uint32_t GenerateValidNetworkID(void);
        uint32_t GenerateLongRadioDeviceId(void);
        uint16_t GenerateShortRadioDeviceId(void);

        // Spectrum Management Procedure

        void EvaluateAllChannels(void);
        SubslotStatus EvaluateSubslot(double rssiDbm);
        void DisplayChannelEvaluations(void);
        void SelectOperatingChannel(void);
        void ScheduleBackgroundScan(Time delay);
        void OnRssiMeasurement(uint16_t channelId, double rssiDbm, std::shared_ptr<Dect2020ExtSubslotScanContext> context);
        void MeasureRSSIOnChannel(Ptr<Dect2020ExtChannel> channel, int64_t frameDurationNs, uint32_t numSubslots, std::shared_ptr<Dect2020ExtSubslotScanContext> context);
        void BackgroundScan(void);

        // Beacon Transmission

        void StartBeaconTransmission(void);
        void StartNetworkBeaconTransmission(void);
        void StartClusterBeaconTransmission(void);
        void SendNetworkBeaconOnChannel(uint16_t channelId);
        void SendClusterBeaconOnChannel(uint16_t channelId);
        Ptr<Packet> BuildNetworkBeacon(void);
        Ptr<Packet> BuildClusterBeacon(uint32_t clusterChannelId);
        Dect2020ExtRandomAccessResourceIE BuildRandomAccessResourceIE(void);
        uint8_t CalculateAbsoluteStartSubslot(void);
        Dect2020ExtPHYControlFieldType1 BuildPhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes);

        // Association Procedure

        void ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm);
        void HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtCandidateInfo* ft);
        void HandleUnicastPacket(Ptr<Packet> packet);
        Dect2020ExtFtCandidateInfo* FindOrCreateFtCandidate(uint16_t shortFtId);
        void EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIE& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIE& randomAccessResourceIE, Dect2020ExtFtCandidateInfo* ft);
        void SelectBestFtCandidate(void);
        void VerifyWaitingForSelectedFtAssociationStatus(void);
        void SendAssociationRequest(Dect2020ExtFtCandidateInfo* ft);
        void VerifyPendingAssociationStatus(void);
        void ProcessAssociationRequest(const Dect2020ExtAssociationRequestMessageIE& associationRequestMessage, uint32_t transmitterAddress);
        void SendAssociationResponse(Dect2020ExtAssociatedPtInfo ptInfo);
        void ProcessAssociationResponse(Dect2020ExtAssociationResponseMessageIE associationResponseMessage, uint32_t transmitterAddress);
        void VerifyAssociatedStatus(void);
        Dect2020ExtFtCandidateInfo* FindFtCandidateByLongId(uint32_t longFtId);

        // Dect2020ExtMac Setters and Getters

        void SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType);
        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType GetTerminationPointType(void) const;
        void SetNetDevice(Ptr<Dect2020ExtNetDevice> device);
        Ptr<Dect2020ExtNetDevice> GetNetDevice(void) const;
        Ptr<Node> GetNode(void) const;
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
        std::vector<Dect2020ExtChannelEvaluation> GetChannelEvaluations(void) const;
        void SetCurrentChannelId(uint16_t channelId);
        uint16_t GetCurrentChannelId(void) const;
        double GetRxGainFromIndex(uint8_t index) const;

        // Configuration

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber(void) const;
        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor(void) const;
        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor(void) const;
        void PrintConfiguration(void) const;

        uint8_t m_nextAvailableSubslot = 2;

        bool m_dectDelay = false;

        uint16_t m_currentChannelId;

        uint32_t m_associatedFTNetDeviceLongRdId = 0;
    private:
        std::vector<Dect2020ExtChannelEvaluation> m_channelEvaluations;
        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType m_terminationPointType;
        Ptr<Dect2020ExtNetDevice> m_device;
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

        uint32_t m_networkBeaconSweepCount;
        Time m_nextNetworkBeaconTime;

        std::vector<Dect2020ExtFtCandidateInfo> m_ftCandidates;

        Dect2020ExtMacMultiplexingHeaderTypeA m_lastFtMacMxHeader;

        Dect2020ExtAssociationStatus m_associationStatus;
        bool m_isWaitingForClusterBeacon;

        uint8_t m_lastSfn;
        Dect2020ExtFtCandidateInfo m_selectedFtCandidate;

        uint8_t m_potentialShortNetworkId;

        std::vector<Dect2020ExtAssociatedPtInfo> m_associatedPtDevices;

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