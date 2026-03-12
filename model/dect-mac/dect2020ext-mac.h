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
#include "ns3/dect2020ext-rd-capability-ie.h"
#include "ns3/dect2020ext-association-control-ie.h"
#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-app.h"
#include "ns3/dect2020ext-phy-control-field-type-2.h"

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/node.h"

#include <map>
#include <memory>
#include <vector>
#include <queue>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtNetDevice;
class Dect2020ExtPhy;
class Dect2020ExtPHYControlFieldType1;
class Dect2020ExtRandomAccessResourceIE;
class Dect2020ExtRdCapabilityIE;
class Dect2020ExtMacScheduler;
class Dect2020ExtApp;

struct Dect2020ExtHarqProcess
{
    enum State
    {
        FREE, // prces is available for a new packet
        WAITING_ACK, // packet sent, waiting for feedback
        NACK_PENDING // NACK received, retransmission needed
    };

    State state = FREE;
    uint8_t processId = 0;
    uint8_t ndi = 0;
    uint8_t rv = 0;
    uint8_t reTxCount = 0;
    uint8_t maxRetx = 4;

    Ptr<Packet> packet;
    Time txTime;
};

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

struct Dect2020ExtRandomAccessContext
{
    Dect2020ExtRandomAccessResourceIE m_randomAccessResourceIE;
    std::vector<double> m_startPositions;
    uint32_t m_CW_MIN;
    uint32_t m_CW_MAX;
    uint32_t m_CW_CURRENT;
    Ptr<Packet> m_packet;
    Dect2020ExtPHYControlFieldType1 m_physicalHeaderFieldType1;
    Dect2020ExtPHYControlFieldType2 m_physicalHeaderFieldType2;
    int m_physicalHeaderFieldType;

    // LBT sampling
    std::vector<double> m_lbtRssiSamples; // Store RSSI samples during LBT
    uint32_t m_lbtSampleCount = 0; // Counter for collected samples
    uint32_t m_lbtTotalSamples = 5; // Total samples to collect

    // Timing reference
    Time m_rachStartTime; // Absolute time when RACH resources start
    uint32_t m_currentPositionIndex = 0; // Track which start positiion we are at
};

struct Dect2020ExtRSSI1OnChannelContext
{
    uint16_t channelNumber;
    std::vector<std::vector<double>> rssiAverages; // [subslotNumber][RSSI samples]
};

struct Dect2020ExtBackgroundScanContext
{
    // Store per-channel contexts (keyed by channel number)
    std::map<uint16_t, std::shared_ptr<Dect2020ExtRSSI1OnChannelContext>> channelContexts; // [channelNumber][RSSI averages]
};

struct Dect2020ExtSubslotScanContext
{
    uint16_t channelId;
    Dect2020ExtChannelEvaluation evaluation;
    uint32_t subslotCount;
    uint32_t totalSubslots;
    // std::function<void(const Dect2020ExtChannelEvaluation&)> onComplete;
};

struct Dect2020ExtFtCandidateInfo
{
    // uint8_t shortNetworkId; // Short Network ID of the FT candidate
    uint32_t networkId;     // Network ID of the FT candidate
    uint32_t shortFtId;     // Short RD ID of the FT candidate
    uint32_t longFtId;      // Long RD ID of the FT candidate

    uint16_t clusterChannelId;
    uint8_t sfn;
    Time receptionTime;
    double rssiDbm;

    Dect2020ExtPHYControlFieldType1 ftPhyHeaderField;       // Last received Physical Header Field
    Dect2020ExtBeaconHeader ftBeaconHeader;                 // Last received Beacon Header
    Dect2020ExtUnicastHeader ftUnicastHeader;               // Last received Unicast Header
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
    Dect2020ExtRdCapabilityIE rdCapabilityIe;

    // double lastRssi;
};

class Dect2020ExtMac : public Object
{

    public:
        Dect2020ExtMac();
        virtual ~Dect2020ExtMac();

        static TypeId GetTypeId();

        enum Dect2020ExtAssociationStatus
        {
            NOT_ASSOCIATED,
            ASSOCIATION_PREPARING,
            WAITING_FOR_SELECTED_FT,
            ASSOCIATION_PENDING,
            ASSOCIATED
        };

        // Test
        void TestFunction();

        // Initialization

        void Initialize();
        void Start();
        void DiscoverNetworks();
        void InitializeNetwork();
        uint32_t GenerateValidNetworkID();
        uint32_t GenerateLongRadioDeviceId();
        uint16_t GenerateShortRadioDeviceId();

        // Spectrum Management Procedure

        void DisplayChannelEvaluations();
        void ScheduleBackgroundScan();
        void OperatingChannelSelection(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext);
        void SampleRSSIFromInterference(std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> context, uint32_t subslotNumber);
        void StartSubslotScan(std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext, uint32_t subslotNumber);
        void EndSubslotScan(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext, uint32_t subslotNumber);
        void MeasureRSSI1OnChannel(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext);
        void BackgroundScan();

        // Beacon Transmission

        void StartBeaconTransmission();
        void StartNetworkBeaconTransmission();
        void StartClusterBeaconTransmission();
        void SendNetworkBeaconOnChannel(uint16_t channelId);
        void SendClusterBeaconOnChannel(uint16_t channelId);

        // Building MAC PDU

        Ptr<Packet> BuildNetworkBeacon();
        Ptr<Packet> BuildClusterBeacon(uint32_t clusterChannelId);
        Dect2020ExtRandomAccessResourceIE BuildRandomAccessResourceIE();
        Dect2020ExtPHYControlFieldType1 BuildPhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes);

        // Beacon Transmission Helper Functions

        uint8_t CalculateAbsoluteStartSubslot();
        uint32_t ConvertNetworkBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod period);
        uint32_t ConvertClusterBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod period);
        uint8_t CalculateMacPduSizeInSubslots(uint32_t packetLengthInBytes) const;
        uint8_t CalculateMacPduSizeInSlots(uint32_t packetLengthInBytes) const;

        // Random Access Procedure

        void InitiateRandomAccess(Dect2020ExtRandomAccessResourceIE randomAccessResourceIE, Ptr<Packet> packet, Dect2020ExtPHYControlFieldType1 physicalHeaderField);
        void InitiateHarqRandomAccess(Dect2020ExtRandomAccessResourceIE randomAccessResourceIE, Ptr<Packet> packet, Dect2020ExtPHYControlFieldType2 physicalHeaderField);
        void InitiateBackoff(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void PerformLBT(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void SampleLBTRSSI(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void EvaluateLBTChannel(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        bool CheckIfMacPduFitsInMaxRandomAccessTxTime(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const;

        // Association Procedure

        void ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm, bool data);
        void HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtCandidateInfo* ft);
        void HandleUnicastPacket(Ptr<Packet> packet);
        void HandleDataPacket(Ptr<Packet> packet);
        Dect2020ExtFtCandidateInfo* FindOrCreateFtCandidate(uint16_t shortFtId);
        void EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIE& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIE& randomAccessResourceIE, Dect2020ExtFtCandidateInfo* ft);
        void SelectBestFtCandidate();
        void VerifyWaitingForSelectedFtAssociationStatus(void);
        void SendAssociationRequest(Dect2020ExtFtCandidateInfo* ft);
        void VerifyPendingAssociationStatus();
        void ProcessAssociationRequest(const Dect2020ExtAssociationControlIE associationControlIE, const Dect2020ExtRdCapabilityIE& rdCapabilityIE, const Dect2020ExtAssociationRequestMessageIE& associationRequestMessage, uint32_t transmitterAddress);
        void SendAssociationResponse(Dect2020ExtAssociatedPtInfo ptInfo);
        void ProcessAssociationResponse(Dect2020ExtAssociationResponseMessageIE associationResponseMessage, uint32_t transmitterAddress);
        void VerifyAssociatedStatus();
        Dect2020ExtFtCandidateInfo* FindFtCandidateByLongId(uint32_t longFtId);
        Time CalculcateTimeOffsetFromCurrentSubslot(uint32_t delayInSubslots);

        // Dect2020ExtMac Setters and Getters

        void SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType);
        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType GetTerminationPointType(void) const;
        void SetNetDevice(Ptr<Dect2020ExtNetDevice> device);
        Ptr<Dect2020ExtNetDevice> GetNetDevice(void) const;
        Ptr<Node> GetNode() const;
        void SetPhy(Ptr<Dect2020ExtPhy> phy);
        Ptr<Dect2020ExtPhy> GetPhy() const;
        void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
        void SetNetworkId(uint32_t networkId);
        uint32_t GetNetworkId() const;
        void SetLongRadioDeviceId(uint32_t rdId);
        uint32_t GetLongRadioDeviceId() const;
        void SetShortRadioDeviceId(uint16_t rdId);
        uint16_t GetShortRadioDeviceId() const;
        uint32_t GetOperatingChannelId() const;
        std::vector<Dect2020ExtChannelEvaluation> GetChannelEvaluations(void) const;
        void SetCurrentChannelId(uint16_t channelId);
        uint16_t GetCurrentChannelId() const;
        double GetRxGainFromIndex(uint8_t index) const;
        Ptr<Dect2020ExtChannelManager> GetChannelManager() const;

        void SetMacScheduler(Ptr<Dect2020ExtMacScheduler> macScheduler);

        Dect2020ExtAssociationStatus GetAssociationStatus();
        void ChangeAssociationStatus(Dect2020ExtAssociationStatus associationStatus);

        // HARQ
        void InitiateHARQ();
        void SendHarqAck();

        // Application
        void EnqueuePacket();
        void LaunchApp();

        // ACK/NACK
        Dect2020ExtPHYControlFieldType2 BuildPhysicalHeaderFieldType2(uint8_t packetLengthType, uint32_t packetLengthBytes);

        uint32_t m_associatedFTNetDeviceLongRdId = 0;
        Dect2020ExtFtCandidateInfo m_selectedFtCandidate;
        uint8_t m_nextAvailableSubslot = 2;
        bool m_dectDelay = false;
        uint16_t m_currentChannelId;

        // ###########################
        // ###### Configuration ######
        // ###########################

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber() const;
        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor() const;
        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor() const;
        void ConfigureRandomAccessResources(Dect2020ExtRandomAccessResourceIE randomAccessResourceIE);
        void ConfigureAssociationControlIE(Dect2020ExtAssociationControlIE associationControlIE);
        void ConfigureRngStream(int64_t stream);
        void PrintConfiguration() const;

        std::string MAC_IDENTITY;

    private:
        std::vector<Dect2020ExtChannelEvaluation> m_channelEvaluations;
        std::map<uint16_t, std::vector<double>> m_rssiByChannel;
        uint16_t m_operatingChannelId;
        uint16_t m_networkBeaconChannelId;
        uint32_t m_networkId;
        uint32_t m_longRadioDeviceId;
        uint16_t m_shortRadioDeviceId;
        Ptr<Dect2020ExtMacScheduler> m_macScheduler;

        Time m_networkBeaconPeriodTime = MilliSeconds(100); // Network Beacon Period
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod m_networkBeaconPeriod = Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod::NETWORK_BEACON_PERIOD_100MS;
        Time m_clusterBeaconPeriodTime = MilliSeconds(50); // Cluster Beacon Period
        Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod m_clusterBeaconPeriod = Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod::CLUSTER_BEACON_PERIOD_50MS;

        uint8_t m_mcs = 0; // Modulation and Coding Scheme

        uint32_t m_networkBeaconSweepCount;
        Time m_nextNetworkBeaconTime;

        uint32_t m_networkBeaconSubslot = 0;
        uint32_t m_clusterBeaconSubslot = 4;

        std::vector<Dect2020ExtFtCandidateInfo> m_ftCandidates;
        Dect2020ExtMacMultiplexingHeaderTypeA m_lastFtMacMxHeader;
        Dect2020ExtAssociationStatus m_associationStatus;
        bool m_isWaitingForClusterBeacon;
        uint8_t m_lastSfn;
        uint8_t m_potentialShortNetworkId;
        std::vector<Dect2020ExtAssociatedPtInfo> m_associatedPtDevices;
        Ptr<UniformRandomVariable> m_backoffRandomVariable;

        // Application
        std::queue<Ptr<Packet>> m_txQueue;
        Ptr<Dect2020ExtApp> m_app;

        // HARQ
        static constexpr uint8_t HARQ_MAX_PROCESSES = 1;
        std::array<Dect2020ExtHarqProcess, HARQ_MAX_PROCESSES> m_harqTable;

        // ###########################
        // ###### Configuration ######
        // ###########################

        uint8_t m_bandNumber;
        bool m_bandNumberSet = false;

        uint8_t m_subcarrierScalingFactor;
        bool m_subcarrierScalingFactorSet = false;

        uint8_t m_fourierTransformScalingFactor;
        bool m_fourierTransformScalingFactorSet = false;

        Ptr<Dect2020ExtChannelManager> m_channelManager;

        Dect2020ExtNetDevice::Dect2020ExtTerminationPointType m_terminationPointType;
        bool m_terminationPointTypeSet = false;

        Ptr<Dect2020ExtPhy> m_phy;

        Dect2020ExtRandomAccessResourceIE m_configuredRandomAccessResourceIE;
        bool m_configuredRandomAccessResourceIESet = false;

        Dect2020ExtAssociationControlIE m_configuredAssociationControlIE;
        bool m_configuredAssociationControlIESet = false;

        int64_t m_configuredRngStream;
        bool m_configuredRngStreamSet = false;

        // Dect2020ExtRdCapabilityIE m_configuredRdCapabilityIE;
        // bool m_configuredRdCapabilityIESet = false;

        Ptr<Dect2020ExtNetDevice> m_device;

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

        // CONSTANTS SPECIFICALLY FOR SUBCARRIER SCALING FACTOR 1
        const double GI_US = 18.52; // GI in microseconds GI = (4/9) * T_u_symb
        const double STF_US = 64.815; // STF = (14/9) * T_u_symb
        const double MINIMUM_LBT_PERIOD_US = STF_US + GI_US;
};

} // namespace ns3

#endif /* DECT2020EXT_MAC_H */