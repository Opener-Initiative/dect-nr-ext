#include "dect2020ext-mac.h" 
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-mac-pdu-helper.h"
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

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/callback.h"
#include "ns3/pointer.h"

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <string>

namespace ns3
{

using DlDataReception = Dect2020ExtAssociationControlIE::DlDataReception;
using ULPeriod = Dect2020ExtAssociationControlIE::ULPeriod;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMac);

Dect2020ExtMac::Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
    m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
}

Dect2020ExtMac::~Dect2020ExtMac()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtMac::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMac")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtMac>()

                                .AddAttribute("config_bandNumber",
                                              "DECT-2020 band number configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_bandNumber),
                                              MakeUintegerChecker<uint8_t>(1, 21))

                                .AddAttribute("config_subcarrierScalingFactor",
                                              "DECT-2020 subcarrier scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_subcarrierScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8))

                                .AddAttribute("config_fourierTransformScalingFactor",
                                              "DECT-2020 Fourier transform scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtMac::m_fourierTransformScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8));
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
Dect2020ExtMac::Start()
{
    // Configuration
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

    // Initialization
    m_backoffRandomVariable = CreateObject<UniformRandomVariable>();
    m_backoffRandomVariable->SetStream(GetNode()->GetId());

    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        NS_ABORT_MSG_IF(!m_configuredRandomAccessResourceIESet, "Random Access Resources were not configured for Dect2020ExtMac");

        std::ostringstream oss;
        oss << "FT_MAC[" << m_device->GetNode()->GetId() << "]";
        MAC_IDENTITY = oss.str();

        SetLongRadioDeviceId(GenerateLongRadioDeviceId());
        SetShortRadioDeviceId(GenerateShortRadioDeviceId());
        InitializeNetwork();
    }
    if (m_device->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
    {
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
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": Discovering networks");

    if (m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING ||
        m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT ||
        m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATED)
    {
        return; // Device is in association process --> abort discovering networks
    }

    // if (m_associatedFTNetDeviceLongRdId != 0)
    // {
    //     return; // Device is already associated --> abort
    // }

    std::vector<Ptr<Dect2020ExtChannel>> channelList = m_channelManager->GetValidChannels(this->GetBandNumber());

    // Check if there are any valid channels
    if (channelList.empty())
    {
        NS_LOG_WARN("No valid channels available for band " << (uint32_t)this->GetBandNumber());
        return;
    }

    // find the current channel
    uint32_t current = m_currentChannelId;
    uint32_t nextChannelId = 0;

    // search if current + 1 is part of the channel list
    bool found = false;
    for (auto& ch : channelList)
    {
        if (ch->m_channelId == current + 1)
        {
            nextChannelId = ch->m_channelId;
            found = true;
            break;
        }
    }

    if (!found)
    {
        // no valid next channel -> take the first one of the band
        nextChannelId = channelList.front()->m_channelId;
    }

    // Switch to the next channel
    NS_LOG_INFO(MAC_IDENTITY<< ": Switching to channel " << nextChannelId);
    SetCurrentChannelId(nextChannelId);

    Time t = MilliSeconds(10); // Discover Network wait time
    Simulator::Schedule(t, &Dect2020ExtMac::DiscoverNetworks, this); // Schedule next discovery
}

void
Dect2020ExtMac::InitializeNetwork()
{
    NS_LOG_INFO(MAC_IDENTITY << ": Starting network initialization");
    uint32_t networkId = GenerateValidNetworkID();
    SetNetworkId(networkId);

    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::ScheduleBackgroundScan, this);
}

uint32_t
Dect2020ExtMac::GenerateValidNetworkID(void)
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
Dect2020ExtMac::GenerateLongRadioDeviceId(void)
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
Dect2020ExtMac::GenerateShortRadioDeviceId(void)
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
Dect2020ExtMac::DisplayChannelEvaluations(void)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_DEBUG("============ Channel Evaluations Summary ============");
    
    // Display all channel evaluations
    if (m_channelEvaluations.empty())
    {
        NS_FATAL_ERROR("No channel evaluations available yet at: " << Simulator::Now().GetMilliSeconds());
        return;
    }
    
    // Display header
    NS_LOG_DEBUG("Channel ID | Free | Possible | Busy | Total | Free %");
    NS_LOG_DEBUG("-----------------------------------------------------");
    
    // Display each channel evaluation
    for (const auto& eval : m_channelEvaluations)
    {
        uint32_t total = eval.Total();
        double freePercentage = (total > 0) ? (eval.free * 100.0 / total) : 0.0;
        
        std::ostringstream oss;
        oss << std::setw(10) << eval.channelId << " | "
            << std::setw(4) << eval.free << " | "
            << std::setw(8) << eval.possible << " | "
            << std::setw(4) << eval.busy << " | "
            << std::setw(5) << total << " | "
            << std::fixed << std::setprecision(1) << std::setw(5) << freePercentage << "%";
        
        NS_LOG_DEBUG(oss.str());
    }
    
    NS_LOG_DEBUG("====================================================");
}

/**
 * @brief Schedule the background scan with the given delay.
 * 
 * Connect the OnRssiMeasurement callback to RssiMeasured Trace Callback. 
 * Then, schedule the background scan with the given delay.
 */
void
Dect2020ExtMac::ScheduleBackgroundScan()
{
    NS_LOG_FUNCTION(this);

    // Connect the OnRssiMeasurement callback to RssiMeasured Trace Callback
    // m_channelManager->TraceConnectWithoutContext("RssiMeasured", MakeCallback(&Dect2020ExtMac::OnRssiMeasurement, this));

    // Schedule the background scan
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::BackgroundScan, this);
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
        evaluation.channelId = it->first;

        for (uint32_t subslotNumber = 0; subslotNumber < it->second.size(); ++subslotNumber)
        {
            // NS_LOG_INFO("Subslot " << subslotNumber << " RSSI: " << it->second[subslotNumber]);
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
    
    if (!evaluations.empty())
    {
        m_operatingChannelId = evaluations[0].channelId;
        NS_LOG_INFO(MAC_IDENTITY << ": Selected Operating Channel: " << m_operatingChannelId 
                    << " with " << evaluations[0].free << " free subslots");
    }
    else
    {
        NS_LOG_WARN("No channel evaluations available for selection");
    }

    StartBeaconTransmission();
}

void
Dect2020ExtMac::SampleRSSIFromInterference(std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> context, uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);

    // Calculate channel index from the channel number
    uint16_t channelIndex = context->channelNumber - m_channelManager->GetFirstValidChannelNumber(m_channelManager->GetBandNumber(context->channelNumber));

    // Get RSSI from interference
    double rssiChannelWatt = (*m_phy->m_interference->m_allSignals)[channelIndex];
    double rssiChannelDbm = m_channelManager->WToDbm(rssiChannelWatt);

    // Store the RSSI sample
    context->rssiAverages[subslotNumber].push_back(rssiChannelDbm);
    
    // NS_LOG_INFO("Sampled RSSI on channel " << context->channelNumber << ", subslot " << subslotNumber << ": " << rssiChannelDbm << " dBm");
}

void
Dect2020ExtMac::StartSubslotScan(std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext, uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);

    // Debug
    // if (subslotNumber == 0)
    // {
    //     NS_LOG_INFO("Starting scanning on " << channelContext->channelNumber);
    // }

    // 1. Tune to the correct channel
    SetCurrentChannelId(channelContext->channelNumber);

    // 2. Get power from the subslot with sampling rate
    double slotDurationNs = 416666; // 0,41667 milliseconds in nanoseconds
    double subslotDurationNs = slotDurationNs / m_channelManager->GetSubslotsPerSlot();
    double baseSamplingOffsetNs = subslotDurationNs / 5;

    for (uint32_t sampleIndex = 0; sampleIndex < 5; sampleIndex++)
    {
        double samplingOffsetNs = baseSamplingOffsetNs * sampleIndex;

        // NS_LOG_INFO("Scheduling RSSI sampling at " << samplingOffsetNs << " ns");
        Simulator::Schedule(NanoSeconds(samplingOffsetNs), &Dect2020ExtMac::SampleRSSIFromInterference, this, channelContext, subslotNumber);
    }

    // 3. Store the result of average RSSI on the subslot
}

void
Dect2020ExtMac::EndSubslotScan(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext, uint32_t subslotNumber)
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
Dect2020ExtMac::MeasureRSSI1OnChannel(std::shared_ptr<Dect2020ExtBackgroundScanContext> backgroundScanContext, std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext)
{
    NS_LOG_FUNCTION(this << channelContext->channelNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t slotsPerChannel = 24;
    uint32_t subslotsPerChannel = numSubslotsPerSlot * slotsPerChannel;

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
        std::shared_ptr<Dect2020ExtRSSI1OnChannelContext> channelContext = std::make_shared<Dect2020ExtRSSI1OnChannelContext>();
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
    NS_LOG_FUNCTION(this);

    // Tune the device to the selected operating channel
    m_currentChannelId = m_operatingChannelId;

    // Network Beacon Transmission - subslot 0 (start of the first slot)
    Dect2020ExtMacSchedulerOperation networkBeaconOperation;
    networkBeaconOperation.m_operationType = Dect2020ExtMacSchedulerOperation::TX_NETWORK_BEACON;
    networkBeaconOperation.m_channelNumber = m_currentChannelId;
    networkBeaconOperation.m_startSubslot = 0;
    networkBeaconOperation.m_endSubslot = 0;

    networkBeaconOperation.m_operation = [this](){
        this->StartNetworkBeaconTransmission();
    };
    networkBeaconOperation.m_periodFrames = ConvertNetworkBeaconPeriodToFrames(m_networkBeaconPeriod);
    networkBeaconOperation.m_frameOffset = m_macScheduler->GetSFN() + 1; // +1 since the operation is scheduled on the next frame

    m_networkBeaconSubslot = 0;

    m_macScheduler->ScheduleOperationOnNextFrame(networkBeaconOperation);

    // Cluster Beacon Transmission - subslot 3 (start of the second slot)
    Dect2020ExtMacSchedulerOperation clusterBeaconOperation;
    clusterBeaconOperation.m_operationType = Dect2020ExtMacSchedulerOperation::TX_CLUSTER_BEACON;
    clusterBeaconOperation.m_channelNumber = m_currentChannelId;
    clusterBeaconOperation.m_startSubslot = 4;
    clusterBeaconOperation.m_endSubslot = 4;

    clusterBeaconOperation.m_operation = [this](){
        this->StartClusterBeaconTransmission();
    };

    clusterBeaconOperation.m_periodFrames = ConvertClusterBeaconPeriodToFrames(m_clusterBeaconPeriod);
    clusterBeaconOperation.m_frameOffset = m_macScheduler->GetSFN() + 1; // +1 since the operation is scheduled on the next frame

    m_clusterBeaconSubslot = 4;

    m_macScheduler->ScheduleOperationOnNextFrame(clusterBeaconOperation);
}

void
Dect2020ExtMac::StartNetworkBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendNetworkBeaconOnChannel, this, m_currentChannelId);
}

void
Dect2020ExtMac::StartClusterBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendClusterBeaconOnChannel, this, m_operatingChannelId);
}

void
Dect2020ExtMac::SendNetworkBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    NS_LOG_DEBUG(this);
    Ptr<Packet> networkBeacon = BuildNetworkBeacon();

    NS_LOG_INFO(MAC_IDENTITY << ": Sending Network Beacon (channel=" << m_currentChannelId << ",slot=" << m_macScheduler->GetCurrentSlotNumber() << ",subslot=" << m_macScheduler->GetCurrentSubslotNumber() << ")");
    m_phy->Send(networkBeacon, BuildPhysicalHeaderField(1, networkBeacon->GetSize()));
}

void
Dect2020ExtMac::SendClusterBeaconOnChannel(uint16_t channelId)
{
    NS_LOG_FUNCTION(this << channelId);
    Ptr<Packet> packet = BuildClusterBeacon(channelId);

    NS_LOG_INFO(MAC_IDENTITY << ": Sending Cluster Beacon (channel=" << m_currentChannelId << ",slot=" << m_macScheduler->GetCurrentSlotNumber() << ",subslot=" << m_macScheduler->GetCurrentSubslotNumber() << ")");
    m_phy->Send(packet, BuildPhysicalHeaderField(1, packet->GetSize()));
}

Ptr<Packet>
Dect2020ExtMac::BuildNetworkBeacon()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> beacon = Create<Packet>();
    
    // Network Beacon Message IE
    Dect2020ExtNetworkBeaconMessageIE networkBeaconMessageIE;
    networkBeaconMessageIE.SetTxPowerIncluded(false);
    networkBeaconMessageIE.SetPowerConstraints(false);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(false);
    networkBeaconMessageIE.SetNetworkBeaconChannels(0);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_100MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_50MS);
    networkBeaconMessageIE.SetNextClusterChannel(m_operatingChannelId);

    // Calculate time to next Cluster Beacon
    // Get absolute time offsets for each beacon within a frame
    double networkBeaconTimeNs = m_macScheduler->GetSubslotStartTimeOffsetNs(m_networkBeaconSubslot);
    double clusterBeaconTimeNs = m_macScheduler->GetSubslotStartTimeOffsetNs(m_clusterBeaconSubslot);

    // Calculate the offset between them
    double offsetTimeNs = clusterBeaconTimeNs - networkBeaconTimeNs;

    auto timeToNextClusterBeaconUs = static_cast<uint32_t>(offsetTimeNs / 1000.0);

    // NS_LOG_INFO(MAC_IDENTITY << ": time to next cluster beacon " << timeToNextClusterBeaconUs << "us");

    networkBeaconMessageIE.SetTimeToNext(timeToNextClusterBeaconUs);

    beacon->AddHeader(networkBeaconMessageIE);

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

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
Dect2020ExtMac::BuildClusterBeacon(uint32_t clusterChannelId)
{
    NS_LOG_FUNCTION(this);
    Ptr<Packet> beacon = Create<Packet>();

    // Cluster Beacon Message IE
    Dect2020ExtClusterBeaconMessageIE clusterBeaconMessageIE;
    clusterBeaconMessageIE.SetSFN(m_macScheduler->GetSFN());
    clusterBeaconMessageIE.SetTxPowerIncluded(false);
    clusterBeaconMessageIE.SetPowerConstraints(false);
    clusterBeaconMessageIE.SetFrameOffsetIncluded(false);
    clusterBeaconMessageIE.SetNextChannelIncluded(false);
    clusterBeaconMessageIE.SetTimeToNextFieldPresent(false);
    clusterBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    clusterBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    clusterBeaconMessageIE.SetCountToTrigger(15);
    clusterBeaconMessageIE.SetRelQuality(0);
    clusterBeaconMessageIE.SetMinQuality(0);

    beacon->AddHeader(clusterBeaconMessageIE);

    // Cluster beacon message is always followed by the Random Access Ressource IE
    Dect2020ExtRandomAccessResourceIE randomAccessResourceIE = BuildRandomAccessResourceIE();
    beacon->AddHeader(randomAccessResourceIE);

    // MAC Multiplexing Header Type A
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::CLUSTER_BEACON_MESSAGE);

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

Dect2020ExtRandomAccessResourceIE
Dect2020ExtMac::BuildRandomAccessResourceIE()
{
    NS_LOG_FUNCTION(this);
    Dect2020ExtRandomAccessResourceIE randomAccessResourceIE;
    randomAccessResourceIE.SetRepeat(0);
    randomAccessResourceIE.SetSystemFrameNumberFieldIncluded(false);
    randomAccessResourceIE.SetChannelFieldIncluded(false);
    randomAccessResourceIE.SetSeparateChannelFieldIncluded(false);

    // uint8_t startSubslot = CalculateAbsoluteStartSubslot();
    randomAccessResourceIE.SetStartSubslot(10); // where RACH resources start
    randomAccessResourceIE.SetLengthType(0); // length in subslots
    randomAccessResourceIE.SetRaraLength(50);
    randomAccessResourceIE.SetMaxRachLengthType(0); // length in subslots
    randomAccessResourceIE.SetMaxRachLength(4);     // Max 4 subslots (2 slots) / transmission
    randomAccessResourceIE.SetCwMinSig(1);
    randomAccessResourceIE.SetDectDelay(m_dectDelay);
    randomAccessResourceIE.SetResponseWindow(5);
    randomAccessResourceIE.SetCwMaxSig(5);

    return randomAccessResourceIE;
}

Dect2020ExtPHYControlFieldType1
Dect2020ExtMac::BuildPhysicalHeaderField(uint8_t packetLengthType, uint32_t packetLengthBytes)
{
    Dect2020ExtPHYControlFieldType1 physicalHeaderField;
    physicalHeaderField.SetPacketLengthType(packetLengthType); // 0 = in Subslots, 1 = in Slots

    if (packetLengthType == 0) // Packet length is given in subslots
    {
        // TBD
    }
    else if (packetLengthType == 1) // Packet length is given in slots
    {
        double packetLengthBits = packetLengthBytes * 8;
        double mcsTransportBlockSizeBits =
            this->m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor,
                                                  m_fourierTransformScalingFactor,
                                                  m_mcs);
        uint16_t packetLengthInSlots = 0;
        packetLengthInSlots = std::ceil(packetLengthBits / mcsTransportBlockSizeBits);

        // NS_LOG_INFO(MAC_IDENTITY << ": MAC PDU length in slots: " << packetLengthInSlots);

        physicalHeaderField.SetPacketLength(packetLengthInSlots); // Size of packet in slots
    }

    physicalHeaderField.SetShortNetworkID(m_networkId);
    physicalHeaderField.SetTransmitterIdentity(m_shortRadioDeviceId);
    physicalHeaderField.SetTransmitPower(23); // dBm -- device power class 1
    physicalHeaderField.SetDFMCS(m_mcs);

    return physicalHeaderField;
}

uint8_t
Dect2020ExtMac::CalculateAbsoluteStartSubslot()
{
    NS_LOG_FUNCTION(this);
    // uint8_t absoluteStartSubslot = m_nextAvailableSubslot;
    // uint8_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;

    // // If the next available subslot is greater than the number of subslots per frame,
    // // wrap around to the beginning of the frame
    // if (absoluteStartSubslot >= subslotsPerFrame)
    // {
    //     absoluteStartSubslot = absoluteStartSubslot % subslotsPerFrame;
    //     m_nextAvailableSubslot = absoluteStartSubslot;
    // }

    // m_nextAvailableSubslot += 2;

    // return absoluteStartSubslot;
    return 6; // start of slot 3
}

uint32_t
Dect2020ExtMac::ConvertNetworkBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtNetworkBeaconPeriod period)
{
    switch (period)
    {
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS: return 5; // 50ms / 10ms = 5 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_100MS: return 10; // 100ms / 10ms = 10 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_500MS: return 50; // 500ms / 10ms = 50 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_1000MS: return 100; // 1000ms / 10ms = 100 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_1500MS: return 150; // 1500ms / 10ms = 150 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_2000MS: return 200; // 2000ms / 10ms = 200 frames
        case Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_4000MS: return 400; // 4000ms / 10ms = 400 frames
        default:
            NS_FATAL_ERROR("Invalid network beacon period");
            return 10; // fallback
    }
}

uint32_t
Dect2020ExtMac::ConvertClusterBeaconPeriodToFrames(Dect2020ExtNetworkBeaconMessageIE::Dect2020ExtClusterBeaconPeriod period)
{
    switch (period)
    {
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_10MS: return 1; // 10ms / 10ms = 1 frame
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_50MS: return 5; // 50ms / 10ms = 5 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS: return 10; // 100ms / 10ms = 10 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_500MS: return 50; // 500ms / 10ms = 50 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_1000MS: return 100; // 1000ms / 10ms = 100 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_1500MS: return 150; // 1500ms / 10ms = 150 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_2000MS: return 200; // 2000ms / 10ms = 200 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_4000MS: return 400; // 4000ms / 10ms = 400 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_8000MS: return 800; // 8000ms / 10ms = 800 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_16000MS: return 1600; // 16000ms / 10ms = 1600 frames
        case Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_32000MS: return 3200; // 32000ms / 10ms = 3200 frames
        default:
            NS_FATAL_ERROR("Invalid cluster beacon period");
            return 1; // fallback
    }
}

uint8_t
Dect2020ExtMac::CalculateMacPduSizeInSubslots(uint32_t packetLengthInBytes) const
{
    double packetLengthInBits = packetLengthInBytes * 8;
    double mcsTransportBlockSizeBitsPerSlot = m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor, m_fourierTransformScalingFactor, m_mcs);

    uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();

    // Calculate transport block size per subslot
    double mcsTransportBlockSizeBitsPerSubslot = mcsTransportBlockSizeBitsPerSlot / subslotsPerSlot;

    // Calculate packet length in subslots (round up)
    uint8_t packetLengthInSubslots = std::ceil(packetLengthInBits / mcsTransportBlockSizeBitsPerSubslot);

    return packetLengthInSubslots;
}

uint8_t
Dect2020ExtMac::CalculateMacPduSizeInSlots(uint32_t packetLengthInBytes) const
{
    double packetLengthInBits = packetLengthInBytes * 8;
    double mcsTransportBlockSizeBitsPerSlot = m_phy->GetMcsTransportBlockSize(m_subcarrierScalingFactor, m_fourierTransformScalingFactor, m_mcs);

    // Calculate packet length in slots (round up)
    uint8_t packetLengthInSlots = std::ceil(packetLengthInBits / mcsTransportBlockSizeBitsPerSlot);

    return packetLengthInSlots;
}

void
Dect2020ExtMac::InitiateRandomAccess(Dect2020ExtRandomAccessResourceIE randomAccessResourceIE, Ptr<Packet> packet, Dect2020ExtPHYControlFieldType1 physicalHeaderField)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": initiating Random Access Procedure");
    uint16_t startSubslot = randomAccessResourceIE.GetStartSubslot();
    bool lengthInSubslots = !randomAccessResourceIE.GetLengthType();
    uint8_t length = randomAccessResourceIE.GetRaraLength(); // RACH resources length

    // Calculate the total RACH resource duration in microseconds
    double rachDurationUs;
    if (lengthInSubslots)
    {
        uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
        double subslotDurationUs = (m_macScheduler->SLOT_DURATION_NS / subslotsPerSlot) / 1000.0; // Convert ns to us
        rachDurationUs = length * subslotDurationUs;
    }
    else
    {
        // tbd --> Length in slots
        NS_LOG_WARN("WARNING: RACH resources length is in slots (not yet implemented)");
        return;
    }

    const double START_POSITION_INTERVAL_US = STF_US + GI_US; // 83.335 us;

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

    // Calculate CW_MIN; CW_MIN = 8 * CWmin_sig
    uint32_t CW_MIN = 8 * randomAccessResourceIE.GetCwMinSig();

    // Calculate CW_MAX; CW_MAX = 256 * CWmax_sig
    uint32_t CW_MAX = 256 * randomAccessResourceIE.GetCwMaxSig();

    // Initialize and set CW_CURRENT to CW_MIN
    uint32_t CW_CURRENT = CW_MIN;

    // Calculate time until RACH resources start
    double timeToRachStartNs = m_macScheduler->GetTimeTillSubslotStartNs(startSubslot);
    Time delay = NanoSeconds(timeToRachStartNs);

    std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext = std::make_shared<Dect2020ExtRandomAccessContext>();
    randomAccessContext->m_randomAccessResourceIE = randomAccessResourceIE;
    randomAccessContext->m_startPositions = startPositions;
    randomAccessContext->m_CW_MAX = CW_MAX;
    randomAccessContext->m_CW_MIN = CW_MIN;
    randomAccessContext->m_CW_CURRENT = CW_CURRENT;
    randomAccessContext->m_packet = packet;
    randomAccessContext->m_physicalHeaderField = physicalHeaderField;

    // Store RACH start time for precise timing calculations
    randomAccessContext->m_rachStartTime = Simulator::Now() + delay;
    randomAccessContext->m_currentPositionIndex = 0; // Start at position 0

    // Schedule the backoff initialization to start when RACH resources begin
    Simulator::Schedule(delay, &Dect2020ExtMac::InitiateBackoff, this, randomAccessContext);
}

void
Dect2020ExtMac::InitiateBackoff(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": initiating backoff at subslot - " << m_macScheduler->GetCurrentSubslotNumber());

    // Pick a random backoff value uniformly distributed between 0 and CW_CURRENT
    uint32_t backoffValue = m_backoffRandomVariable->GetInteger(0, randomAccessContext->m_CW_CURRENT);

    // Calculate target position: current position + backoff
    uint32_t targetPositionIndex = randomAccessContext->m_currentPositionIndex + backoffValue;

    // Ensure we don't exceed available positions
    if (targetPositionIndex >= randomAccessContext->m_startPositions.size())
    {        
        NS_LOG_WARN("Backoff target position " << targetPositionIndex 
                    << " exceeds available positions (" 
                    << randomAccessContext->m_startPositions.size() << ")");

        // Defer to next RACH opportunity
        m_associationStatus = Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT;
        ChangeAssociationStatus(Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT);
        return;
    }

    // Calculate backoff duration in microseconds
    double targetPositionTimeUs = randomAccessContext->m_startPositions[targetPositionIndex];
    double currentPositionTimeUs = randomAccessContext->m_startPositions[randomAccessContext->m_currentPositionIndex];
    double backoffDurationUs = targetPositionTimeUs - currentPositionTimeUs;

    NS_LOG_INFO(MAC_IDENTITY << ": random backoff value: " << backoffValue);
    
    // Update current position to target position
    randomAccessContext->m_currentPositionIndex = targetPositionIndex;

    // Set rachBackOff
    double totalDelayUs = backoffDurationUs;
    Time totalDelay = MicroSeconds(totalDelayUs);

    // Schedule the LBT (Listen Before Talk) after rachBackOff expires
    Simulator::Schedule(totalDelay, &Dect2020ExtMac::PerformLBT, this, randomAccessContext);
}

void
Dect2020ExtMac::PerformLBT(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": Performing LBT at subslot " << m_macScheduler->GetCurrentSubslotNumber());

    // Reset sampling context
    randomAccessContext->m_lbtSampleCount = 0;
    randomAccessContext->m_lbtRssiSamples.clear();
    randomAccessContext->m_lbtRssiSamples.reserve(5); // Reserve space for 5 samples

    // Calculate our current position relative to RACH start
    Time currentTime = Simulator::Now();
    Time elapsedSinceRachStart = currentTime - randomAccessContext->m_rachStartTime;
    double elapsedUs = elapsedSinceRachStart.GetMicroSeconds();

    // Find which start position we are closest to
    const double START_POSITION_INTERVAL_US = STF_US + GI_US;
    auto currentPositionIndex = static_cast<uint32_t>(std::round(elapsedUs / START_POSITION_INTERVAL_US));

    NS_LOG_INFO(MAC_IDENTITY << ": LBT at position " << currentPositionIndex 
                 << " (elapsed: " << elapsedUs << " us)");

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
        Simulator::Schedule(MicroSeconds(offsetUs), &Dect2020ExtMac::SampleLBTRSSI, this, randomAccessContext);
    }
}

void
Dect2020ExtMac::SampleLBTRSSI(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": Sampling LBT RSSI");

    // Get current channel number and calculate channel index
    uint16_t currentChannelNumber = m_currentChannelId;
    uint16_t firstValidChannelNumber = m_channelManager->GetFirstValidChannelNumber(m_bandNumber);
    uint16_t channelIndex = currentChannelNumber - firstValidChannelNumber;

    // Sample channel power from PHY interference model
    double rssiChannelWatt = (*m_phy->m_interference->m_allSignals)[channelIndex];
    double rssiChannelDbm = m_channelManager->WToDbm(rssiChannelWatt);

    // Store the sample
    randomAccessContext->m_lbtRssiSamples.push_back(rssiChannelDbm);
    randomAccessContext->m_lbtSampleCount++;

    // If all samples collected, evaluate the channel
    if (randomAccessContext->m_lbtSampleCount >= randomAccessContext->m_lbtTotalSamples)
    {
        EvaluateLBTChannel(randomAccessContext);
    }
}

void
Dect2020ExtMac::EvaluateLBTChannel(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": Evaluating LBT channel at subslot " << m_macScheduler->GetCurrentSubslotNumber());
    NS_LOG_INFO(MAC_IDENTITY << ": SFN: " << m_macScheduler->GetSFN());

    // Calculate average RSSI from all samples
    double totalRssi = 0.0;
    double maxRssi = -std::numeric_limits<double>::infinity();

    for (double rssi : randomAccessContext->m_lbtRssiSamples)
    {
        totalRssi += rssi;
        maxRssi = std::max(maxRssi, rssi);
    }

    double avgRssiDbm = totalRssi / randomAccessContext->m_lbtRssiSamples.size();

    NS_LOG_INFO(MAC_IDENTITY << ": LBT RSSI - Average: " << avgRssiDbm << " dBm, Max: " << maxRssi << " dBm");

    // Determine channel state based on RSSI threshlods
    bool channelIsFree = (avgRssiDbm <= RSSI_THRESHOLD_MIN);
    bool channelIsPossible = (avgRssiDbm > RSSI_THRESHOLD_MIN && avgRssiDbm <= RSSI_THRESHOLD_MAX);
    bool channelIsBusy = (avgRssiDbm > RSSI_THRESHOLD_MAX);

    if (channelIsFree || channelIsPossible)
    {
        // Determine if MAC PDU fits into the remaining RACH resource subslots/slots
        bool fitsInRachResources = CheckIfMacPduFitsInMaxRandomAccessTxTime(randomAccessContext);
        if (fitsInRachResources)
        {
            // Transmit packet
            NS_LOG_INFO(MAC_IDENTITY << ": Transmitting -> power on channel is " << avgRssiDbm << " dBm");
            m_phy->Send(randomAccessContext->m_packet, randomAccessContext->m_physicalHeaderField);
        }
        else
        {
            // Defer to next random access resources
            // Reset to WAITING_FOR_SELECTED_FT so next cluster beacon re-triggers association
            NS_LOG_INFO("Deferring to next random access resources -> the MAC PDU doesn't fit into RACH resources");
            m_associationStatus = Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT;
            return;
        }
    }
    else if (channelIsBusy)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Re-initiating backoff with doubled CW_CURRENT -> the channel is busy (avg RSSI: " << avgRssiDbm << " dBm)");

        // After LBT completes, we are at the next position (LBT duration = 1 interval)
        randomAccessContext->m_currentPositionIndex += 1;

        // Exponential backoff: double CW_CURRENT, cap at CW_MAX
        uint32_t newCW = std::min(randomAccessContext->m_CW_CURRENT * 2, randomAccessContext->m_CW_MAX);
        randomAccessContext->m_CW_CURRENT = newCW;

        // Reset sample counter and vector for next attempt
        randomAccessContext->m_lbtSampleCount = 0;
        randomAccessContext->m_lbtRssiSamples.clear();

        // Re-initiate backoff with increased CW
        InitiateBackoff(randomAccessContext);
    }
}

bool
Dect2020ExtMac::CheckIfMacPduFitsInMaxRandomAccessTxTime(std::shared_ptr<Dect2020ExtRandomAccessContext> randomAccessContext) const
{
    bool lengthInSubslots = !randomAccessContext->m_randomAccessResourceIE.GetMaxRachLengthType();

    if (lengthInSubslots)
    {
        uint8_t packetLengthInSubslots = CalculateMacPduSizeInSubslots(randomAccessContext->m_packet->GetSize());
        NS_LOG_INFO("MAC PDU size: " << (uint32_t)packetLengthInSubslots << " subslots");
        return packetLengthInSubslots <= randomAccessContext->m_randomAccessResourceIE.GetMaxRachLength();
    }
    else
    {
        uint8_t packetLengthInSlots = CalculateMacPduSizeInSlots(randomAccessContext->m_packet->GetSize());
        NS_LOG_INFO("MAC PDU size: " << (uint32_t)packetLengthInSlots << " slots");
        return packetLengthInSlots <= randomAccessContext->m_randomAccessResourceIE.GetMaxRachLength();
    }
}

void
Dect2020ExtMac::ReceiveFromPhy(Ptr<Packet> packet, double rssiDbm)
{
    NS_LOG_FUNCTION(this << packet);
    // NS_LOG_INFO("Received packet from PHY with RSSI " << rssiDbm << " dBm on channel " << m_currentChannelId);

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField;
    packet->RemoveHeader(physicalHeaderField);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    packet->RemoveHeader(macHeaderType);

    if (macHeaderType.GetMacHeaderTypeField() == Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER)
    {
        // Create FT Candidate Info
        // FT Candidate Info can be updated if the candidate has sent a beacon message before
        Dect2020ExtFtCandidateInfo* ft = FindOrCreateFtCandidate(physicalHeaderField.GetTransmitterIdentity());
        ft->rssiDbm = rssiDbm;
        ft->ftPhyHeaderField = physicalHeaderField;
        ft->receptionTime = Simulator::Now();
        ft->shortFtId = physicalHeaderField.GetTransmitterIdentity();

        HandleBeaconPacket(packet, ft);
    }

    else if (macHeaderType.GetMacHeaderTypeField() == Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Unicast Packet");
        HandleUnicastPacket(packet);
    }

    // Forward the packet to the net device for minimal implementation
    m_device->Receive(packet);
}

void
Dect2020ExtMac::HandleBeaconPacket(Ptr<Packet> packet, Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this << packet);

    // --- Beacon Header ---
    Dect2020ExtBeaconHeader beaconHeader;
    packet->RemoveHeader(beaconHeader);

    ft->ftBeaconHeader = beaconHeader;
    ft->networkId = beaconHeader.GetNetworkId();
    ft->longFtId = beaconHeader.GetTransmitterAddress();

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    packet->RemoveHeader(mxHeader);
    m_lastFtMacMxHeader = mxHeader;

    Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding ieType = mxHeader.GetIeTypeFieldEncoding();

    // --- Network Beacon Message ---
    if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Network Beacon Message");

        // --- Network Beacon Message IE ---
        Dect2020ExtNetworkBeaconMessageIE networkBeaconMessage;
        packet->RemoveHeader(networkBeaconMessage);
        ft->ftNetworkBeaconMessage = networkBeaconMessage;

        // Get the cluster channel id from the network beacon message
        uint16_t ftBeaconNextClusterChannel = networkBeaconMessage.GetNextClusterChannel();
        NS_LOG_INFO(MAC_IDENTITY << ": Next cluster channel in network beacon: " << ftBeaconNextClusterChannel);
        ft->clusterChannelId = ftBeaconNextClusterChannel;
    }

    // --- Cluster Beacon Message ---
    else if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::CLUSTER_BEACON_MESSAGE)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Cluster Beacon Message");

        // if the PT has received a Cluster Beacon and no Network Beacon before, we assume the
        // cluster channel is the current channel
        if (ft->ftNetworkBeaconMessage.GetNextClusterChannel() == 0)
        {
            NS_LOG_INFO(MAC_IDENTITY << ": No Network Beacon Message received before, setting cluster channel to current channel");
            ft->ftNetworkBeaconMessage.SetNextClusterChannel(this->m_currentChannelId);
            ft->clusterChannelId = this->m_currentChannelId;
        }

        Dect2020ExtRandomAccessResourceIE randomAccessResourceIE;
        packet->RemoveHeader(randomAccessResourceIE);
        ft->ftRandomAccessResourceIE = randomAccessResourceIE;

        Dect2020ExtClusterBeaconMessageIE clusterBeaconMessage;
        packet->RemoveHeader(clusterBeaconMessage);
        ft->ftClusterBeaconMessage = clusterBeaconMessage;

        EvaluateClusterBeacon(clusterBeaconMessage, randomAccessResourceIE, ft);
    }
}

void
Dect2020ExtMac::HandleUnicastPacket(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    packet->RemoveHeader(unicastHeader);

    // Check if the Unicast Message is for this device
    uint32_t receiverAddress = unicastHeader.GetReceiverAddress();
    uint32_t transmitterAddress = unicastHeader.GetTransmitterAddress();

    if (receiverAddress != this->GetLongRadioDeviceId())
    {
        return; // Packet is not for this device
    }

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    packet->RemoveHeader(mxHeader);

    Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding ieType = mxHeader.GetIeTypeFieldEncoding();

    // Message is an association request
    if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE)
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Received Association Request");
        // check whether this device is an FT --> a PT cannot process an association request
        if (m_device->GetTerminationPointType() !=
            Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
        {
            NS_LOG_WARN("Association Request received, but this device is not an FT.");
            return;
        }

        // Strip the message headers
        Dect2020ExtAssociationRequestMessageIE associationRequestMessage;
        packet->RemoveHeader(associationRequestMessage);

        Dect2020ExtRdCapabilityIE rdCapabilityIE;
        packet->RemoveHeader(rdCapabilityIE);

        Dect2020ExtAssociationControlIE associationControlIE;
        packet->RemoveHeader(associationControlIE);

        ProcessAssociationRequest(associationControlIE, rdCapabilityIE, associationRequestMessage, transmitterAddress);
    }

    // Message is an association response
    else if (ieType == Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE)
    {
        // check whether this device is an PT --> TODO: implement associationable FTs
        if (m_device->GetTerminationPointType() !=
            Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
        {
            NS_LOG_WARN("Association Response received, but this device is not an PT. FT "
                        "association not yet implemented.");
            return;
        }

    //     // // Strip the message headers
        Dect2020ExtAssociationResponseMessageIE associationResponseMessage;
        packet->RemoveHeader(associationResponseMessage);

        // Dect2020RdCapabilityIE rdCapabilityIE;
        // packet->RemoveHeader(rdCapabilityIE);

        ProcessAssociationResponse(associationResponseMessage, transmitterAddress);
    }
}

Dect2020ExtFtCandidateInfo*
Dect2020ExtMac::FindOrCreateFtCandidate(uint16_t shortFtId)
{
    NS_LOG_FUNCTION(this << shortFtId);
    for (size_t idx = 0; idx < m_ftCandidates.size(); idx++)
    {
        if (m_ftCandidates[idx].shortFtId == shortFtId)
        {
            return &m_ftCandidates[idx];
        }
    }

    // No candidate found, create one
    Dect2020ExtFtCandidateInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

void
Dect2020ExtMac::EvaluateClusterBeacon(const Dect2020ExtClusterBeaconMessageIE& clusterBeaconMessage, const Dect2020ExtRandomAccessResourceIE& randomAccessResourceIE, Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this);
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
        // This is the time in seconds for the PT to listen to other channels to collect potential FT candidates
        Time searchBestFtTimeSeconds = Seconds(2);
        
        // ASSOCIATION_PREPARING means the device is scanning the network to find the best FT candidate
        m_associationStatus = Dect2020ExtAssociationStatus::ASSOCIATION_PREPARING;
        NS_LOG_INFO(MAC_IDENTITY << ": Switching to ASSOCIATION_PREPARING status");
        NS_LOG_INFO(MAC_IDENTITY << ": Discovering networks");
        DiscoverNetworks();
        Simulator::Schedule(searchBestFtTimeSeconds, &Dect2020ExtMac::SelectBestFtCandidate, this);
    }

    // prepare association request --> The device is waiting fot the cluster Beacon of the selected FT
    if (m_associationStatus == Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT && m_selectedFtCandidate.longFtId == ft->longFtId)
    {
        // Schedule the Association Request
        Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendAssociationRequest, this, ft);
    }
}

void
Dect2020ExtMac::SelectBestFtCandidate(void)
{
    NS_LOG_FUNCTION(this);

    // Step 1: Find the FT with the best (highest) RSSI-2
    double bestRssiDbm = -150.0; // start with very low RSSI
    Dect2020ExtFtCandidateInfo bestCandidate;
    bool found = false;

    for (const auto& ft : m_ftCandidates)
    {
        if (ft.rssiDbm > bestRssiDbm)
        {
            bestRssiDbm = ft.rssiDbm;
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate found with RSSI " << bestRssiDbm << " dBm");
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate long ID: " << ft.longFtId);
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate network ID: " << ft.networkId);
            NS_LOG_INFO(MAC_IDENTITY << ": Best FT candidate cluster channel ID: " << ft.clusterChannelId);
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
    NS_LOG_INFO(MAC_IDENTITY << ": Switching to best FT's channel: " << bestCandidate.clusterChannelId);
    m_operatingChannelId = bestCandidate.clusterChannelId;
    m_currentChannelId = bestCandidate.clusterChannelId;

    // Step 4: Set status and wait for next cluster beacon of selected FT
    m_associationStatus = Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT;
    NS_LOG_INFO(MAC_IDENTITY << ": Swithing to WAITING_FOR_SELECTED_FT status");

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
Dect2020ExtMac::SendAssociationRequest(Dect2020ExtFtCandidateInfo* ft)
{
    NS_LOG_FUNCTION(this << ft);
    NS_LOG_INFO(MAC_IDENTITY << ": Preparing to send association request");

    Ptr<Packet> packet = Create<Packet>();

    // --- Association Control IE ---
    Dect2020ExtAssociationControlIE associationControlIE;
    associationControlIE.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIE.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIE.SetUlPeriod(ULPeriod::S_10); // association maintenance period 10 seconds

    packet->AddHeader(associationControlIE);

    // --- RD Capability IE ---
    Dect2020ExtRdCapabilityIE rdCapabilityIE;
    rdCapabilityIE.SetRelease(this->m_device->m_release);
    rdCapabilityIE.SetGroupAssignment(this->m_device->m_supportGroupAssignment);
    rdCapabilityIE.SetPaging(this->m_device->m_supportPaging);
    rdCapabilityIE.SetOperatingModes(this->m_device->m_operatingModes);
    rdCapabilityIE.SetMesh(this->m_device->m_mesh);
    rdCapabilityIE.SetScheduledAccessDataTransfer(this->m_device->m_scheduledAccessDataTransfer);
    rdCapabilityIE.SetMacSecurity(this->m_device->m_macSecurity);
    rdCapabilityIE.SetDlcServiceType(this->m_device->m_dlcServiceType);
    rdCapabilityIE.SetRdPowerClass(this->m_device->m_rdPowerClass);
    rdCapabilityIE.SetMaxNssFoRx(this->m_device->m_maxNssFoRx);
    rdCapabilityIE.SetRxForTxDiversity(this->m_device->m_rxForTxDiversity);
    rdCapabilityIE.SetRxGain(this->m_device->m_rxGain);
    rdCapabilityIE.SetMaxMcs(this->m_device->m_maxMcs);
    rdCapabilityIE.SetSoftBufferSize(this->m_device->m_softBufferSize);
    rdCapabilityIE.SetNumOfHarqProcesses(this->m_device->m_numOfHarqProcesses);
    rdCapabilityIE.SetHarqFeedbackDelay(this->m_device->m_harqFeedbackDelay);
    rdCapabilityIE.SetDDelay(this->m_device->m_dDelay);
    rdCapabilityIE.SetHalfDulp(this->m_device->m_halfDulp);

    packet->AddHeader(rdCapabilityIE);

    // --- Association Request Message IE ---
    Dect2020ExtAssociationRequestMessageIE associationRequestMessage;
    associationRequestMessage.SetSetupCause(0); // Initial association
    associationRequestMessage.SetNumberOfFlows(0);
    associationRequestMessage.SetPowerConstraints(0);
    associationRequestMessage.SetFtMode(0);
    associationRequestMessage.SetCurrent(0);
    associationRequestMessage.SetHarqProcessesTx(0);
    associationRequestMessage.SetMaxHarqReTxDelay(0);
    associationRequestMessage.SetHarqProcessesRx(0);
    associationRequestMessage.SetMaxHarqReRxDelay(0);
    associationRequestMessage.SetFlowId(0);

    packet->AddHeader(associationRequestMessage);

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA muxHeader;
    muxHeader.SetMacExtensionFieldEncoding(0);
    muxHeader.SetLengthField(0);
    muxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_REQUEST_MESSAGE);

    packet->AddHeader(muxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(0);
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

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField = BuildPhysicalHeaderField(1, packet->GetSize());
    physicalHeaderField.SetShortNetworkID(m_potentialShortNetworkId);

    m_associationStatus = Dect2020ExtAssociationStatus::ASSOCIATION_PENDING;

    // Initiate Random Access Procedure with the selected FT
    InitiateRandomAccess(ft->ftRandomAccessResourceIE, packet, physicalHeaderField);
}

void
Dect2020ExtMac::VerifyPendingAssociationStatus(void)
{
    if (this->m_associationStatus == Dect2020ExtAssociationStatus::ASSOCIATION_PENDING)
    {
        m_associationStatus = Dect2020ExtAssociationStatus::NOT_ASSOCIATED;
    }
}

void
Dect2020ExtMac::ProcessAssociationRequest(const Dect2020ExtAssociationControlIE associationControlIE, const Dect2020ExtRdCapabilityIE& rdCapabilityIE, const Dect2020ExtAssociationRequestMessageIE& associationRequestMessage, uint32_t transmitterAddress)
{
    NS_LOG_FUNCTION(this << associationRequestMessage << transmitterAddress);
    NS_LOG_INFO(MAC_IDENTITY << ": Processing Association Request");

    Dect2020ExtAssociatedPtInfo ptInfo;
    ptInfo.associationEstablished = Simulator::Now();
    ptInfo.longRdId = transmitterAddress;
    // ptInfo.numberOfFlows = associationRequestMessage.GetNumberOfFlows();
    // ptInfo.powerConstraints = associationRequestMessage.GetPowerConstraints();
    // ptInfo.ftMode = associationRequestMessage.GetFtMode();
    // ptInfo.harqProcessesTx = associationRequestMessage.GetHarqProcessesTx();
    // ptInfo.maxHarqReTxDelay = associationRequestMessage.GetMaxHarqReTxDelay();
    // ptInfo.harqProcessesRx = associationRequestMessage.GetHarqProcessesRx();
    // ptInfo.maxHarqReRxDelay = associationRequestMessage.GetMaxHarqReRxDelay();
    // ptInfo.flowId = associationRequestMessage.GetFlowId();
    // // ptInfo.cb_m = assoControlIe.GetClusterBeaconMonitoring() == 1 ? true : false;
    // // ptInfo.rdCapabilityIe = rdCapabilityIe;

    m_associatedPtDevices.push_back(ptInfo);

    // TODO: implement logic to accept or reject device. Currently every device is accepted

    // calculate the time to send the association response. DECT_Delay == 0 --> subslot n + HARQ_feedback_delay +1
    // uint32_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;

    // uint32_t currentAbsSubslot = m_phy->GetCurrentAbsoluteSubslot();

    // uint8_t harqDelay = rdCapabilityIE.GetHarqFeedbackDelay();
    // uint32_t responseAbsSubslot;

    // if (m_dectDelay == 0)
    // {
    //     responseAbsSubslot = currentAbsSubslot + harqDelay + 1;
    //     NS_LOG_INFO("harqDelay: " << harqDelay);
    //     NS_LOG_INFO("responseAbsSubslot: " << responseAbsSubslot);
    //     NS_LOG_INFO("currentAbsSubslot: " << currentAbsSubslot);
    // }
    // else
    // {
    //     responseAbsSubslot = currentAbsSubslot + (subslotsPerFrame / 2); // 0.5 frame later
    // }

    // Time t = CalculcateTimeOffsetFromCurrentSubslot(responseAbsSubslot - currentAbsSubslot);
    // NS_LOG_INFO("t: " << t);

    Simulator::Schedule(Seconds(0), &Dect2020ExtMac::SendAssociationResponse, this, ptInfo);
}

void
Dect2020ExtMac::SendAssociationResponse(Dect2020ExtAssociatedPtInfo ptInfo)
{
    // TODO: Add RD Capability IE
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(MAC_IDENTITY << ": Sending Association Response");

    Ptr<Packet> packet = Create<Packet>();

    // --- Association Response Message ---
    Dect2020ExtAssociationResponseMessageIE associationResponseMessage;
    associationResponseMessage.SetAssociationAccepted(true);
    associationResponseMessage.SetHarqMod(0);
    associationResponseMessage.SetNumberOfFlows(7); // all flows are accepted
    associationResponseMessage.SetGroupIdAndResourceTagIncluded(0);

    packet->AddHeader(associationResponseMessage);

    // --- MAC Multiplexing Header ---
    Dect2020ExtMacMultiplexingHeaderTypeA mxHeader;
    mxHeader.SetMacExtensionFieldEncoding(0);
    mxHeader.SetLengthField(0);
    mxHeader.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::ASSOCIATION_RESPONSE_MESSAGE);

    packet->AddHeader(mxHeader);

    // --- Unicast Header ---
    Dect2020ExtUnicastHeader unicastHeader;
    unicastHeader.SetReset(0);
    unicastHeader.SetSequenceNumber(0);
    unicastHeader.SetReceiverAddress(ptInfo.longRdId);
    unicastHeader.SetTransmitterAddress(this->GetLongRadioDeviceId());

    packet->AddHeader(unicastHeader);

    // --- MAC Header Type ---
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetVersion(0);
    macHeaderType.SetMacSecurity(Dect2020ExtMacHeaderType::MacSecurityField::MAC_SECURITY_NOT_USED);
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::UNICAST_HEADER);

    packet->AddHeader(macHeaderType);

    // --- Physical Header Field ---
    Dect2020ExtPHYControlFieldType1 physicalHeaderField = BuildPhysicalHeaderField(1, packet->GetSize());
    physicalHeaderField.SetShortNetworkID(m_networkId);

    m_phy->Send(packet, physicalHeaderField); // Send the packet to the PHY
}

void
Dect2020ExtMac::ProcessAssociationResponse(Dect2020ExtAssociationResponseMessageIE associationResponseMessage, uint32_t transmitterAddress)
{
    NS_LOG_FUNCTION(this << associationResponseMessage << transmitterAddress);

    if (m_associationStatus != ASSOCIATION_PENDING)
    {
        NS_LOG_WARN(
            "Association Response received, but association status is not ASSOCIATION_PENDING. "
            "Ignoring the response.");
        return;
    }

    if (associationResponseMessage.GetAssociationAccepted())
    {
        // Currently, we accept all association responses
        ChangeAssociationStatus(Dect2020ExtAssociationStatus::ASSOCIATED);

        // If we dont receive a Cluster Beacon every 60 Seconds, we assume the FT is offline
        // Simulator::Schedule(Seconds(60), &Dect2020ExtMac::VerifyAssociatedStatus, this);

        // Store the associated FTs Long RD ID
        m_associatedFTNetDeviceLongRdId = transmitterAddress;
        NS_LOG_INFO(MAC_IDENTITY << ": " << Simulator::Now().GetMicroSeconds()
                    << ": Dect2020Mac::HandleUnicastPacket: Association Response by 0x" << std::hex
                    << this->GetLongRadioDeviceId() << " accepted");
    }
    else
    {
        // tbd
    }
}

void
Dect2020ExtMac::VerifyAssociatedStatus(void)
{
    Dect2020ExtFtCandidateInfo* ft = FindFtCandidateByLongId(m_associatedFTNetDeviceLongRdId);
    Time lastrBeaconReceived = ft->receptionTime;
    Time currentTime = Simulator::Now();
    Time VerifyAssociatedStatusPeriod = Seconds(60);
    NS_LOG_INFO(MAC_IDENTITY << ": ASSOCIATED");

    // Check if the last received Cluster Beacon is older than 60 seconds
    if (currentTime - lastrBeaconReceived > VerifyAssociatedStatusPeriod)
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

Dect2020ExtFtCandidateInfo*
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
    Dect2020ExtFtCandidateInfo newCandidate;
    m_ftCandidates.push_back(newCandidate);

    return &m_ftCandidates.back();
}

Time
Dect2020ExtMac::CalculcateTimeOffsetFromCurrentSubslot(uint32_t delayInSubslots)
{
    uint32_t subslotsPerFrame = m_channelManager->GetSubslotsPerSlot() * 24;
    uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    
    // Get consistent snapshot of PHY state to avoid timing issues
    uint32_t currentSfn = m_phy->GetCurrentSFN();
    uint32_t currentAbsSubslot = m_phy->GetCurrentAbsoluteSubslot();

    // Calculate target position based on the delay
    uint32_t targetAbsSubslot = currentAbsSubslot + delayInSubslots;

    // Calculate target SFN (wraps at 256)
    uint8_t targetSfn = (currentSfn + (targetAbsSubslot / subslotsPerFrame)) % 256;

    // Calculate slot and subslot within the target frame
    uint32_t subslotWithinFrame = targetAbsSubslot % subslotsPerFrame;
    uint32_t slot = subslotWithinFrame / subslotsPerSlot;
    uint32_t subslot = subslotWithinFrame % subslotsPerSlot;

    Time t = m_phy->GetTimeToNextAbsoluteSubslot(targetSfn, slot, subslot);

    return t;
}

void
Dect2020ExtMac::SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType terminationPointType)
{
    m_terminationPointType = terminationPointType;
    m_terminationPointTypeSet = true;
}

Dect2020ExtNetDevice::Dect2020ExtTerminationPointType
Dect2020ExtMac::GetTerminationPointType(void) const
{
    return m_terminationPointType;
}

void
Dect2020ExtMac::SetNetDevice(Ptr<Dect2020ExtNetDevice> device)
{
    m_device = device;
}

Ptr<Dect2020ExtNetDevice>
Dect2020ExtMac::GetNetDevice(void) const
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

/**
 * Assigns the PHY to the MAC
 */
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

void
Dect2020ExtMac::SetNetworkId(uint32_t networkId)
{
    m_networkId = networkId;
}

uint32_t
Dect2020ExtMac::GetNetworkId(void) const
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
Dect2020ExtMac::GetLongRadioDeviceId(void) const
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
Dect2020ExtMac::GetShortRadioDeviceId(void) const
{
    return m_shortRadioDeviceId;
}

uint32_t
Dect2020ExtMac::GetOperatingChannelId(void) const
{
    return m_operatingChannelId;
}

std::vector<Dect2020ExtChannelEvaluation>
Dect2020ExtMac::GetChannelEvaluations(void) const
{
    return m_channelEvaluations;
}

void
Dect2020ExtMac::SetCurrentChannelId(uint16_t channelId)
{
    m_currentChannelId = channelId;
}

uint16_t
Dect2020ExtMac::GetCurrentChannelId(void) const
{
    return m_currentChannelId;
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
        case Dect2020ExtAssociationStatus::NOT_ASSOCIATED: newStatus = "NOT_ASSOCIATED";
        case Dect2020ExtAssociationStatus::ASSOCIATION_PREPARING: newStatus = "ASSOCIATION_PREPARING";
        case Dect2020ExtAssociationStatus::WAITING_FOR_SELECTED_FT: newStatus = "WAITING_FOR_SELECTED_FT";
        case Dect2020ExtAssociationStatus::ASSOCIATION_PENDING: newStatus = "ASSOCIATION_PENDING";
        case Dect2020ExtAssociationStatus::ASSOCIATED: newStatus = "ASSOCIATED";
    }
    m_associationStatus = associationStatus;
    NS_LOG_INFO(MAC_IDENTITY << ": Association status changed to " << newStatus);
}

void
Dect2020ExtMac::SetBandNumber(uint8_t bandNumber)
{
    m_bandNumber = bandNumber;
    m_bandNumberSet = true;
}

uint8_t
Dect2020ExtMac::GetBandNumber(void) const
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
Dect2020ExtMac::GetSubcarrierScalingFactor(void) const
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
Dect2020ExtMac::ConfigureRandomAccessResources(Dect2020ExtRandomAccessResourceIE randomAccessResourceIE)
{
    m_configuredRandomAccessResourceIE = randomAccessResourceIE;
    m_configuredRandomAccessResourceIESet = true;
}

void
Dect2020ExtMac::PrintConfiguration(void) const
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

} // namespace ns3