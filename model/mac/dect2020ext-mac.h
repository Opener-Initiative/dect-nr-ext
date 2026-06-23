#ifndef DECT2020EXT_MAC_H
#define DECT2020EXT_MAC_H

#include "ns3/dect2020ext-net-device.h"
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
#include "ns3/dect2020ext-phy-control-field-type-2.h"
#include "ns3/dect2020ext-mac-header-type.h"

#include "ns3/node.h"

#include <map>
#include <vector>
#include <queue>

namespace ns3
{

using Dect2020ExtNetworkBeaconPeriod = Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod;
using Dect2020ExtClusterBeaconPeriod = Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod;
using Dect2020ExtTerminationPointType = Dect2020ExtNetDevice::Dect2020ExtTerminationPointType;

class Dect2020ExtApp;
class Dect2020ExtPhy;
struct Dect2020ExtRandomAccessContext;

struct Dect2020ExtHarqProcess
{
    enum State
    {
        FREE, // Available for a new packet
        WAITING_ACK, // Packet sent, waiting for feedback
    };

    void Reset()
    {
        state = FREE;
        ndi = true;
        reTxCount = 0;
        data = false;
        packet = nullptr;
        randomAccessContext = nullptr;
    }

    State state = FREE;
    bool ndi;
    uint8_t reTxCount = 0;
    bool data;
    Ptr<Packet> packet;
    std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext;

    const uint8_t processNumber = 0; // Currently, only one process can exist with the process number 0
    const uint8_t maxRetx = 4; // Fixed number of max retransmissions, currently not used
    const bool random = true; // This context is only used for the Random Access Procedure
};

struct Dect2020ExtRandomAccessContext
{
    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
    std::vector<double> startPositions;
    uint32_t CW_MIN;
    uint32_t CW_MAX;
    uint32_t CW_CURRENT;
    Dect2020ExtHarqProcess* process;
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2;

    // LBT sampling
    std::vector<double> lbtRssiSamples; // Store RSSI samples during LBT
    uint32_t lbtSampleCount = 0; // Counter for collected samples
    uint32_t lbtTotalSamples = 5; // Total samples to collect
};

struct Dect2020ExtChannelEvaluation
{
    uint16_t channelNumber;
    uint32_t free = 0;
    uint32_t possible = 0;
    uint32_t busy = 0;

    uint32_t Total() const
    {
        return free + possible + busy;
    }
};

struct Dect2020ExtRssi1OnChannelContext
{
    uint16_t channelNumber;
    Time channelMeasurementStartTime;
    std::vector<std::vector<double>> rssiAverages; // [subslotNumber][RSSI samples]
};

struct Dect2020ExtBackgroundScanContext
{
    // Store per-channel contexts (keyed by channel number)
    std::map<uint16_t, std::shared_ptr<Dect2020ExtRssi1OnChannelContext>> channelContexts; // [channelNumber][RSSI averages]
};

struct Dect2020ExtSubslotScanContext
{
    uint16_t channelNumber;
    Dect2020ExtChannelEvaluation evaluation;
    uint32_t subslotCount;
    uint32_t totalSubslots;
};

struct Dect2020ExtFtInfo
{
    double rssiDbm = 0.0;
    uint8_t shortNetworkId = 0;
    uint32_t networkId = 0;
    uint32_t shortFtId = 0;
    uint32_t longFtId = 0;
    uint16_t clusterChannelNumber = 0;
    uint8_t sfn = 0;
    Time receptionTime;

    Dect2020ExtPhyControlFieldType1 phyControlFieldType1;                 // Last received PHY Control Field Type 1
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2;                 // Last received PHY Control Field Type 2
    Dect2020ExtBeaconHeader beaconHeader;                                 // Last received Beacon Header
    Dect2020ExtUnicastHeader unicastHeader;                               // Last received Unicast Header
    Dect2020ExtNetworkBeaconMessageIe networkBeaconMessageIe;             // Last received Network Beacon Message
    Dect2020ExtClusterBeaconMessageIe clusterBeaconMessageIe;             // Last received Cluster Beacon Message
    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;             // Last received Random Access Resource IE
    Dect2020ExtRdCapabilityIe rdCapabilityIe;                             // Last received RD Capability IE
    Dect2020ExtAssociationResponseMessageIe associationResponseMessageIe; // Last received Association Response Message IE
};

struct Dect2020ExtPtInfo
{
    Time associationAcceptedTime;
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2;
    Dect2020ExtUnicastHeader unicastHeader;
    Dect2020ExtAssociationRequestMessageIe associationRequestMessageIe;
    Dect2020ExtRdCapabilityIe rdCapabilityIe;
    Dect2020ExtAssociationControlIe associationControlIe;
};

class Dect2020ExtMac : public Object
{

    public:
        Dect2020ExtMac();
        virtual ~Dect2020ExtMac();

        static TypeId GetTypeId();

        enum Dect2020ExtAssociationStatus
        {
            NOT_ASSOCIATED, // The device is not associated
            ASSOCIATION_PREPARING, // The device is discovering networks to find the best FT candidate for association
            WAITING_FOR_SELECTED_FT, // The device is waiting when another Cluster Beacon will trigger association
            ASSOCIATION_PENDING, // The device is in the procedd of association and executing the Random Access Procedure
            ASSOCIATED // The device is associated
        };

        void TestFunction();

        // ###### Initialization ######

        void Initialize();
        void Start();
        void DiscoverNetworks();
        void InitializeNetwork();
        uint32_t GenerateValidNetworkId();
        uint32_t GenerateLongRadioDeviceId();
        uint16_t GenerateShortRadioDeviceId();

        // ###### Background Scan ######

        void BackgroundScan();
        void MeasureRSSI1OnChannel(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext);
        void StartSubslotScan(std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext, uint32_t subslotNumber);
        void EndSubslotScan(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext, uint32_t subslotNumber);
        void SampleRSSIFromInterference(std::shared_ptr<Dect2020ExtRssi1OnChannelContext> context, uint32_t subslotNumber);
        void OperatingChannelSelection(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext);
        void DisplayChannelEvaluations();

        // ###### Beacon Transmission ######

        void StartBeaconTransmission();
        void StartNetworkBeaconTransmission();
        void StartClusterBeaconTransmission();
        void SendNetworkBeaconOnChannel(uint16_t channelNumber);
        void SendClusterBeaconOnChannel(uint16_t channelNumber);

        // ###### Building MAC PDU ######

        Ptr<Packet> BuildNetworkBeacon();
        Ptr<Packet> BuildClusterBeacon(uint16_t clusterChannelNumber);
        Dect2020ExtRandomAccessResourceIe BuildRandomAccessResourceIe();

        // Build PHY Control Field Type 1  
        // packetLengthType: false - length in subslots, true - length in slots
        Dect2020ExtPhyControlFieldType1 BuildPhyControlFieldType1(bool packetLengthType, uint32_t packetLengthInBytes);

        // Build PHY Control Field Type 1  
        // Currently, only FORMAT_000 is used
        Dect2020ExtPhyControlFieldType2 BuildPhyControlFieldType2(bool packetLengthType, uint32_t packetSizeInBytes, bool ndi, uint8_t harqProcessNumber, bool ack);

        Ptr<Packet> BuildAssociationRequest(Dect2020ExtFtInfo* ft);
        Ptr<Packet> BuildAssociationResponse(Dect2020ExtPtInfo* ptInfo);

        // ###### MAC-PDU-Related Conversion and Calculation

        uint32_t ConvertNetworkBeaconPeriodToFrames(Dect2020ExtNetworkBeaconPeriod period);
        uint32_t ConvertClusterBeaconPeriodToFrames(Dect2020ExtClusterBeaconPeriod period);
        uint8_t CalculateMacPduSizeInSubslots(uint32_t packetSizeInBytes) const;
        uint8_t CalculateMacPduSizeInSlots(uint32_t packetSizeInBytes) const;

        // ###### HARQ, Random Access Procedure, and Data Transmission ######

        void InitiateHarq(bool data);

        // Find a free HARQ process.  
        // While only one HARQ process is currently used, this function is implemented
        // to find a free HARQ process when
        // multiple processes are available.
        Dect2020ExtHarqProcess* FindFreeHarqProcess();

        // Find a HARQ process based on the process number.
        Dect2020ExtHarqProcess* FindHarqProcess(uint8_t harqProcessNumber);

        void InitiateRandomAccess(Dect2020ExtRandomAccessResourceIe randomAccessResourceIe,
                                  Dect2020ExtHarqProcess* harqProcess,
                                  Dect2020ExtPhyControlFieldType2 phyControlFieldType2,
                                  bool newContext = true,
                                  std::shared_ptr<Dect2020ExtRandomAccessContext> previousContext = nullptr);
        void InitiateBackoff(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void PerformLbt(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void SampleLbtRssi(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        void EvaluateLbtChannel(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext);
        bool CheckIfMacPduFitsInMaxRandomAccessTxTime(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const;

        // Check if the random access transmission (response window)
        // fits into the remaining RACH resources
        bool CheckIfRachTxFitsInRemainingResources(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const;
        void VerifyHarqAck(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2, bool data);
        void HandleHarqAck(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2);
        void SendHarqAck(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2);
        void MonitorTxQueue();
        void StartKeepAlivePeriod();
        void VerifyKeepAlivePeriodNotExceeded();
        void RestartKeepAlivePeriod();
        void EnqueuePacket();
        void LaunchApp();

        // ###### MAC Packet Handling ######

        void ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm, bool random);
        void HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtInfo* ft);
        void HandleUnicastPacket(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2);
        void HandleDataPacket(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2);

        // Check if the Unicast Message is for this device.
        bool PacketIsForThisDevice(Dect2020ExtUnicastHeader unicastHeader);

        // ###### Association Procedure ######

        Dect2020ExtFtInfo* FindOrCreateFtCandidate(uint16_t shortFtId);

        // Find FT candidate among all possible candidates for association.  
        // This function asserts that the candidate exists and must be called
        // in the context where the candidate is assumed to exist.
        Dect2020ExtFtInfo* FindFtCandidate(uint16_t shortFtId);

        Dect2020ExtFtInfo* FindFtCandidateByLongId(uint32_t longFtId);

        // Create PT device info
        // or find the PT device info among all associated PT device infos
        Dect2020ExtPtInfo* FindOrCreateAssociatedPtInfo(uint16_t shortPtId);

        void EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIe& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIe& randomAccessResourceIe, Dect2020ExtFtInfo* ft);
        void SelectBestFtCandidate();
        void SendAssociationRequest(Dect2020ExtFtInfo* ft);
        void ProcessAssociationRequest(Dect2020ExtPtInfo* ptInfo);
        void SendAssociationResponse(Dect2020ExtPtInfo* ptInfo);
        void ProcessAssociationResponse(Dect2020ExtFtInfo* ftInfo);
        void ChangeAssociationStatus(Dect2020ExtAssociationStatus associationStatus);

        // void VerifyWaitingForSelectedFtAssociationStatus();
        // void VerifyPendingAssociationStatus();
        // void VerifyAssociatedStatus();

        // ###### Dect2020ExtMac Setters and Getters ######

        Ptr<Node> GetNode() const;

        void SetNetworkId(uint32_t networkId);
        uint32_t GetNetworkId() const;

        void SetLongRadioDeviceId(uint32_t rdId);
        uint32_t GetLongRadioDeviceId() const;

        void SetShortRadioDeviceId(uint16_t rdId);
        uint16_t GetShortRadioDeviceId() const;

        void SetOperatingChannelNumber(uint16_t channelNumber);
        uint16_t GetOperatingChannelNumber() const;

        std::vector<Dect2020ExtChannelEvaluation> GetChannelEvaluations() const;

        double GetRxGainFromIndex(uint8_t index) const;

        void SetMacScheduler(Ptr<Dect2020ExtMacScheduler> macScheduler);

        Dect2020ExtAssociationStatus GetAssociationStatus();

        // ###### Configuration ######

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber() const;

        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor() const;

        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor() const;

        void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
        Ptr<Dect2020ExtChannelManager> GetChannelManager() const;

        void SetTerminationPointType(Dect2020ExtTerminationPointType terminationPointType);
        Dect2020ExtTerminationPointType GetTerminationPointType() const;

        void SetPhy(Ptr<Dect2020ExtPhy> phy);
        Ptr<Dect2020ExtPhy> GetPhy() const;

        void ConfigureRandomAccessResources(Dect2020ExtRandomAccessResourceIe randomAccessResourceIe);
        void ConfigureAssociationControlIe(Dect2020ExtAssociationControlIe associationControlIe);
        void ConfigureRngStream(int64_t stream);

        void SetNetDevice(Ptr<Dect2020ExtNetDevice> device);
        Ptr<Dect2020ExtNetDevice> GetNetDevice() const;

        void ProceedToBeaconTransmission(bool value);
        void ProceedToAssociationRequest(bool value);
        void ProceedToAssociationResponse(bool value);
        void ProceedToDataTransmission(bool value);

        void DisplayChannelEvaluations(bool value);
        void PrintConfiguration() const;

        void SetSearchBestFtTimeMs(double timeMs);

        void SetPacketLimit(int packetLimit);
        void SetPacketGenerationPeriod(Time period);

        // ###### Required to be public for dect2020ext-utilities.cc ######
        uint32_t m_associatedFtNetDeviceLongRdId = 0;
        Dect2020ExtFtInfo m_selectedFtCandidate;
        std::string MAC_IDENTITY;

    private:
        uint16_t m_operatingChannelNumber;
        std::vector<Dect2020ExtChannelEvaluation> m_channelEvaluations;
        std::map<uint16_t, std::vector<double>> m_rssiByChannel;
        uint32_t m_networkId;
        uint32_t m_longRadioDeviceId;
        uint16_t m_shortRadioDeviceId;
        Ptr<Dect2020ExtMacScheduler> m_macScheduler;
        Time m_networkBeaconPeriodTime = MilliSeconds(100); // Network Beacon Period
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod m_networkBeaconPeriod = Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod::NETWORK_BEACON_PERIOD_100MS;
        Time m_clusterBeaconPeriodTime = MilliSeconds(50); // Cluster Beacon Period
        Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod m_clusterBeaconPeriod = Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod::CLUSTER_BEACON_PERIOD_50MS;
        uint8_t m_mcs = 0; // Modulation and Coding Scheme
        uint32_t m_networkBeaconSweepCount;
        Time m_nextNetworkBeaconTime;
        uint32_t m_networkBeaconSubslot = 0;
        uint32_t m_clusterBeaconSubslot = 4;
        std::vector<Dect2020ExtFtInfo> m_ftCandidates;
        Dect2020ExtAssociationStatus m_associationStatus = NOT_ASSOCIATED;
        bool m_isWaitingForClusterBeacon;
        uint8_t m_lastSfn;
        uint8_t m_potentialShortNetworkId;
        std::vector<Dect2020ExtPtInfo> m_associatedPtDevices;
        Ptr<Dect2020ExtApp> m_app;
        Time m_lastRxTime;
        bool m_dectDelay = false;

        // Random Access
        Ptr<UniformRandomVariable> m_backoffRandomVariable;

        // Tx Queue
        std::queue<Ptr<Packet>> m_txQueue;

        // ###### HARQ ######

        static constexpr uint8_t HARQ_MAX_PROCESSES = 1;
        std::array<Dect2020ExtHarqProcess, HARQ_MAX_PROCESSES> m_harqProcesses;

        // ###### Configuration ######

        uint8_t m_bandNumber;
        bool m_bandNumberSet = false;

        uint8_t m_subcarrierScalingFactor;
        bool m_subcarrierScalingFactorSet = false;

        uint8_t m_fourierTransformScalingFactor;
        bool m_fourierTransformScalingFactorSet = false;

        Ptr<Dect2020ExtChannelManager> m_channelManager;

        Dect2020ExtTerminationPointType m_terminationPointType;
        bool m_terminationPointTypeSet = false;

        Ptr<Dect2020ExtPhy> m_phy;

        Dect2020ExtRandomAccessResourceIe m_configuredRandomAccessResourceIe;
        bool m_configuredRandomAccessResourceIeSet = false;

        Dect2020ExtAssociationControlIe m_configuredAssociationControlIe;
        bool m_configuredAssociationControlIeSet = false;

        int64_t m_configuredRngStream;
        bool m_configuredRngStreamSet = false;

        Ptr<Dect2020ExtNetDevice> m_device;

        bool m_proceedToBeaconTransmission = true;
        bool m_proceedToAssociationRequest = true;
        bool m_proceedToAssociationResponse = true;
        bool m_proceedToDataTransmission = true;

        bool m_displayChannelEvaluations = false;

        double m_searchBestFtTimeMs = 0;

        int m_packetLimit;
        bool m_packetLimitSet = false;

        Time m_packetGenerationPeriod;
        bool m_packetGenerationPeriodSet = false;

        // ###### Constants ######

        const double RSSI_THRESHOLD_MIN = -85; // dBm
        const double RSSI_THRESHOLD_MAX = -50; // dBm TODO: RSSI_THRESHOLDMAX = -52 - max TX Power of RD
        const int SCAN_MEAS_DURATION = 24;     // Slots
        const double SCAN_SUITABLE = 0.75;     // Threshold when an operating channel can be considered
                                               // fulfilling the operating conditions.
        const int SCAN_STATUS_VALID = 300;     // Seconds

        // ###### Constants specifically for subcarrier scaling factor 1 ######

        const double GI_US = 18.52; // GI in microseconds GI = (4/9) * T_u_symb
        const double STF_US = 64.815; // STF = (14/9) * T_u_symb
        const double MINIMUM_LBT_PERIOD_US = STF_US + GI_US;
        const double START_POSITION_INTERVAL_US = STF_US + GI_US;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_H