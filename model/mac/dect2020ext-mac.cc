#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-association-request-message-ie.h"
#include "ns3/dect2020ext-unicast-header.h"
#include "ns3/dect2020ext-association-response-message-ie.h"
#include "ns3/dect2020ext-rd-capability-ie.h"
#include "ns3/dect2020ext-association-control-ie.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-config-values.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"
#include "ns3/pointer.h"

#include <cstdint>
#include <cstdlib>
#include <glib.h>
#include <limits>
#include <memory>
#include <sstream>
#include <algorithm>
#include <string>
#include <iomanip>

namespace ns3
{

using PhyState = Dect2020ExtPhy::PhyState;
using MacHeaderTypeField = Dect2020ExtMacHeaderType::MacHeaderTypeField;
using IeTypeFieldEncoding = Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding;
using DlDataReception = Dect2020ExtAssociationControlIe::DlDataReception;
using UlPeriod = Dect2020ExtAssociationControlIe::UlPeriod;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
}

Dect2020ExtMac::~Dect2020ExtMac()
{
}

TypeId
Dect2020ExtMac::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMac")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtMac>();
    return tid;
}

void
Dect2020ExtMac::TestFunction()
{
    if (m_terminationPointType == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Hi, I am FT!");
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Hi, I am PT!");
    }
}

void
Dect2020ExtMac::Initialize()
{
    std::string errorMessage = " was not set for Dect2020ExtMac";

    NS_ABORT_MSG_IF(!m_bandNumberSet, "Band number" << errorMessage);
    NS_ABORT_MSG_IF(!m_subcarrierScalingFactorSet, "Subcarrier scaling factor" << errorMessage);
    NS_ABORT_MSG_IF(!m_fourierTransformScalingFactorSet, "Fourier transform scaling factor is not set for Dect2020ExtMac");
    NS_ABORT_MSG_IF(!m_channelManager, "Channel Manager is not set for Dect2020ExtMac");
    NS_ABORT_MSG_IF(!m_terminationPointTypeSet, "Termination Point Type is not set for Dect2020ExtMac");
    NS_ABORT_MSG_IF(!m_phy, "PHY is not set for Dect2020ExtMac");
    NS_ABORT_MSG_IF(!m_device, "Net Device is not set for Dect2020ExtMac");

    // MAC Scheduler
    m_macScheduler = CreateObject<Dect2020ExtMacScheduler>();

    m_macScheduler->SetBandNumber(m_bandNumber);
    m_macScheduler->SetSubcarrierScalingFactor(m_subcarrierScalingFactor);
    m_macScheduler->SetFourierTransformScalingFactor(m_fourierTransformScalingFactor);
    m_macScheduler->SetChannelManager(m_channelManager);

    m_macScheduler->SetMac(this);
    m_macScheduler->Initialize();
    m_macScheduler->StartMacScheduler();

    // Random Variable
    m_backoffRandomVariable = CreateObject<UniformRandomVariable>();

    if (!m_configuredRngStreamSet)
    {
        m_backoffRandomVariable->SetStream(GetNode()->GetId());
    }
    else
    {
        m_backoffRandomVariable->SetStream(m_configuredRngStream);
    }

    // Network ID for FT
    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        SetNetworkId(GenerateValidNetworkId());
    }
}

void
Dect2020ExtMac::Start()
{
    std::string errorMessage = " was not set for Dect2020ExtMac";

    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        NS_ABORT_MSG_IF(!m_configuredRandomAccessResourceIeSet, "Random Access Resource Ie" << errorMessage);

        std::ostringstream oss;
        oss << "FT_MAC[" << m_device->GetNode()->GetId() << "]";
        MAC_IDENTITY = oss.str();

        SetLongRadioDeviceId(GenerateLongRadioDeviceId());
        SetShortRadioDeviceId(GenerateShortRadioDeviceId());

        InitializeNetwork();
    }
    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
    {
        NS_ABORT_MSG_IF(!m_configuredAssociationControlIeSet, "Association Control Ie" << errorMessage);

        std::ostringstream oss;
        oss << "PT_MAC[" << m_device->GetNode()->GetId() << "]";
        MAC_IDENTITY = oss.str();

        SetLongRadioDeviceId(GenerateLongRadioDeviceId());
        SetShortRadioDeviceId(GenerateShortRadioDeviceId());

        DiscoverNetworks();
    }
}

void
Dect2020ExtMac::DiscoverNetworks()
{
    if (m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING ||
        m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT ||
        m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATED)
    {
        return; // Device is in association process --> abort discovering networks
    }

    BandParameters bp = m_channelManager->GetBandParameters(this->GetBandNumber());

    // Advance to the next channel; wrap around to nStart when outside [nStart, nEnd-1]
    // The lower-bound check also handles the uninitialized / out-of-band case on first entry.
    uint16_t nextChannelNumber = (m_operatingChannelNumber >= bp.nStart && m_operatingChannelNumber < bp.nEnd)
                                 ? m_operatingChannelNumber + 1
                                 : bp.nStart;

    // Switch to the next channel
    NS_LOG_INFO(MAC_IDENTITY << ": Switching to channel " << nextChannelNumber);
    SetOperatingChannelNumber(nextChannelNumber);

    Time t = MilliSeconds(10); // Discover Network wait time
    Simulator::Schedule(t, &Dect2020ExtMac::DiscoverNetworks, this); // Schedule next discovery
}

void
Dect2020ExtMac::InitializeNetwork()
{
    NS_LOG_INFO(MAC_IDENTITY << ": Starting background scan");

    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::BackgroundScan, this);
}

uint32_t
Dect2020ExtMac::GenerateValidNetworkId()
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint32_t networkId = 0;
    uint8_t lsb;
    uint32_t msb;
    do
    {
        networkId = randomVar->GetValue(1, std::numeric_limits<uint32_t>::max());
        lsb = networkId & 0xFF;
        msb = (networkId >> 8) & 0xFFFFFF;
    } while (lsb == 0x00 || msb == 0x000000);

    return networkId;
}

// TODO: Mitigate collision of long radio device IDs
uint32_t
Dect2020ExtMac::GenerateLongRadioDeviceId()
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint32_t rdId = 0;

    do
    {
        rdId = randomVar->GetValue(1, 0xFFFFFFFD); // Range 0x00000001 bis 0xFFFFFFFD
    } while (rdId == 0x00000000 || rdId == 0xFFFFFFFE || rdId == 0xFFFFFFFF);

    NS_LOG_DEBUG(Simulator::Now().GetMilliSeconds()
                 << ": Long Radio Device ID " << rdId << "generated.");

    return rdId;
}

uint16_t
Dect2020ExtMac::GenerateShortRadioDeviceId()
{
    Ptr<UniformRandomVariable> randomVar = CreateObject<UniformRandomVariable>();
    uint16_t rdId = 0;

    do
    {
        rdId = randomVar->GetValue(1, 0xFFFE); // Range 0x0001 to 0xFFFE
    } while (rdId == 0x0000 || rdId == 0xFFFF);

    NS_LOG_DEBUG(Simulator::Now().GetMilliSeconds()
                 << ": Short Radio Device ID " << rdId << "generated.");

    return rdId;
}


void
Dect2020ExtMac::DisplayChannelEvaluations()
{
    NS_LOG_INFO("============ Channel Evaluations Summary ============");
    
    // Display all channel evaluations
    if (m_channelEvaluations.empty())
    {
        NS_FATAL_ERROR("No channel evaluations available yet at: " << Simulator::Now().GetMilliSeconds());
        return;
    }
    
    // Display header
    NS_LOG_INFO("Channel ID | Free | Possible | Busy | Total | Free %");
    NS_LOG_INFO("-----------------------------------------------------");
    
    // Display each channel evaluation
    for (const auto& eval : m_channelEvaluations)
    {
        uint32_t total = eval.Total();
        double freePercentage = (total > 0) ? (eval.free * 100.0 / total) : 0.0;
        
        std::ostringstream oss;
        oss << std::setw(10) << eval.channelNumber << " | "
            << std::setw(4) << eval.free << " | "
            << std::setw(8) << eval.possible << " | "
            << std::setw(4) << eval.busy << " | "
            << std::setw(5) << total << " | "
            << std::fixed << std::setprecision(1) << std::setw(5) << freePercentage << "%";
        
        NS_LOG_INFO(oss.str());
    }
    
    NS_LOG_INFO("====================================================");
}

void
Dect2020ExtMac::OperatingChannelSelection(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext)
{
    NS_LOG_FUNCTION(this);
    std::map<uint16_t, std::vector<double>> channelRssiAverages; // [channelNumber][vector of RSSI1 averages per subslots]
    std::vector<Dect2020ExtChannelEvaluation> evaluations;

    for (auto it = backgroundScanContext->channelContexts.begin(); it != backgroundScanContext->channelContexts.end(); ++it)
    {
        // each subslot
        for (auto &subslot : it->second->rssiAverages)
        {
            double averageRssi = 0; // average RSSI1 of the subslot
            // each rssi sample
            for (auto rssiSample : subslot)
            {
                averageRssi += rssiSample;
            }
            averageRssi /= subslot.size();
            channelRssiAverages[it->first].push_back(averageRssi);
        }
    }
    for (auto it = channelRssiAverages.begin(); it != channelRssiAverages.end(); ++it)
    {
        // Create an evaluation for each channel
        Dect2020ExtChannelEvaluation evaluation;
        evaluation.channelNumber = it->first;

        for (uint32_t subslotNumber = 0; subslotNumber < it->second.size(); ++subslotNumber)
        {
            double rssi = it->second[subslotNumber];
            if (rssi <= RSSI_THRESHOLD_MIN)
            {
                evaluation.free++;
            }
            else if (rssi > RSSI_THRESHOLD_MIN && rssi <= RSSI_THRESHOLD_MAX)
            {
                evaluation.possible++;
            }
            else if (rssi > RSSI_THRESHOLD_MAX)
            {
                evaluation.busy++;
            }
        }
        evaluations.push_back(evaluation);
    }

    // Sort by free subslots
    auto SortByFreeSubslotsLambda = [](const Dect2020ExtChannelEvaluation& a, const Dect2020ExtChannelEvaluation& b)
    {
        return a.free > b.free;
    };
    
    std::stable_sort(evaluations.begin(), evaluations.end(), SortByFreeSubslotsLambda);

    m_channelEvaluations = evaluations;
    
    if (!evaluations.empty())
    {
        m_operatingChannelNumber = evaluations[0].channelNumber;
        NS_LOG_INFO(MAC_IDENTITY << ": Selected Operating Channel: " << m_operatingChannelNumber
                    << " with " << evaluations[0].free << " free subslots");
        if (m_displayChannelEvaluations)
        {
            DisplayChannelEvaluations();
        }
    }
    else
    {
        NS_FATAL_ERROR("No channel evaluations available for selection");
    }

    if (m_proceedToBeaconTransmission)
    {
        StartBeaconTransmission();
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Execution finished");
        return;
    }
}

void
Dect2020ExtMac::SampleRSSIFromInterference(std::shared_ptr<Dect2020ExtRssi1OnChannelContext> context, uint32_t subslotNumber)
{
    // Calculate channel index from the channel number
    uint16_t channelIndex = context->channelNumber - m_channelManager->GetFirstValidChannelNumber(m_channelManager->GetBandNumber(context->channelNumber));

    // Get RSSI from interference
    double rssiChannelWatt = (*m_phy->m_interference->m_allSignals)[channelIndex];
    double rssiChannelDbm = m_channelManager->WToDbm(rssiChannelWatt);

    // Store the RSSI sample
    context->rssiAverages[subslotNumber].push_back(rssiChannelDbm);
    
    // NS_LOG_INFO(MAC_IDENTITY << ": Sampled RSSI on channel " << context->channelNumber << ", subslot " << subslotNumber << ": " << rssiChannelDbm << " dBm");
}

void
Dect2020ExtMac::StartSubslotScan(std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext, uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);

    // 1. Tune to the correct channel
    SetOperatingChannelNumber(channelContext->channelNumber);

    // 2. Get power from the subslot with sampling rate
    double subslotDurationNs = SLOT_DURATION_NS / m_channelManager->GetSubslotsPerSlot();
    double baseSamplingOffsetNs = subslotDurationNs / 5;

    for (uint32_t sampleIndex = 0; sampleIndex < 5; sampleIndex++)
    {
        double samplingOffsetNs = baseSamplingOffsetNs * sampleIndex;

        // NS_LOG_INFO("Scheduling RSSI sampling at " << samplingOffsetNs << " ns");
        Simulator::Schedule(NanoSeconds(samplingOffsetNs), &Dect2020ExtMac::SampleRSSIFromInterference, this, channelContext, subslotNumber);
    }
}

void
Dect2020ExtMac::EndSubslotScan(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext, uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    backgroundScanContext->channelContexts[channelContext->channelNumber] = channelContext;
    if (channelContext->channelNumber == bandParams.nEnd && subslotNumber == m_channelManager->GetSubslotsPerSlot() * 24 - 1)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Finished scanning all subslots on channel " << channelContext->channelNumber);
        NS_LOG_INFO(MAC_IDENTITY << ": Finished scanning all channels");
        OperatingChannelSelection(backgroundScanContext);
    }

    else if (subslotNumber == m_channelManager->GetSubslotsPerSlot() * 24 - 1)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Finished scanning all subslots on channel " << channelContext->channelNumber);
    }
}

void
Dect2020ExtMac::MeasureRSSI1OnChannel(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Starting to scan channel " << channelContext->channelNumber);

    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotsPerChannel = numSubslotsPerSlot * SLOTS_PER_FRAME;

    // Store the measurement start time for testing
    channelContext->channelMeasurementStartTime = Simulator::Now();

    // Initialize the RSSI average vector for the channel
    channelContext->rssiAverages.resize(subslotsPerChannel);

    // Schedule StartSlotScan at the start of all subslots inside the channel
    // Schedule EndSlotScan at the end of all subslots inside the channel
    for (uint32_t subslotNumber = 0; subslotNumber < subslotsPerChannel; subslotNumber++)
    {
        double subslotStartTimeNs = m_macScheduler->GetSubslotStartTimeOffsetNs(subslotNumber);
        double subslotEndTimeNs = m_macScheduler->GetSubslotEndTimeOffsetNs(subslotNumber);

        Simulator::Schedule(NanoSeconds(subslotStartTimeNs), &Dect2020ExtMac::StartSubslotScan, this, channelContext, subslotNumber);
        Simulator::Schedule(NanoSeconds(subslotEndTimeNs), &Dect2020ExtMac::EndSubslotScan, this, backgroundScanContext, channelContext, subslotNumber);
    }
}

void
Dect2020ExtMac::BackgroundScan()
{
    NS_LOG_FUNCTION(this);
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    double baseScanDelayMs = 0;

    // Create context for background scan
    std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext = std::make_shared<Dect2020ExtBackgroundScanContext>();

    for (uint32_t channelNumber = bandParams.nStart; channelNumber <= bandParams.nEnd; channelNumber++)
    {
        // Create context for each channel scan
        std::shared_ptr<Dect2020ExtRssi1OnChannelContext> channelContext = std::make_shared<Dect2020ExtRssi1OnChannelContext>();
        channelContext->channelNumber = channelNumber;

        // Schedule MeasureRSSI1OnChannel for the channel
        // Use one background scan context for all channels
        Simulator::Schedule(MilliSeconds(baseScanDelayMs), &Dect2020ExtMac::MeasureRSSI1OnChannel, this, backgroundScanContext, channelContext);
        baseScanDelayMs += 10; // 10ms delay between channels (frame duration)
    }
}

void
Dect2020ExtMac::StartBeaconTransmission()
{
    // Network Beacon Transmission - subslot 0 (start of the first slot)
    Dect2020ExtMacSchedulerOperation networkBeaconOperation;
    networkBeaconOperation.operationType = Dect2020ExtMacSchedulerOperation::TX_NETWORK_BEACON;
    networkBeaconOperation.channelNumber = m_operatingChannelNumber;
    networkBeaconOperation.startSubslot = 0;
    networkBeaconOperation.endSubslot = 0;

    networkBeaconOperation.operation = [this](){
        this->StartNetworkBeaconTransmission();
    };
    networkBeaconOperation.periodFrames = ConvertNetworkBeaconPeriodToFrames(m_networkBeaconPeriod);
    networkBeaconOperation.frameOffset = m_macScheduler->GetSfn() + 1; // +1 since the operation is scheduled on the next frame

    m_networkBeaconSubslot = 0;

    m_macScheduler->ScheduleOperationOnNextFrame(networkBeaconOperation);

    // Cluster Beacon Transmission - subslot 3 (start of the second slot)
    Dect2020ExtMacSchedulerOperation clusterBeaconOperation;
    clusterBeaconOperation.operationType = Dect2020ExtMacSchedulerOperation::TX_CLUSTER_BEACON;
    clusterBeaconOperation.channelNumber = m_operatingChannelNumber;
    clusterBeaconOperation.startSubslot = 4;
    clusterBeaconOperation.endSubslot = 4;

    clusterBeaconOperation.operation = [this](){
        this->StartClusterBeaconTransmission();
    };

    clusterBeaconOperation.periodFrames = ConvertClusterBeaconPeriodToFrames(m_clusterBeaconPeriod);
    clusterBeaconOperation.frameOffset = m_macScheduler->GetSfn() + 1; // +1 since the operation is scheduled on the next frame

    m_clusterBeaconSubslot = 4;

    m_macScheduler->ScheduleOperationOnNextFrame(clusterBeaconOperation);
}

void
Dect2020ExtMac::StartNetworkBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendNetworkBeaconOnChannel, this, m_operatingChannelNumber);
}

void
Dect2020ExtMac::StartClusterBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendClusterBeaconOnChannel, this, m_operatingChannelNumber);
}

void
Dect2020ExtMac::SendNetworkBeaconOnChannel(uint16_t channelNumber)
{
    // Sanity check to assert that the device is currently not transmmitting another packet
    if (m_phy->GetState() == PhyState::TX)
    {
        NS_FATAL_ERROR("Device is already transmitting a packet while attempting to transmit another packet");
    }

    NS_LOG_INFO(MAC_IDENTITY << ": Sending Network Beacon (channel=" << m_operatingChannelNumber << ",slot=" << m_macScheduler->GetCurrentSlotNumber() << ",subslot=" << m_macScheduler->GetCurrentSubslotNumber() << ")");

    Ptr<Packet> packet = BuildNetworkBeacon();
    bool lengthInSlots = false;
    bool data = false;
    bool random = false;

    m_phy->Send(packet, BuildPhyControlFieldType1(lengthInSlots, packet->GetSize()), data, random);
}

void
Dect2020ExtMac::SendClusterBeaconOnChannel(uint16_t channelNumber)
{
    // Sanity check to assert that the device is currently not transmmitting another packet
    if (m_phy->GetState() == PhyState::TX)
    {
        NS_FATAL_ERROR("Device is already transmitting a packet while attempting to transmit another packet");
    }

    NS_LOG_INFO(MAC_IDENTITY << ": Sending Cluster Beacon (channel=" << m_operatingChannelNumber << ",slot=" << m_macScheduler->GetCurrentSlotNumber() << ",subslot=" << m_macScheduler->GetCurrentSubslotNumber() << ")");

    Ptr<Packet> packet = BuildClusterBeacon(channelNumber);
    bool data = false;
    bool random = false;
    bool lengthInSlots = false;

    m_phy->Send(packet, BuildPhyControlFieldType1(lengthInSlots, packet->GetSize()), data, random);
}

Ptr<Packet>
Dect2020ExtMac::BuildNetworkBeacon()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> beacon = Create<Packet>();
    
    // Network Beacon Message Ie
    Dect2020ExtNetworkBeaconMessageIe networkBeaconMessageIe;
    networkBeaconMessageIe.SetTxPowerIncluded(false);
    networkBeaconMessageIe.SetPowerConstraints(false);
    networkBeaconMessageIe.SetCurrentClusterChannelIncluded(false);
    networkBeaconMessageIe.SetNetworkBeaconChannels(0);
    networkBeaconMessageIe.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_100MS);
    networkBeaconMessageIe.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_50MS);
    networkBeaconMessageIe.SetNextClusterChannel(m_operatingChannelNumber);

    // Calculate time to next Cluster Beacon
    // Get absolute time offsets for each beacon within a frame
    double networkBeaconTimeNs = m_macScheduler->GetSubslotStartTimeOffsetNs(m_networkBeaconSubslot);
    double clusterBeaconTimeNs = m_macScheduler->GetSubslotStartTimeOffsetNs(m_clusterBeaconSubslot);

    // Calculate the offset between them
    double offsetTimeNs = clusterBeaconTimeNs - networkBeaconTimeNs;

    auto timeToNextClusterBeaconUs = static_cast<uint32_t>(offsetTimeNs / 1000.0);

    // NS_LOG_INFO(MAC_IDENTITY << ": time to next cluster beacon " << timeToNextClusterBeaconUs << "us");

    networkBeaconMessageIe.SetTimeToNext(timeToNextClusterBeaconUs);

    beacon->AddHeader(networkBeaconMessageIe);

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the Ie Header
    mxHeaderTypeA.SetLengthField(false); // No length field included in the Ie Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    beacon->AddHeader(mxHeaderTypeA);

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(m_networkId);
    beaconHeader.SetTransmitterAddress(m_longRadioDeviceId);

    beacon->AddHeader(beaconHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    beacon->AddHeader(macHeaderType);

    return beacon;
}

Ptr<Packet>
Dect2020ExtMac::BuildClusterBeacon(uint16_t clusterChannelNumber)
{
    NS_LOG_FUNCTION(this);
    Ptr<Packet> beacon = Create<Packet>();

    // Cluster Beacon Message Ie
    Dect2020ExtClusterBeaconMessageIe clusterBeaconMessageIe;
    clusterBeaconMessageIe.SetSFN(m_macScheduler->GetSfn());
    clusterBeaconMessageIe.SetTxPowerIncluded(false);
    clusterBeaconMessageIe.SetPowerConstraints(false);
    clusterBeaconMessageIe.SetFrameOffsetIncluded(false);
    clusterBeaconMessageIe.SetNextChannelIncluded(false);
    clusterBeaconMessageIe.SetTimeToNextFieldPresent(false);
    clusterBeaconMessageIe.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS);
    clusterBeaconMessageIe.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS);
    clusterBeaconMessageIe.SetCountToTrigger(15);
    clusterBeaconMessageIe.SetRelQuality(0);
    clusterBeaconMessageIe.SetMinQuality(0);

    beacon->AddHeader(clusterBeaconMessageIe);

    // Cluster beacon message is always followed by the Random Access Ressource Ie
    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe = BuildRandomAccessResourceIe();
    beacon->AddHeader(randomAccessResourceIe);

    // MAC Multiplexing Header Type A
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the Ie Header
    mxHeaderTypeA.SetLengthField(false); // No length field included in the Ie Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::CLUSTER_BEACON_MESSAGE);

    beacon->AddHeader(mxHeaderTypeA);

    // Beacon Header
    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(m_networkId);
    beaconHeader.SetTransmitterAddress(m_longRadioDeviceId);
    beacon->AddHeader(beaconHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);
    beacon->AddHeader(macHeaderType);

    return beacon;
}

Dect2020ExtRandomAccessResourceIe
Dect2020ExtMac::BuildRandomAccessResourceIe()
{
    if (m_configuredRandomAccessResourceIeSet)
    {
        return m_configuredRandomAccessResourceIe;
    }
    else
    {
        NS_FATAL_ERROR("No Random Access Resource IE provided for MAC");
    }
}

Dect2020ExtPhyControlFieldType1
Dect2020ExtMac::BuildPhyControlFieldType1(bool packetLengthType, uint32_t packetLengthInBytes)
{
    Dect2020ExtPhyControlFieldType1 physicalHeaderField;
    physicalHeaderField.SetPacketLengthType(packetLengthType); // 0 = in Subslots, 1 = in Slots

    if (packetLengthType == 0) // Packet length is given in subslots
    {
        physicalHeaderField.SetPacketLength(CalculateMacPduSizeInSubslots(packetLengthInBytes));
    }
    else if (packetLengthType == 1) // Packet length is given in slots
    {
        physicalHeaderField.SetPacketLength(CalculateMacPduSizeInSlots(packetLengthInBytes)); // Size of packet in slots
    }

    physicalHeaderField.SetShortNetworkId(m_networkId);
    physicalHeaderField.SetTransmitterIdentity(m_shortRadioDeviceId);
    physicalHeaderField.SetTransmitPower(10); // dBm -- device power class 1
    physicalHeaderField.SetDFMCS(m_mcs);

    return physicalHeaderField;
}

Dect2020ExtPhyControlFieldType2
Dect2020ExtMac::BuildPhyControlFieldType2(bool packetLengthType, uint32_t packetSizeInBytes, bool ndi, uint8_t harqProcessNumber)
{
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2(Dect2020ExtPhyControlFieldType2::PhyControlFieldType2Format::FORMAT_000);
    phyControlFieldType2.SetPacketLengthType(packetLengthType);

    if (packetLengthType) // length in slots
    {
        phyControlFieldType2.SetPacketLength(CalculateMacPduSizeInSlots(packetSizeInBytes));
    }
    else // length in subslots
    {
        phyControlFieldType2.SetPacketLength(CalculateMacPduSizeInSubslots(packetSizeInBytes));
    }

    phyControlFieldType2.SetShortNetworkId(0); // currently not used
    phyControlFieldType2.SetTransmitterIdentity(m_shortRadioDeviceId);
    phyControlFieldType2.SetTransmitPower(10);
    phyControlFieldType2.SetDfmcs(m_mcs);
    phyControlFieldType2.SetReceiverIdentity(m_selectedFtCandidate.shortFtId);
    phyControlFieldType2.SetNumberOfSpatialStreams(0); // currently not used
    phyControlFieldType2.SetDfRedundancyVersion(0);
    phyControlFieldType2.SetDfNewDataIndication(ndi);
    phyControlFieldType2.SetDfHarqProcessNumber(harqProcessNumber);
    phyControlFieldType2.SetFeedbackFormat(1);
    phyControlFieldType2.SetFeedbackInfo(0);

    return phyControlFieldType2;
}

Ptr<Packet>
Dect2020ExtMac::BuildAssociationRequest(Dect2020ExtFtInfo* ft)
{
    Ptr<Packet> packet = Create<Packet>();

    // --- Association Control Ie ---
    packet->AddHeader(m_configuredAssociationControlIe);

    // --- RD Capability Ie ---
    Dect2020ExtRdCapabilityIe rdCapabilityIe;
    rdCapabilityIe.SetRelease(this->m_device->m_release);
    rdCapabilityIe.SetGroupAssignment(this->m_device->m_supportGroupAssignment);
    rdCapabilityIe.SetPaging(this->m_device->m_supportPaging);
    rdCapabilityIe.SetOperatingModes(this->m_device->m_operatingModes);
    rdCapabilityIe.SetMesh(this->m_device->m_mesh);
    rdCapabilityIe.SetScheduledAccessDataTransfer(this->m_device->m_scheduledAccessDataTransfer);
    rdCapabilityIe.SetMacSecurity(this->m_device->m_macSecurity);
    rdCapabilityIe.SetDlcServiceType(this->m_device->m_dlcServiceType);
    rdCapabilityIe.SetRdPowerClass(this->m_device->m_rdPowerClass);
    rdCapabilityIe.SetMaxNssFoRx(this->m_device->m_maxNssFoRx);
    rdCapabilityIe.SetRxForTxDiversity(this->m_device->m_rxForTxDiversity);
    rdCapabilityIe.SetRxGain(this->m_device->m_rxGain);
    rdCapabilityIe.SetMaxMcs(this->m_device->m_maxMcs);
    rdCapabilityIe.SetSoftBufferSize(this->m_device->m_softBufferSize);
    rdCapabilityIe.SetNumOfHarqProcesses(this->m_device->m_numOfHarqProcesses);
    rdCapabilityIe.SetHarqFeedbackDelay(this->m_device->m_harqFeedbackDelay);
    rdCapabilityIe.SetDDelay(this->m_device->m_dDelay);
    rdCapabilityIe.SetHalfDulp(this->m_device->m_halfDulp);

    packet->AddHeader(rdCapabilityIe);

    // --- Association Request Message IE ---
    Dect2020ExtAssociationRequestMessageIe associationRequestMessage;
    associationRequestMessage.SetSetupCause(0); // Initial association
    associationRequestMessage.SetNumberOfFlows(0);
    associationRequestMessage.SetPowerConstraints(false);
    associationRequestMessage.SetFtMode(false);
    associationRequestMessage.SetCurrent(false);
    associationRequestMessage.SetHarqProcessesTx(0);
    associationRequestMessage.SetMaxHarqReTxDelay(0);
    associationRequestMessage.SetHarqProcessesRx(0);
    associationRequestMessage.SetMaxHarqReRxDelay(0);
    associationRequestMessage.SetFlowId(0);

    packet->AddHeader(associationRequestMessage);

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA muxHeader;
    muxHeader.SetMacExtensionFieldEncoding(0);
    muxHeader.SetLengthField(false);
    muxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE);

    packet->AddHeader(muxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(false);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(ft->longFtId);
    unicastHeader.SetTransmitterAddress(this->GetLongRadioDeviceId());

    packet->AddHeader(unicastHeader);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER);

    packet->AddHeader(macHeaderType);

    return packet;
}

Ptr<Packet>
Dect2020ExtMac::BuildAssociationResponse(Dect2020ExtPtInfo ptInfo)
{
    Ptr<Packet> packet = Create<Packet>();

    // --- RD Capability IE ---
    Dect2020ExtRdCapabilityIe rdCapabilityIe;
    rdCapabilityIe.SetRelease(this->m_device->m_release);
    rdCapabilityIe.SetGroupAssignment(this->m_device->m_supportGroupAssignment);
    rdCapabilityIe.SetPaging(this->m_device->m_supportPaging);
    rdCapabilityIe.SetOperatingModes(this->m_device->m_operatingModes);
    rdCapabilityIe.SetMesh(this->m_device->m_mesh);
    rdCapabilityIe.SetScheduledAccessDataTransfer(this->m_device->m_scheduledAccessDataTransfer);
    rdCapabilityIe.SetMacSecurity(this->m_device->m_macSecurity);
    rdCapabilityIe.SetDlcServiceType(this->m_device->m_dlcServiceType);
    rdCapabilityIe.SetRdPowerClass(this->m_device->m_rdPowerClass);
    rdCapabilityIe.SetMaxNssFoRx(this->m_device->m_maxNssFoRx);
    rdCapabilityIe.SetRxForTxDiversity(this->m_device->m_rxForTxDiversity);
    rdCapabilityIe.SetRxGain(this->m_device->m_rxGain);
    rdCapabilityIe.SetMaxMcs(this->m_device->m_maxMcs);
    rdCapabilityIe.SetSoftBufferSize(this->m_device->m_softBufferSize);
    rdCapabilityIe.SetNumOfHarqProcesses(this->m_device->m_numOfHarqProcesses);
    rdCapabilityIe.SetHarqFeedbackDelay(this->m_device->m_harqFeedbackDelay);
    rdCapabilityIe.SetDDelay(this->m_device->m_dDelay);
    rdCapabilityIe.SetHalfDulp(this->m_device->m_halfDulp);

    packet->AddHeader(rdCapabilityIe);

    // --- Association Response Message IE ---
    Dect2020ExtAssociationResponseMessageIe associationResponseMessage;
    associationResponseMessage.SetAssociationAccepted(true);
    associationResponseMessage.SetHarqMod(false); // HARQ configuration accepted as configured in the Association Request
    associationResponseMessage.SetNumberOfFlows(0); // Currently not used
    associationResponseMessage.SetGroupIdAndResourceTagIncluded(false); // Currently not used

    packet->AddHeader(associationResponseMessage);

    // --- MAC Multiplexing Header Type A ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    mxHeader.SetMacExtensionFieldEncoding(0);
    mxHeader.SetLengthField(false);
    mxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE);

    packet->AddHeader(mxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(false);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(ptInfo.unicastHeader.GetTransmitterAddress());
    unicastHeader.SetTransmitterAddress(this->GetLongRadioDeviceId());

    packet->AddHeader(unicastHeader);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER);

    packet->AddHeader(macHeaderType);

    return packet;
}

uint32_t
Dect2020ExtMac::ConvertNetworkBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtNetworkBeaconPeriod period)
{
    switch (period)
    {
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS: return 5; // 50ms / 10ms = 5 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_100MS: return 10; // 100ms / 10ms = 10 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_500MS: return 50; // 500ms / 10ms = 50 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_1000MS: return 100; // 1000ms / 10ms = 100 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_1500MS: return 150; // 1500ms / 10ms = 150 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_2000MS: return 200; // 2000ms / 10ms = 200 frames
        case Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_4000MS: return 400; // 4000ms / 10ms = 400 frames
        default:
            NS_FATAL_ERROR("Invalid network beacon period");
            return 10; // fallback
    }
}

uint32_t
Dect2020ExtMac::ConvertClusterBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIe::Dect2020ExtClusterBeaconPeriod period)
{
    switch (period)
    {
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_10MS: return 1; // 10ms / 10ms = 1 frame
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_50MS: return 5; // 50ms / 10ms = 5 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS: return 10; // 100ms / 10ms = 10 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_500MS: return 50; // 500ms / 10ms = 50 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_1000MS: return 100; // 1000ms / 10ms = 100 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_1500MS: return 150; // 1500ms / 10ms = 150 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_2000MS: return 200; // 2000ms / 10ms = 200 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_4000MS: return 400; // 4000ms / 10ms = 400 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_8000MS: return 800; // 8000ms / 10ms = 800 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_16000MS: return 1600; // 16000ms / 10ms = 1600 frames
        case Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_32000MS: return 3200; // 32000ms / 10ms = 3200 frames
        default:
            NS_FATAL_ERROR("Invalid cluster beacon period");
            return 1; // fallback
    }
}

uint8_t
Dect2020ExtMac::CalculateMacPduSizeInSubslots(uint32_t packetSizeInBytes) const
{
    double packetLengthInBits = packetSizeInBytes * 8;
    double mcsTransportBlockSizeBitsPerSlot = m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor, m_fourierTransformScalingFactor, m_mcs);

    uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();

    // Calculate transport block size per subslot
    double mcsTransportBlockSizeBitsPerSubslot = mcsTransportBlockSizeBitsPerSlot / subslotsPerSlot;

    // Calculate packet length in subslots (round up)
    uint8_t packetLengthInSubslots = std::ceil(packetLengthInBits / mcsTransportBlockSizeBitsPerSubslot);

    return packetLengthInSubslots;
}

uint8_t
Dect2020ExtMac::CalculateMacPduSizeInSlots(uint32_t packetSizeInBytes) const
{
    double packetLengthInBits = packetSizeInBytes * 8;
    double mcsTransportBlockSizeBitsPerSlot = m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor, m_fourierTransformScalingFactor, m_mcs);

    // Calculate packet length in slots (round up)
    uint8_t packetLengthInSlots = std::ceil(packetLengthInBits / mcsTransportBlockSizeBitsPerSlot);

    return packetLengthInSlots;
}

void
Dect2020ExtMac::InitiateHarq(bool data)
{
    Dect2020ExtHarqProcess* process = FindFreeHarqProcess();

    // The packet for transmission is taken from the front of the Tx queue
    Ptr<Packet> packet = m_txQueue.front();
    m_txQueue.pop();

    NS_LOG_INFO(MAC_IDENTITY << ": Dequeued packet");

    process->state = Dect2020ExtHarqProcess::WAITING_ACK;
    process->ndi = true; // New data with no previous versions
    process->packet = packet->Copy();
    process->data = data;

    bool packetLengthInSlots = false;
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2 = BuildPhyControlFieldType2(packetLengthInSlots, packet->GetSize(), process->ndi, process->processNumber);

    uint32_t currentSubslotNumber = m_macScheduler->GetCurrentSubslotNumber();
    uint16_t raraStartSubslot = m_selectedFtCandidate.randomAccessResourceIe.GetStartSubslot();
    uint16_t raraEndSubslot = raraStartSubslot + m_selectedFtCandidate.randomAccessResourceIe.GetRaraLength();

    // InitiateRandomAccess must always be scheduled either inside or at the start of RACH resources

    // If the RACH resources are yet to come in the frame -> wait till the RACH resources
    if (currentSubslotNumber < raraStartSubslot)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Executing Random Access in the current frame");

        double timeToRachStartNs = m_macScheduler->GetTimeTillSubslotStartNs(raraStartSubslot);
        NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the current frame are in " << timeToRachStartNs / 1000000000 << "s");
        Time delay = NanoSeconds(timeToRachStartNs);

        Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                            m_selectedFtCandidate.randomAccessResourceIe,
                            process,
                            phyControlFieldType2,
                            true,
                            std::shared_ptr<Dect2020ExtRandomAccessContext>(nullptr));
    }
    // If the device is already inside or at the start of the RACH resources -> execute InitiateRandomAccess immediately
    if (currentSubslotNumber >= raraStartSubslot && currentSubslotNumber < raraEndSubslot)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Executing Random Access immediately");
        InitiateRandomAccess(m_selectedFtCandidate.randomAccessResourceIe, process, phyControlFieldType2);
    }
    // If the device is outside of the RACH resources -> execute the random access at the next RACH resources in the next frame
    if (currentSubslotNumber >= raraEndSubslot)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Executing Random Access at the next RACH resources in the next frame");

        double timeToRachStartNs = m_macScheduler->GetTimeTillNextFrameNs() + m_macScheduler->GetSubslotStartTimeOffsetNs(m_selectedFtCandidate.randomAccessResourceIe.GetStartSubslot());
        NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the next frame are in " << timeToRachStartNs / 1000000 << " ms");
        Time delay = NanoSeconds(timeToRachStartNs);

        Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                            m_selectedFtCandidate.randomAccessResourceIe,
                            process,
                            phyControlFieldType2,
                            true,
                            std::shared_ptr<Dect2020ExtRandomAccessContext>(nullptr));
    }
}

Dect2020ExtHarqProcess*
Dect2020ExtMac::FindFreeHarqProcess()
{
    Dect2020ExtHarqProcess* process = nullptr;
    for (auto& p: m_harqProcesses)
    {
        if (p.state == Dect2020ExtHarqProcess::FREE)
        {
            process = &p;
            break;
        }
    }

    // There should always be a free process when this function is called
    // Currently, Stop-and-Wait ARQ protocol is implemented
    if (!process)
    {
        NS_FATAL_ERROR(MAC_IDENTITY << ": No free HARQ process");
    }

    return process;
}

Dect2020ExtHarqProcess*
Dect2020ExtMac::FindHarqProcess(uint8_t harqProcessNumber)
{
    // Find a HARQ process with the process number from the PHY Control Field Type 2
    Dect2020ExtHarqProcess* process = nullptr;
    for (auto& p: m_harqProcesses)
    {
        if (p.processNumber == harqProcessNumber)
        {
            process = &p;
            break;
        }
    }

    if (!process)
    {
        NS_FATAL_ERROR("HARQ process was not found");
    }

    return process;
}

void
Dect2020ExtMac::SendHarqAck(Ptr<Packet> ack, Dect2020ExtPhyControlFieldType2 phyCtrl)
{
    NS_LOG_FUNCTION(MAC_IDENTITY);
    NS_LOG_INFO(MAC_IDENTITY << ": Sending ACK");
    // m_phy->Send(ack, phyCtrl);
}

void
Dect2020ExtMac::HandleHarqAck(Ptr<Packet> ack, Dect2020ExtPhyControlFieldType2 phyCtrl)
{
    NS_LOG_FUNCTION(MAC_IDENTITY);
    // Find a HARQ process
    Dect2020ExtHarqProcess* proc = nullptr;
    for (auto& p: m_harqProcesses)
    {
        if (p.processNumber == phyCtrl.GetDfHarqProcessNumber())
        {
            proc = &p;
            break;
        }
    }

    if (!proc)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": No HARQ process found");
        return; // all processes in use - back-pressure to the tx queue
    }
   
    NS_LOG_INFO(MAC_IDENTITY << ": HARQ process " << (uint32_t)proc->processNumber << " ACK");

    // Reset the HARQ process
    proc->Reset();

    RestartKeepAlivePeriod();

    if (!m_txQueue.empty())
    {
        // InitiateHARQ();
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Tx queue empty, monitoring Tx queue");
        MonitorTxQueue();
    }
}

void
Dect2020ExtMac::VerifyHarqAck(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2)
{
    // Find a HARQ process with the process number from the PHY Control Field Type 2
    Dect2020ExtHarqProcess* process = nullptr;
    for (auto& p: m_harqProcesses)
    {
        if (p.processNumber == phyControlFieldType2.GetDfHarqProcessNumber())
        {
            process = &p;
            break;
        }
    }

    if (!process)
    {
        NS_FATAL_ERROR("HARQ process was not found");
        return;
    }

    // If HARQ ACK has not been already received -> retransmit with the same context, but increased CW
    if (process->state == Dect2020ExtHarqProcess::WAITING_ACK)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": received no ACK, retransmitting");

        // Exponential backoff for the same random access context:
        // double CW_CURRENT, but cap at CW_MAX
        uint32_t newCW = std::min(process->randomAccessContext->CW_CURRENT * 2, process->randomAccessContext->CW_MAX);
        process->randomAccessContext->CW_CURRENT = newCW;

        // Reset sample counter and sample vector for the next attempt
        process->randomAccessContext->lbtSampleCount = 0;
        process->randomAccessContext->lbtRssiSamples.clear();
      
        uint32_t currentSubslotNumber = m_macScheduler->GetCurrentSubslotNumber();
        uint16_t raraStartSubslot = m_selectedFtCandidate.randomAccessResourceIe.GetStartSubslot();
        uint16_t raraEndSubslot = raraStartSubslot + m_selectedFtCandidate.randomAccessResourceIe.GetRaraLength();
        bool newContext = false;

        // If the RACH resources are yet to come in the frame -> wait till the RACH resources
        if (currentSubslotNumber < raraStartSubslot)
        {
            NS_LOG_INFO(MAC_IDENTITY << ": Executing the Random Access Procedure in the current frame");

            double timeToRachStartNs = m_macScheduler->GetTimeTillSubslotStartNs(raraStartSubslot);
            NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the current frame are in " << timeToRachStartNs / 1000000000 << "s");
            Time delay = NanoSeconds(timeToRachStartNs);

            Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                                m_selectedFtCandidate.randomAccessResourceIe,
                                process,
                                phyControlFieldType2,
                                newContext,
                                process->randomAccessContext);
            return;
        }
        // If the device is already inside or at the start of the RACH resources -> execute the Random Access Procedure immediately
        if (currentSubslotNumber >= raraStartSubslot && currentSubslotNumber < raraEndSubslot)
        {
            InitiateRandomAccess(process->randomAccessContext->randomAccessResourceIe,
                                process,
                                phyControlFieldType2,
                                newContext,
                                process->randomAccessContext);
            return;
        }
        // If the device is outside of the RACH resources -> execute the random access at the next RACH resources in the next frame
        if (currentSubslotNumber >= raraEndSubslot)
        {
            NS_LOG_INFO(MAC_IDENTITY << ": Executing the Random Access Procedure in the next RACH resources in the next frame");

            double timeToRachStartNs = m_macScheduler->GetTimeTillNextFrameNs() + m_macScheduler->GetSubslotStartTimeOffsetNs(m_selectedFtCandidate.randomAccessResourceIe.GetStartSubslot());
            NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the next frame are in " << timeToRachStartNs / 1000000 << " ms");
            Time delay = NanoSeconds(timeToRachStartNs);

            Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                                m_selectedFtCandidate.randomAccessResourceIe,
                                process,
                                phyControlFieldType2,
                                newContext,
                                process->randomAccessContext);
            return;
        }
    }

    NS_LOG_INFO(MAC_IDENTITY << ": ACK verified");
}

void
Dect2020ExtMac::MonitorTxQueue()
{
    if (m_txQueue.empty())
    {
        Simulator::Schedule(MilliSeconds(1), &Dect2020ExtMac::MonitorTxQueue, this);
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Detected a new packet in Tx queue");
        // InitiateHARQ();
    }
}

void
Dect2020ExtMac::InitiateRandomAccess(Dect2020ExtRandomAccessResourceIe randomAccessResourceIe,
                                     Dect2020ExtHarqProcess* process,
                                     Dect2020ExtPhyControlFieldType2 phyControlFieldType2,
                                     bool newContext,
                                     std::shared_ptr<Dect2020ExtRandomAccessContext> previousContext)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Initiating Random Access Procedure");

    uint16_t startSubslot = randomAccessResourceIe.GetStartSubslot();
    bool lengthInSubslots = !randomAccessResourceIe.GetLengthType();
    uint8_t length = randomAccessResourceIe.GetRaraLength(); // RACH resources length

    // Calculate the total RACH resource duration in microseconds
    double rachDurationUs;
    if (lengthInSubslots)
    {
        rachDurationUs = m_macScheduler->GetTimeTillSubslotStartNs(startSubslot + length) / 1000.0; // Convert ns to us
    }
    else
    {
        // TBD --> Length in slots
        NS_FATAL_ERROR("WARNING: RACH resources length is in slots (not yet implemented)");
        return;
    }

    // Calculate all possible start positions
    // Start positions are at 0, 1, 2, 3... times (STF + GI)
    std::vector<double> startPositions;
    double currentPosition = 0.0;
    uint32_t positionIndex = 0;

    while (currentPosition < rachDurationUs)
    {
        startPositions.push_back(currentPosition);
        positionIndex++;
        currentPosition = positionIndex * START_POSITION_INTERVAL_US;
    }

    if (newContext)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Creating new context");

        // Calculate CW_MIN; CW_MIN = 8 * CWmin_sig
        uint32_t CW_MIN = 8 * randomAccessResourceIe.GetCwMinSig();

        // Calculate CW_MAX; CW_MAX = 256 * CWmax_sig
        uint32_t CW_MAX = 256 * randomAccessResourceIe.GetCwMaxSig();

        // Initialize and set CW_CURRENT to CW_MIN
        uint32_t CW_CURRENT = CW_MIN;

        std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext = std::make_shared<Dect2020ExtRandomAccessContext>();
        randomAccessContext->randomAccessResourceIe = randomAccessResourceIe;
        randomAccessContext->startPositions = startPositions;
        randomAccessContext->CW_MAX = CW_MAX;
        randomAccessContext->CW_MIN = CW_MIN;
        randomAccessContext->CW_CURRENT = CW_CURRENT;
        randomAccessContext->process = process;
        randomAccessContext->phyControlFieldType2 = phyControlFieldType2;

        process->randomAccessContext = randomAccessContext;

        InitiateBackoff(randomAccessContext);
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Using existing context");

        previousContext->startPositions = startPositions; // Update starting positions
        InitiateBackoff(previousContext);
    }
}

void
Dect2020ExtMac::InitiateBackoff(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    // Sanity check for configuration
    // Determine if MAC PDU fits into the remaining RACH resource subslots/slots
    bool fitsInRachResources = CheckIfMacPduFitsInMaxRandomAccessTxTime(randomAccessContext);

    if (!fitsInRachResources)
    {
        NS_FATAL_ERROR("Packet doesn't fit in Random Access Max Tx time");
    }

    NS_LOG_INFO(MAC_IDENTITY << ": Initiating backoff at subslot - " << m_macScheduler->GetCurrentSubslotNumber());

    // Pick a random backoff value uniformly distributed between 0 and CW_CURRENT
    uint32_t backoffValue = m_backoffRandomVariable->GetInteger(0, randomAccessContext->CW_CURRENT);

    // Ensure we don't exceed available positions
    if (backoffValue >= randomAccessContext->startPositions.size())
    {        
        NS_LOG_INFO(MAC_IDENTITY << ": Backoff target position " << backoffValue << " exceeds available positions (" << randomAccessContext->startPositions.size() << ")");

        // If all possible positions are exceeded ->
        // the device is no longer inside RACH resources and need to wait for them again in the next frame
        double timeToRachStartNs = m_macScheduler->GetTimeTillNextFrameNs() + m_macScheduler->GetSubslotStartTimeOffsetNs(randomAccessContext->randomAccessResourceIe.GetStartSubslot());
        NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the next frame are in " << timeToRachStartNs / 1000000 << " ms");
        Time delay = NanoSeconds(timeToRachStartNs);

        Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                            randomAccessContext->randomAccessResourceIe,
                            randomAccessContext->process,
                            randomAccessContext->phyControlFieldType2,
                            false,
                            randomAccessContext);
        return;
    }

    // Calculate backoff duration in microseconds
    double targetPositionTimeUs = randomAccessContext->startPositions[backoffValue];

    NS_LOG_INFO(MAC_IDENTITY << ": Random backoff value: " << backoffValue);
    NS_LOG_INFO(MAC_IDENTITY << ": Target position time: " << targetPositionTimeUs << "us");

    // Schedule the LBT (Listen Before Talk) after rachBackOff expires
    Simulator::Schedule(MicroSeconds(targetPositionTimeUs), &Dect2020ExtMac::PerformLbt, this, randomAccessContext);
}

void
Dect2020ExtMac::PerformLbt(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_DEBUG(MAC_IDENTITY << ": Performing LBT at subslot " << m_macScheduler->GetCurrentSubslotNumber());

    // Reset sampling context
    randomAccessContext->lbtSampleCount = 0;
    randomAccessContext->lbtRssiSamples.clear();
    randomAccessContext->lbtRssiSamples.reserve(5); // Reserve space for 5 samples

    // Define sampling points within START_POSITION_INTERVAL_US (STF + GI)
    std::vector<double> samplingOffsetsUs = {
        0.0, // Start of STF
        STF_US / 2.0, // Middle of STF
        STF_US, // Start of GI
        STF_US + (GI_US / 2.0), // Middle of GI
        STF_US + GI_US // End of GI
    };

    // Schedule RSSI sampling at each offset
    for (double offsetUs : samplingOffsetsUs)
    {
        Simulator::Schedule(MicroSeconds(offsetUs), &Dect2020ExtMac::SampleLbtRssi, this, randomAccessContext);
    }
}

void
Dect2020ExtMac::SampleLbtRssi(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_DEBUG(MAC_IDENTITY << ": Sampling LBT RSSI");

    // Get current channel number and calculate channel index
    uint16_t currentChannelNumber = m_operatingChannelNumber;
    uint16_t firstValidChannelNumber = m_channelManager->GetFirstValidChannelNumber(m_bandNumber);
    uint16_t channelIndex = currentChannelNumber - firstValidChannelNumber;

    // Sample channel power from PHY interference model
    double rssiChannelWatt = (*m_phy->m_interference->m_allSignals)[channelIndex];
    double rssiChannelDbm = m_channelManager->WToDbm(rssiChannelWatt);

    // Store the sample
    randomAccessContext->lbtRssiSamples.push_back(rssiChannelDbm);
    randomAccessContext->lbtSampleCount++;

    // If all samples collected, evaluate the channel
    if (randomAccessContext->lbtSampleCount >= randomAccessContext->lbtTotalSamples)
    {
        EvaluateLbtChannel(randomAccessContext);
    }
}

void
Dect2020ExtMac::EvaluateLbtChannel(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    // Sanity check to assert that the device is currently not transmmitting another packet
    if (m_phy->GetState() == PhyState::TX)
    {
        NS_FATAL_ERROR("Device is already transmitting a packet while attempting to transmit another packet");
    }

    bool rachTxFitsInRemainingResources = CheckIfRachTxFitsInRemainingResources(randomAccessContext);

    if (rachTxFitsInRemainingResources)
    {
        // Calculate average RSSI from all samples
        double totalRssi = 0.0;
        double maxRssi = -std::numeric_limits<double>::infinity();

        for (double rssi : randomAccessContext->lbtRssiSamples)
        {
            totalRssi += rssi;
            maxRssi = std::max(maxRssi, rssi);
        }

        double avgRssiDbm = totalRssi / randomAccessContext->lbtRssiSamples.size();

        // Determine channel state based on RSSI threshlods
        bool channelIsFree = (avgRssiDbm <= RSSI_THRESHOLD_MIN);
        bool channelIsPossible = (avgRssiDbm > RSSI_THRESHOLD_MIN && avgRssiDbm <= RSSI_THRESHOLD_MAX);
        bool channelIsBusy = (avgRssiDbm > RSSI_THRESHOLD_MAX);

        // If channel is free or possible -> transmit immediately
        if (channelIsFree || channelIsPossible)
        {
            // Transmit packet
            NS_LOG_INFO(MAC_IDENTITY << ": Transmitting" << " -> power on channel is " << avgRssiDbm << " dBm");

            bool random = true;
            m_phy->Send(randomAccessContext->process->packet, randomAccessContext->phyControlFieldType2, randomAccessContext->process->data, random);

            // Verify ACK after the RACH response window expires
            // Response window size is in subslots
            double subslotDurationNs = SLOT_DURATION_NS / m_channelManager->GetSubslotsPerSlot();
            Time delay = NanoSeconds(subslotDurationNs * randomAccessContext->randomAccessResourceIe.GetResponseWindow());
            Simulator::Schedule(delay, &Dect2020ExtMac::VerifyHarqAck, this, randomAccessContext->process->packet, randomAccessContext->phyControlFieldType2);
        }
        // If the channel is detectet busy or the PHY state became Rx right at the time
        // when this function is called -> re-initiate the Random Access Procedure
        else if (channelIsBusy || m_phy->GetState() == PhyState::RX)
        {
            NS_LOG_INFO(MAC_IDENTITY << ": Re-initiating Random Access Procedure with the same context");
            NS_LOG_INFO(MAC_IDENTITY << ": The PHY or the channel is busy (avg RSSI: " << avgRssiDbm << " dBm");

            // Reset sample counter and sample vector for the next attempt
            randomAccessContext->lbtSampleCount = 0;
            randomAccessContext->lbtRssiSamples.clear();

            // Re-initiate the Random Access Procedure with the same context
            bool newContext = false;
            InitiateRandomAccess(randomAccessContext->randomAccessResourceIe,
                                 randomAccessContext->process,
                                 randomAccessContext->phyControlFieldType2,
                                 newContext,
                                 randomAccessContext);
            return;
        }
    }
    else
    {
        // Defer till next RACH resources in the next frame
        NS_LOG_INFO(MAC_IDENTITY << ": MAC PDU and HARQ feedback don't fit into the remaining RACH resources. "
                                 << "Deferring random access till the next RACH resources in the next frame.");
        double timeToRachStartNs = m_macScheduler->GetTimeTillNextFrameNs() + m_macScheduler->GetSubslotStartTimeOffsetNs(randomAccessContext->randomAccessResourceIe.GetStartSubslot());
        NS_LOG_INFO(MAC_IDENTITY << ": Next RACH resources in the next frame are in " << timeToRachStartNs / 1000000 << " ms");
        Time delay = NanoSeconds(timeToRachStartNs);

        // Reset sample counter and sample vector for the next attempt
        randomAccessContext->lbtSampleCount = 0;
        randomAccessContext->lbtRssiSamples.clear();

        // Re-initiate the Random Access Procedure with the same context
        bool newContext = false;
        Simulator::Schedule(delay, &Dect2020ExtMac::InitiateRandomAccess, this,
                            randomAccessContext->randomAccessResourceIe,
                            randomAccessContext->process,
                            randomAccessContext->phyControlFieldType2,
                            newContext,
                            randomAccessContext);
        return;
    }
}

bool
Dect2020ExtMac::CheckIfMacPduFitsInMaxRandomAccessTxTime(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const
{
    bool lengthInSubslots = !randomAccessContext->randomAccessResourceIe.GetMaxRachLengthType();

    if (lengthInSubslots)
    {
        uint8_t packetLengthInSubslots = CalculateMacPduSizeInSubslots(randomAccessContext->process->packet->GetSize() + randomAccessContext->phyControlFieldType2.GetSerializedSize());
        NS_LOG_INFO(MAC_IDENTITY << ": MAC PDU size: " << (uint32_t)packetLengthInSubslots << " subslots");
        return packetLengthInSubslots <= randomAccessContext->randomAccessResourceIe.GetMaxRachLength();
    }
    else
    {
        uint8_t packetLengthInSlots = CalculateMacPduSizeInSlots(randomAccessContext->process->packet->GetSize() + randomAccessContext->phyControlFieldType2.GetSerializedSize());
        NS_LOG_INFO("MAC PDU size: " << (uint32_t)packetLengthInSlots << " slots");
        return packetLengthInSlots <= randomAccessContext->randomAccessResourceIe.GetMaxRachLength();
    }
}

bool
Dect2020ExtMac::CheckIfRachTxFitsInRemainingResources(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const
{
    uint32_t currentAbsSubslot = m_macScheduler->GetCurrentSubslotNumber();
    uint32_t macPduSizeInSubslots = CalculateMacPduSizeInSubslots(randomAccessContext->process->packet->GetSize() + randomAccessContext->phyControlFieldType2.GetSerializedSize());
    uint32_t responseAbsSubslot = currentAbsSubslot + 3; // Formula from the standard
    uint16_t startSubslot = randomAccessContext->randomAccessResourceIe.GetStartSubslot();
    uint8_t length = randomAccessContext->randomAccessResourceIe.GetRaraLength();

    return responseAbsSubslot + macPduSizeInSubslots <= startSubslot + length;
}

void
Dect2020ExtMac::ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm, bool data, bool random)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Received packet from PHY with RSSI " << rssiDbm << " dBm on channel " << m_operatingChannelNumber);

    Dect2020ExtPhyControlFieldType1 phyControlFieldType1;
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2;

    // Unicast and Data packets are sent with PHY Control Field Type 2 during Random Access
    if (random)
    {
        packet->RemoveHeader(phyControlFieldType2);
    }
    // Beacon packets are sent with PHY Control Field Type 1
    else
    {
        packet->RemoveHeader(phyControlFieldType1);
    }

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    packet->RemoveHeader(macHeaderType);
    MacHeaderTypeField receivedMacHeaderTypeField = macHeaderType.GetMacHeaderTypeField();

    // Either Network or Cluster Beacon (only PT devices process them)
    if (receivedMacHeaderTypeField == MacHeaderTypeField::BEACON_HEADER && m_terminationPointType == Dect2020ExtNetDevice::PT)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Beacon packet");
        if (m_proceedToAssociationRequest)
        {
            // Create FT Candidate Info
            // FT Candidate Info can be updated if the candidate has sent a beacon message before
            Dect2020ExtFtInfo* ft = FindOrCreateFtCandidate(phyControlFieldType1.GetTransmitterIdentity());
            ft->rssiDbm = rssiDbm;
            ft->phyControlFieldType1 = phyControlFieldType1;
            ft->shortFtId = phyControlFieldType1.GetTransmitterIdentity();
            ft->shortNetworkId = phyControlFieldType1.GetShortNetworkId();

            HandleBeaconPacket(packet, ft);
        }
        else
        {
            return;
        }
    }
    
    // Either Association Request or Response (must be acknowledged for according HARQ process)
    else if (receivedMacHeaderTypeField == MacHeaderTypeField::UNICAST_HEADER)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Unicast packet");
        HandleUnicastPacket(packet, phyControlFieldType2);
    }

    // Data packet used for data transmission (must be acknowledged for according HARQ process)
    else if (macHeaderType.GetMacHeaderTypeField() == Dect2020ExtMacHeaderType::MacHeaderTypeField::DATA_MAC_PDU_HEADER)
    {
        HandleDataPacket(packet, phyControlFieldType2);
    }

    // Forward the packet to the net device for minimal implementation
    m_device->Receive(packet);
}

void
Dect2020ExtMac::HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtInfo* ft)
{
    // --- Beacon Header ---
    Dect2020ExtBeaconHeader beaconHeader;
    packet->RemoveHeader(beaconHeader);

    ft->beaconHeader = beaconHeader;
    ft->networkId = beaconHeader.GetNetworkId();
    ft->longFtId = beaconHeader.GetTransmitterAddress();

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    packet->RemoveHeader(mxHeader);

    Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding ieType = mxHeader.GetIeTypeFieldEncoding();

    // --- Network Beacon Message ---
    if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::NETWORK_BEACON_MESSAGE)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Network Beacon Message");

        // --- Network Beacon Message Ie ---
        Dect2020ExtNetworkBeaconMessageIe networkBeaconMessage;
        packet->RemoveHeader(networkBeaconMessage);
        ft->networkBeaconMessageIe = networkBeaconMessage;

        // Get the cluster channel id from the network beacon message
        uint16_t ftBeaconNextClusterChannel = networkBeaconMessage.GetNextClusterChannel();
        NS_LOG_INFO(MAC_IDENTITY << ": Next cluster channel in network beacon: " << ftBeaconNextClusterChannel);
        ft->clusterChannelNumber = ftBeaconNextClusterChannel;

        // Get time to next cluster beacon from the network beacon message
        uint32_t timeToNextCluster = networkBeaconMessage.GetTimeToNext();
        NS_LOG_INFO(MAC_IDENTITY << ": Time to next cluster beacon: " << timeToNextCluster << " microseconds");
    }

    // --- Cluster Beacon Message ---
    else if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::CLUSTER_BEACON_MESSAGE)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Cluster Beacon Message");

        // if the PT has received a Cluster Beacon and no Network Beacon before, we assume the
        // cluster channel is the current channel
        if (ft->networkBeaconMessageIe.GetNextClusterChannel() == 0)
        {
            NS_LOG_INFO(MAC_IDENTITY << ": No Network Beacon Message received before, setting cluster channel to current channel");
            ft->networkBeaconMessageIe.SetNextClusterChannel(this->m_operatingChannelNumber);
            ft->clusterChannelNumber = this->m_operatingChannelNumber;
        }

        Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
        packet->RemoveHeader(randomAccessResourceIe);
        ft->randomAccessResourceIe = randomAccessResourceIe;

        Dect2020ExtClusterBeaconMessageIe clusterBeaconMessage;
        packet->RemoveHeader(clusterBeaconMessage);
        ft->clusterBeaconMessageIe = clusterBeaconMessage;

        EvaluateClusterBeacon(clusterBeaconMessage, randomAccessResourceIe, ft);
    }
}

void
Dect2020ExtMac::HandleUnicastPacket(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 phyControlFieldType2)
{
    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    packet->RemoveHeader(unicastHeader);

    // Check if the Unicast Message is for this device
    if (!PacketIsForThisDevice(unicastHeader))
    {
        NS_LOG_INFO(MAC_IDENTITY << ": The unicast packet is not for this device");
        return;
    }

    // --- MAC Multiplexing Header Type A ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA;
    packet->RemoveHeader(mxHeaderTypeA);

    IeTypeFieldEncoding ieType = mxHeaderTypeA.GetIeTypeFieldEncoding();

    // Message is an Association Request (only FT devices can process them)
    if (ieType == IeTypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE && m_terminationPointType == Dect2020ExtNetDevice::FT)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Association Request");

        if (m_proceedToAssociationResponse)
        {
            // --- Association Request Message IE ---
            Dect2020ExtAssociationRequestMessageIe associationRequestMessageIe;
            packet->RemoveHeader(associationRequestMessageIe);

            // --- RD Capability IE ---
            Dect2020ExtRdCapabilityIe rdCapabilityIe;
            packet->RemoveHeader(rdCapabilityIe);

            // --- Association Control IE ---
            Dect2020ExtAssociationControlIe associationControlIe;
            packet->RemoveHeader(associationControlIe);

            // Create PT Info
            Dect2020ExtPtInfo ptInfo;
            ptInfo.associationAcceptedTime = Simulator::Now();
            ptInfo.phyControlFieldType2 = phyControlFieldType2;
            ptInfo.unicastHeader = unicastHeader;
            ptInfo.associationRequestMessageIe = associationRequestMessageIe;
            ptInfo.rdCapabilityIe = rdCapabilityIe;
            ptInfo.associationControlIe = associationControlIe;

            ProcessAssociationRequest(ptInfo);
            return;
        }
        else
        {
            return;
        }
    }
    // Message is an Association Response (only PT devices can process them)
    if (ieType == IeTypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE && m_terminationPointType == Dect2020ExtNetDevice::PT)
    {
        // Sanity check to assert that m_selectedFtCandidate
        // is the one who sent the Association Response
        if (m_selectedFtCandidate.phyControlFieldType1.GetTransmitterIdentity() != phyControlFieldType2.GetTransmitterIdentity())
        {
            NS_FATAL_ERROR("FT sent an Association Response to the wrong PT");
        }

        // --- Association Response Message IE ---
        Dect2020ExtAssociationResponseMessageIe associationResponseMessageIe;
        packet->RemoveHeader(associationResponseMessageIe);

        // --- RD Capability IE ---
        Dect2020ExtRdCapabilityIe rdCapabilityIe;
        packet->RemoveHeader(rdCapabilityIe);

        // FindFtCandidate asserts that the FT exists
        Dect2020ExtFtInfo* ftInfo = FindFtCandidate(phyControlFieldType2.GetTransmitterIdentity());
        ftInfo->phyControlFieldType2 = phyControlFieldType2;
        ftInfo->unicastHeader = unicastHeader;
        ftInfo->associationResponseMessageIe = associationResponseMessageIe;
        ftInfo->rdCapabilityIe = rdCapabilityIe;

        ProcessAssociationResponse(ftInfo);
        return;
    }
}

void
Dect2020ExtMac::HandleDataPacket(Ptr<Packet> packet, Dect2020ExtPhyControlFieldType2 physicalHeaderFieldType2)
{
    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    packet->RemoveHeader(unicastHeader);

    // Check if the Unicast Message is for this device
    if (!PacketIsForThisDevice(unicastHeader))
    {
        NS_LOG_INFO(MAC_IDENTITY << ": The packet is not for this device");
        return;
    }

    uint32_t transmitterAddress = unicastHeader.GetTransmitterAddress();

    NS_LOG_INFO(MAC_IDENTITY << ": Received DATA MAC PDU");

    // Only PT devices can receive ACK for DATA MAC PDU
    if (m_terminationPointType == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received HARQ ACK");
        HandleHarqAck(packet, physicalHeaderFieldType2);
        return;
    }

    uint32_t targetId = transmitterAddress;

    auto it = std::find_if(m_associatedPtDevices.begin(),
                        m_associatedPtDevices.end(),
                        [targetId](const Dect2020ExtPtInfo& pt)
                        {
                            return pt.unicastHeader.GetTransmitterAddress() == targetId;
                        });

    if (it != m_associatedPtDevices.end())
    {
        // found — *it is the Dect2020ExtAssociatedPtInfo
        NS_LOG_INFO(MAC_IDENTITY << ": Found PT with longRdId " << it->unicastHeader.GetTransmitterAddress());

        // calculate the time to send the association response. DECT_Delay == 0 --> subslot n + HARQ_feedback_delay +1
        uint32_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;
        uint32_t currentAbsSubslot = m_macScheduler->GetCurrentSubslotNumber();

        uint8_t harqDelay = it->rdCapabilityIe.GetHarqFeedbackDelay();
        uint32_t responseAbsSubslot;

        if (m_dectDelay == 0)
        {
            responseAbsSubslot = currentAbsSubslot + harqDelay + 1;
            NS_LOG_INFO(MAC_IDENTITY << ": HARQ Delay: " << (uint32_t)harqDelay);
            NS_LOG_INFO(MAC_IDENTITY << ": Absolute HARQ ACK subslot: " << responseAbsSubslot);
            NS_LOG_INFO(MAC_IDENTITY << ": Current absolute subslot: " << currentAbsSubslot);
        }
        else
        {
            responseAbsSubslot = currentAbsSubslot + (subslotsPerFrame / 2); // 0.5 frame later
        }

        // Wrap into the current frame's subslot range so the scheduler vector lookup
        // stays in bounds. GetTimeTillSubslotStartNs already handles the case where
        // the wrapped subslot has already passed in the current frame by adding one
        // full frame duration, so cross-frame responses are scheduled correctly.
        uint32_t responseSubslotInFrame = responseAbsSubslot % subslotsPerFrame;

        NS_LOG_INFO(MAC_IDENTITY << ": Current subslot " << currentAbsSubslot);
        NS_LOG_INFO(MAC_IDENTITY << ": HARQ ACK absolute subslot " << responseAbsSubslot
                                 << " (in-frame: " << responseSubslotInFrame << ")");

        double t = m_macScheduler->GetTimeTillSubslotStartNs(responseSubslotInFrame);

        Ptr<Packet> ack = Create<Packet>();

        Dect2020ExtUnicastHeader unicastHeader;
        unicastHeader.SetReset(0);
        unicastHeader.SetMacSequence(0);
        unicastHeader.SetSequenceNumber(0);
        unicastHeader.SetReceiverAddress(it->unicastHeader.GetTransmitterAddress());
        unicastHeader.SetTransmitterAddress(m_longRadioDeviceId);

        ack->AddHeader(unicastHeader);

        Dect2020ExtMacHeaderType macHeaderType;
        macHeaderType.SetVersion(0);
        macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
        macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::DATA_MAC_PDU_HEADER);

        ack->AddHeader(macHeaderType);

        Dect2020ExtPhyControlFieldType2 phyCtrl(Dect2020ExtPhyControlFieldType2::PhyControlFieldType2Format::FORMAT_000);
        phyCtrl.SetPacketLengthType(true); // length in slots
        phyCtrl.SetPacketLength(CalculateMacPduSizeInSlots(ack->GetSize()));
        phyCtrl.SetShortNetworkId(0); // currently not used
        phyCtrl.SetTransmitterIdentity(m_shortRadioDeviceId);
        phyCtrl.SetTransmitPower(23);
        phyCtrl.SetDfmcs(m_mcs);
        phyCtrl.SetReceiverIdentity(0); // not used
        phyCtrl.SetNumberOfSpatialStreams(0); // currently not used
        phyCtrl.SetDfRedundancyVersion(0); // currently not used
        phyCtrl.SetDfNewDataIndication(0); // currently not used
        phyCtrl.SetDfHarqProcessNumber(physicalHeaderFieldType2.GetDfHarqProcessNumber()); // currently not used
        phyCtrl.SetFeedbackFormat(1);
        phyCtrl.SetFeedbackInfo(1);

        Simulator::Schedule(NanoSeconds(t), &Dect2020ExtMac::SendHarqAck, this, ack, phyCtrl);
    }
    else
    {
        NS_LOG_WARN(MAC_IDENTITY << ": PT with longRdId " << targetId << " not found");
    }
}

bool
Dect2020ExtMac::PacketIsForThisDevice(Dect2020ExtUnicastHeader unicastHeader)
{
    return unicastHeader.GetReceiverAddress() == GetLongRadioDeviceId();
}

Dect2020ExtFtInfo*
Dect2020ExtMac::FindOrCreateFtCandidate(uint16_t shortFtId)
{
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].shortFtId == shortFtId)
        {
            return &m_ftCandidates[idx];
        }
    }

    // No candidate found, create one
    Dect2020ExtFtInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

Dect2020ExtFtInfo*
Dect2020ExtMac::FindFtCandidate(uint16_t shortFtId)
{
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].shortFtId == shortFtId)
        {
            return &m_ftCandidates[idx];
        }
    }

    // No candidate found -> raise fatal error
    NS_FATAL_ERROR("No FT candidate found");
}

void
Dect2020ExtMac::EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIe& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIe& randomAccessResourceIe, Dect2020ExtFtInfo* ft)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Evaluating Cluster Beacon");
    ft->sfn = clusterBeaconMessage.GetSFN();
    m_lastSfn = clusterBeaconMessage.GetSFN();

    // Abort if the device status is ASSOCIATED --> The device is already associated to an FT
    if (m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATED)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Device is already associated");
        return;
    }

    // if device is not associated, switch the status to ASSOCIATION_PREPARING and start the "select
    // best FT" timer
    if (m_associationStatus == Dect2020ExtAssociationStatus::NOT_ASSOCIATED)
    {
        // The time in milliseconds for the PT to listen to other channels to discover potential FT candidates
        Time searchBestFtTimeMs;
        if (m_searchBestFtTimeMs != 0)
        {
            searchBestFtTimeMs = MilliSeconds(m_searchBestFtTimeMs);
        }
        else
        {
            searchBestFtTimeMs = MilliSeconds(2000);
        }
        
        ChangeAssociationStatus(ASSOCIATION_PREPARING);
        NS_LOG_INFO(MAC_IDENTITY << ": Discovering networks to find all possible candidates");
        DiscoverNetworks();
        Simulator::Schedule(searchBestFtTimeMs, &Dect2020ExtMac::SelectBestFtCandidate, this);
    }

    // Prepare Association Request --> The device is waiting fot the cluster Beacon of the selected FT
    if (m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT && m_selectedFtCandidate.longFtId == ft->longFtId)
    {
        // Schedule the Association Request
        Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendAssociationRequest, this, ft);
    }
}

void
Dect2020ExtMac::SelectBestFtCandidate()
{
    // Step 1: Find the FT with the best (highest) RSSI-2
    double bestRssiDbm = -150.0; // start with very low RSSI
    Dect2020ExtFtInfo bestCandidate;
    bool found = false;

    for (const auto& ft : m_ftCandidates)
    {
        if (ft.rssiDbm > bestRssiDbm)
        {
            bestRssiDbm = ft.rssiDbm;
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate found with RSSI " << bestRssiDbm << " dBm");
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate long ID: " << ft.longFtId);
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate network ID: " << ft.networkId);
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate cluster channel ID: " << ft.clusterChannelNumber);
            bestCandidate = ft;
            found = true;
        }
    }

    if (!found)
    {
        NS_LOG_WARN("No valid FT candidate found. Staying in NOT_ASSOCIATED state.");
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
        return;
    }

    // Step 2: Save the best FT info
    m_selectedFtCandidate = bestCandidate;

    // Step 3: Switch to the FT's channel
    NS_LOG_INFO(MAC_IDENTITY << ": Switching to best FT's channel: " << bestCandidate.clusterChannelNumber);
    m_operatingChannelNumber = bestCandidate.clusterChannelNumber;

    // Step 4: Set status and wait for next cluster beacon of selected FT
    ChangeAssociationStatus(WAITING_FOR_SELECTED_FT);

    // Cluster Beacon triggers next step in EvaluateClusterBeacon

    // Verify after 2 Seconds if the device is still waiting for the selected FT. This should not
    // be the case because it should have received a Cluster Beacon and sent an Association Request
    // and the status is either associated or association pending.
    // Simulator::Schedule(Seconds(2),
    //                     &Dect2020ExtMac::VerifyWaitingForSelectedFtAssociationStatus,
    //                     this);
}

void
Dect2020ExtMac::VerifyWaitingForSelectedFtAssociationStatus()
{
    if (this->m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT)
    {
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
}

void
Dect2020ExtMac::SendAssociationRequest(Dect2020ExtFtInfo* ft)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Preparing to send Association Request");

    ChangeAssociationStatus(ASSOCIATION_PENDING);

    m_txQueue.push(BuildAssociationRequest(ft));

    bool data = false;
    InitiateHarq(data);
}

void
Dect2020ExtMac::VerifyPendingAssociationStatus()
{
    if (this->m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING)
    {
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
}

void
Dect2020ExtMac::ProcessAssociationRequest(Dect2020ExtPtInfo ptInfo)
{
    NS_LOG_INFO(MAC_IDENTITY << ": Processing Association Request");

    m_associatedPtDevices.push_back(ptInfo);

    // TODO: Implement logic to accept or reject device. Currently every device is accepted
    // TODO: Implement association maintenance

    // Calculate the time to send the association response. DECT_Delay == 0 --> subslot n + HARQ_feedback_delay + 1
    uint32_t currentAbsSubslot = m_macScheduler->GetCurrentSubslotNumber();

    uint8_t harqDelay = ptInfo.rdCapabilityIe.GetHarqFeedbackDelay();
    uint32_t responseAbsSubslot;

    responseAbsSubslot = currentAbsSubslot + harqDelay + 1;
    NS_LOG_INFO(MAC_IDENTITY << ": HARQ Delay: " << (uint32_t)harqDelay);
    NS_LOG_INFO(MAC_IDENTITY << ": Absolute response subslot: " << responseAbsSubslot);
    NS_LOG_INFO(MAC_IDENTITY << ": Current absolute subslot: " << currentAbsSubslot);

    // It is assumed that RACH resources cannot be configured to enter the next frame ->
    // GetTimeTillSubslotStartNs should be safe to call
    double t = m_macScheduler->GetTimeTillSubslotStartNs(responseAbsSubslot);

    Simulator::Schedule(NanoSeconds(t), &Dect2020ExtMac::SendAssociationResponse, this, ptInfo);
}

void
Dect2020ExtMac::SendAssociationResponse(Dect2020ExtPtInfo ptInfo)
{
    // Sanity check to assert that the device is currently not transmmitting another packet
    if (m_phy->GetState() == PhyState::TX)
    {
        NS_FATAL_ERROR("Device is already transmitting a packet while attempting to transmit another packet");
    }

    NS_LOG_INFO(MAC_IDENTITY << ": Sending Association Response at subslot " << m_macScheduler->GetCurrentSubslotNumber());

    Ptr<Packet> packet = BuildAssociationResponse(ptInfo);

    // --- Physical Header Field Type 2 ---
    bool lengthInSlots = false;
    bool ndi = false; // Currently not used, but indicates that that the ACK is for the received packet

    // Build PHY Control Field Type 2 for the appropriate HARQ process number taken
    // from the PHY Control Fild Type 2 of the Association Request
    Dect2020ExtPhyControlFieldType2 phyControlFieldType2 = BuildPhyControlFieldType2(lengthInSlots, packet->GetSize(), ndi, ptInfo.phyControlFieldType2.GetDfHarqProcessNumber()); 

    bool data = false;
    bool random = true; // This packet is an ACK for the received Association Request with Random Access
    m_phy->Send(packet, phyControlFieldType2, data, random); // Send the packet to the PHY
}

void
Dect2020ExtMac::ProcessAssociationResponse(Dect2020ExtFtInfo* ftInfo)
{
    NS_LOG_INFO(MAC_IDENTITY << "Processing Association Response");

    // Sanity check of the association state
    if (m_associationStatus != ASSOCIATION_PENDING)
    {
        NS_FATAL_ERROR("Association Response received, but association status is not ASSOCIATION_PENDING");
        return;
    }

    if (ftInfo->associationResponseMessageIe.GetAssociationAccepted())
    {
        Dect2020ExtHarqProcess* process = FindHarqProcess(ftInfo->phyControlFieldType2.GetDfHarqProcessNumber());

        // Reset the HARQ process
        process->Reset();

        // Currently, all association responses are accepted
        ChangeAssociationStatus(Dect2020ExtAssociationStatus::ASSOCIATED);

        // If we dont receive a Cluster Beacon every 60 Seconds, we assume the FT is offline
        // Simulator::Schedule(Seconds(60), &Dect2020ExtMac::VerifyAssociatedStatus, this);

        // Store the associated FTs Long RD ID
        m_associatedFtNetDeviceLongRdId = ftInfo->unicastHeader.GetTransmitterAddress();

        if (m_proceedToDataTransmission)
        {
            // Starting app transmission
            LaunchApp();
            StartKeepAlivePeriod();
        }
    }
    else
    {
        NS_FATAL_ERROR("Association was not accepted. Currently, all Association Requests must be accepted");
    }
}

void
Dect2020ExtMac::VerifyAssociatedStatus()
{
    Dect2020ExtFtInfo* ft = FindFtCandidateByLongId(m_associatedFtNetDeviceLongRdId);
    Time lastBeaconReceived = ft->receptionTime;
    Time currentTime = Simulator::Now();
    Time VerifyAssociatedStatusPeriod = Seconds(60);
    NS_LOG_INFO(MAC_IDENTITY << ": ASSOCIATED");

    // Check if the last received Cluster Beacon is older than 60 seconds
    if (currentTime - lastBeaconReceived > VerifyAssociatedStatusPeriod)
    {
        NS_LOG_WARN(
            Simulator::Now().GetMicroSeconds()
            << ": Dect2020Mac::VerifyAssociatedStatus: No Cluster Beacon received in the last "
            << "60 seconds. Setting association status to NOT_ASSOCIATED.");
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
    else
    {
        // Check again in VerifyAssociatedStatusPeriod
        Simulator::Schedule(VerifyAssociatedStatusPeriod,
                            &Dect2020ExtMac::VerifyAssociatedStatus,
                            this);
    }
}

Dect2020ExtFtInfo*
Dect2020ExtMac::FindFtCandidateByLongId(uint32_t longFtId)
{
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].longFtId == longFtId)
        {
            return &m_ftCandidates[idx];
        }
    }

    // No candidate found, create one
    Dect2020ExtFtInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

void
Dect2020ExtMac::StartKeepAlivePeriod()
{
    m_lastRxTime = Simulator::Now();
    Simulator::Schedule(m_configuredAssociationControlIe.DecodeUlPeriod(), &Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded, this);
}

void
Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded()
{
    Time ulPeriod = m_configuredAssociationControlIe.DecodeUlPeriod();

    if (Simulator::Now() - m_lastRxTime >= ulPeriod)
    {
        // KeepAlivePeriod exceeded - send Keep Alive Ie
        NS_LOG_INFO(MAC_IDENTITY << ": Keep Alive Period exceeded");
        Simulator::Schedule(ulPeriod, &Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded, this);
    }
    else
    {
        Time remaining = ulPeriod - (Simulator::Now() - m_lastRxTime);
        Simulator::Schedule(Max(remaining, MilliSeconds(1)), &Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded, this);
    }
}

void
Dect2020ExtMac::RestartKeepAlivePeriod()
{
    m_lastRxTime = Simulator::Now();
}

void
Dect2020ExtMac::SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType)
{
    m_terminationPointType = terminationPointType;
    m_terminationPointTypeSet = true;
}

Dect2020ExtNetDevice::Dect2020ExtTerminationPointType
Dect2020ExtMac::GetTerminationPointType() const
{
    return m_terminationPointType;
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_device = device;
}

Ptr<Dect2020ExtNetDevice>
Dect2020ExtMac::GetNetDevice() const
{
    return m_device;
}

Ptr<Node>
Dect2020ExtMac::GetNode() const
{
    if (m_device)
    {
        return m_device->GetNode();
    }
    return nullptr;
}

void
Dect2020ExtMac::SetPhy(Ptr<Dect2020ExtPhy> phy)
{
    m_phy = phy;
    m_phy->SetReceiveCallback(MakeCallback(&Dect2020ExtMac::ReceiveFromPhy, this));
}

Ptr<Dect2020ExtPhy>
Dect2020ExtMac::GetPhy() const
{
    return m_phy;
}

void
Dect2020ExtMac::SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager)
{
    m_channelManager = channelManager;
}

Ptr<Dect2020ExtChannelManager>
Dect2020ExtMac::GetChannelManager() const
{
    return m_channelManager;
}


void
Dect2020ExtMac::SetNetworkId(uint32_t networkId)
{
    m_networkId = networkId;
}

uint32_t
Dect2020ExtMac::GetNetworkId() const
{
    return m_networkId;
}

void
Dect2020ExtMac::SetLongRadioDeviceId(uint32_t rdId)
{
    if (!(rdId == 0x00000000))
    {
        m_longRadioDeviceId = rdId;

        NS_LOG_INFO(MAC_IDENTITY << ": Setting Long Radio Device ID: 0x" << std::hex << rdId);
    }
    else
    {
        NS_LOG_WARN(Simulator::Now().GetMilliSeconds()
                    << ": Invalid Long Radio Device ID detected. Generate a new one.");

        SetLongRadioDeviceId(GenerateLongRadioDeviceId());
    }
}

uint32_t
Dect2020ExtMac::GetLongRadioDeviceId() const
{
    return m_longRadioDeviceId;
}

void
Dect2020ExtMac::SetShortRadioDeviceId(uint16_t rdId)
{
    if (!(rdId == 0x0000))
    {
        m_shortRadioDeviceId = rdId;

        NS_LOG_INFO(MAC_IDENTITY << ": Setting Short Radio Device ID: 0x" << std::hex << rdId);
    }
    else
    {
        NS_LOG_WARN(Simulator::Now().GetMilliSeconds()
                    << ": Invalid Short Radio Device ID detected. Generate a new one.");

        SetShortRadioDeviceId(GenerateShortRadioDeviceId());
    }
}

uint16_t
Dect2020ExtMac::GetShortRadioDeviceId() const
{
    return m_shortRadioDeviceId;
}

void
Dect2020ExtMac::SetOperatingChannelNumber(uint16_t channelNumber)
{
    m_operatingChannelNumber = channelNumber;
}

uint16_t
Dect2020ExtMac::GetOperatingChannelNumber() const
{
    return m_operatingChannelNumber;
}

std::vector<Dect2020ExtChannelEvaluation>
Dect2020ExtMac::GetChannelEvaluations() const
{
    return m_channelEvaluations;
}

double
Dect2020ExtMac::GetRxGainFromIndex(uint8_t index) const
{
    if (index > 8)
    {
        NS_LOG_WARN("Invalid RX gain index: " << static_cast<int>(index));
        return 0; // default fallback
    }

    return static_cast<int8_t>(-10 + index * 2);
}

void
Dect2020ExtMac::SetMacScheduler(Ptr<Dect2020ExtMacScheduler> macScheduler)
{
    m_macScheduler = macScheduler;
}

Dect2020ExtMac::Dect2020ExtAssociationStatus
Dect2020ExtMac::GetAssociationStatus()
{
    return m_associationStatus;
}

void
Dect2020ExtMac::ChangeAssociationStatus(Dect2020ExtAssociationStatus associationStatus)
{
    std::string newStatus = "";
    switch (associationStatus)
    {
        case Dect2020ExtAssociationStatus::NOT_ASSOCIATED: newStatus = "NOT_ASSOCIATED"; break;
        case Dect2020ExtAssociationStatus::ASSOCIATION_PREPARING: newStatus = "ASSOCIATION_PREPARING"; break;
        case Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT: newStatus = "WAITING_FOR_SELECTED_FT"; break;
        case Dect2020ExtAssociationStatus::ASSOCIATION_PENDING: newStatus = "ASSOCIATION_PENDING"; break;
        case Dect2020ExtAssociationStatus::ASSOCIATED: newStatus = "ASSOCIATED"; break;
    }
    m_associationStatus = associationStatus;
    NS_LOG_INFO(MAC_IDENTITY << ": Association status changed to " << newStatus);
}


void
Dect2020ExtMac::EnqueuePacket()
{
    Ptr<Packet> macPdu = Create<Packet>(10); // dummy MAC PDU
    NS_LOG_INFO(MAC_IDENTITY << ": Enqueue packet at subslot " << m_macScheduler->GetCurrentSubslotNumber());

    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(0);
    unicastHeader.SetMacSequence(0);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(m_selectedFtCandidate.longFtId);
    unicastHeader.SetTransmitterAddress(m_longRadioDeviceId);

    macPdu->AddHeader(unicastHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::DATA_MAC_PDU_HEADER);

    macPdu->AddHeader(macHeaderType);

    m_txQueue.push(macPdu);
}

void
Dect2020ExtMac::LaunchApp()
{
    NS_LOG_INFO(MAC_IDENTITY << ": Launching application");
    m_app = CreateObject<Dect2020ExtApp>();
    m_app->SetMac(this);
    m_app->SetTxPeriod(MilliSeconds(5));
    m_app->SetPacketLimit(50);
    m_app->Start();
    // InitiateHARQ();
}

void
Dect2020ExtMac::SetBandNumber(uint8_t bandNumber)
{
    m_bandNumber = bandNumber;
    m_bandNumberSet = true;
}

uint8_t
Dect2020ExtMac::GetBandNumber() const
{
    return m_bandNumber;
}

void
Dect2020ExtMac::SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor)
{
    m_subcarrierScalingFactor = subcarrierScalingFactor;
    m_subcarrierScalingFactorSet = true;
}

uint8_t
Dect2020ExtMac::GetSubcarrierScalingFactor() const
{
    return m_subcarrierScalingFactor;
}

void
Dect2020ExtMac::SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor)
{
    m_fourierTransformScalingFactor = fourierTransformScalingFactor;
    m_fourierTransformScalingFactorSet = true;
}

uint8_t
Dect2020ExtMac::GetFourierTransformScalingFactor() const
{
    return m_fourierTransformScalingFactor;
}

void
Dect2020ExtMac::ConfigureRandomAccessResources(Dect2020ExtRandomAccessResourceIe randomAccessResourceIe)
{
    m_configuredRandomAccessResourceIe = randomAccessResourceIe;
    m_configuredRandomAccessResourceIeSet = true;
}

void
Dect2020ExtMac::ConfigureAssociationControlIe(Dect2020ExtAssociationControlIe associationControlIe)
{
    m_configuredAssociationControlIe = associationControlIe;
    m_configuredAssociationControlIeSet = true;
}

void
Dect2020ExtMac::ConfigureRngStream(int64_t stream)
{
    m_configuredRngStream = stream;
    m_configuredRngStreamSet = true;
}

void
Dect2020ExtMac::ProceedToBeaconTransmission(bool value)
{
    m_proceedToBeaconTransmission = value;
}

void
Dect2020ExtMac::ProceedToAssociationRequest(bool value)
{
    m_proceedToAssociationRequest = value;
}

void
Dect2020ExtMac::ProceedToAssociationResponse(bool value)
{
    m_proceedToAssociationResponse = value;
}

void
Dect2020ExtMac::ProceedToDataTransmission(bool value)
{
    m_proceedToDataTransmission = value;
}

void
Dect2020ExtMac::DisplayChannelEvaluations(bool value)
{
    m_displayChannelEvaluations = value;
}

void
Dect2020ExtMac::PrintConfiguration() const
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": DECT-2020 MAC Configuration:");
    NS_LOG_INFO(MAC_IDENTITY << ": Band Number: " << static_cast<uint32_t>(m_bandNumber));
    NS_LOG_INFO(MAC_IDENTITY << ": Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO(MAC_IDENTITY << ": Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor));

    std::cout << "DECT-2020 MAC Configuration:" << "\n";
    std::cout << "Band Number: " << static_cast<uint32_t>(m_bandNumber) << "\n";
    std::cout << "Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor) << "\n";
    std::cout << "Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor) << "\n";
}

void
Dect2020ExtMac::SetSearchBestFtTimeMs(double timeMs)
{
    m_searchBestFtTimeMs = timeMs;
}

} // namespace ns3