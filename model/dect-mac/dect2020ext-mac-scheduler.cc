#include "dect2020ext-mac-scheduler.h"

#include "ns3/dect2020ext-mac.h"

#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

#include <cstdint>
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacScheduler");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacScheduler);

// ##################################
// #### Mac Scheduler Operation ####
// ##################################

Dect2020ExtMacSchedulerOperation::Dect2020ExtMacSchedulerOperation()
{
    m_operationType = OperationType::IDLE;
    m_resource = nullptr;
    m_periodFrames = 0;
}

Dect2020ExtMacSchedulerOperation::~Dect2020ExtMacSchedulerOperation()
{
}


// ###############################
// #### Mac Scheduler Subslot ####
// ###############################

Dect2020ExtMacSchedulerSubslot::Dect2020ExtMacSchedulerSubslot()
{
}

Dect2020ExtMacSchedulerSubslot::~Dect2020ExtMacSchedulerSubslot()
{
}

// ############################
// #### Mac Scheduler Slot ####
// ############################

Dect2020ExtMacSchedulerSlot::Dect2020ExtMacSchedulerSlot()
{
}

Dect2020ExtMacSchedulerSlot::~Dect2020ExtMacSchedulerSlot()
{
}

// ########################
// #### Mac Scheduler ####
// ########################

Dect2020ExtMacScheduler::Dect2020ExtMacScheduler()
{
}

Dect2020ExtMacScheduler::~Dect2020ExtMacScheduler()
{
}

TypeId
Dect2020ExtMacScheduler::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtMacScheduler")
        .SetParent<Object>()
        .AddConstructor<Dect2020ExtMacScheduler>()
        .SetGroupName("Dect2020Ext")

        .AddAttribute("config_bandNumber",
                        "DECT-2020 band number configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtMacScheduler::m_bandNumber),
                        MakeUintegerChecker<uint8_t>(1, 21))

        .AddAttribute("config_subcarrierScalingFactor",
                        "DECT-2020 subcarrier scaling factor configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtMacScheduler::m_subcarrierScalingFactor),
                        MakeUintegerChecker<uint8_t>(1, 8))

        .AddAttribute("config_fourierTransformScalingFactor",
                        "DECT-2020 Fourier transform scaling factor configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtMacScheduler::m_fourierTransformScalingFactor),
                        MakeUintegerChecker<uint8_t>(1, 8))

        .AddAttribute("config_channelManager",
                        "Dect2020ExtChannelManager configuration",
                        PointerValue(nullptr),
                        MakePointerAccessor(&Dect2020ExtMacScheduler::m_channelManager),
                        MakePointerChecker<Dect2020ExtChannelManager>());
    return tid;
}

void
Dect2020ExtMacScheduler::Initialize()
{
    NS_LOG_FUNCTION(this);

    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot(); // Get the number of subslots per slot based on the subcarrier scaling factor
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber); // Get current band parameters
    uint16_t numChannels = (bandParams.nEnd - bandParams.nStart) + 1; // Number of channels

    // Initialize the mac scheduler with slots and subslots for each channel
    for (uint16_t ch = 0; ch < numChannels; ch++)
    {

        uint16_t absoluteChannelNumber = bandParams.nStart + ch;
        uint32_t subslotNumber = 0;
        std::vector<Dect2020ExtMacSchedulerSlot> slots;

        for (uint32_t slotNumber = 0; slotNumber < SLOTS_PER_FRAME; slotNumber++)
        {
            double slotStartTime = slotNumber * SLOT_DURATION_NS;
            double slotEndTime = (slotNumber + 1) * SLOT_DURATION_NS;
            Dect2020ExtMacSchedulerSlot slot;
            slot.m_slotNumber = slotNumber;
            slot.m_slotStartTimeNs = slotStartTime;
            slot.m_slotEndTimeNs = slotEndTime;

            for (uint32_t subslotIndex = 0; subslotIndex < numSubslotsPerSlot; subslotIndex++, subslotNumber++)
            {
                double subslotStartTime = subslotNumber * (SLOT_DURATION_NS / numSubslotsPerSlot);
                double subslotEndTime = (subslotNumber + 1) * (SLOT_DURATION_NS / numSubslotsPerSlot);

                Dect2020ExtMacSchedulerOperation operation;
                operation.m_operationType = Dect2020ExtMacSchedulerOperation::IDLE;
                operation.m_channelNumber = absoluteChannelNumber;
                operation.m_startSubslot = subslotNumber;
                operation.m_endSubslot = subslotNumber;

                Dect2020ExtMacSchedulerSubslot subslot;
                subslot.m_subslotIndex = subslotIndex;
                subslot.m_subslotNumber = subslotNumber;
                subslot.m_subslotStartTimeNs = subslotStartTime;
                subslot.m_subslotEndTimeNs = subslotEndTime;
                subslot.operation = operation;

                slot.m_subslots.push_back(subslot);
            }
            slots.push_back(slot);
        }
        m_macScheduler[absoluteChannelNumber] = slots;
    }
    m_nextMacScheduler = m_macScheduler;
}

void
Dect2020ExtMacScheduler::StartMacScheduler()
{
    NS_LOG_FUNCTION(this);

    NS_ABORT_MSG_IF(!m_bandNumberSet, "Band number is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_subcarrierScalingFactorSet, "Subcarrier scaling factor is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_fourierTransformScalingFactorSet, "Fourier transform scaling factor is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_channelManager, "Channel Manager is not set for Dect2020ExtMacScheduler");

    m_currentFrameStartTime = Simulator::Now();
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t numSubslotsPerFrame = numSubslotsPerSlot * SLOTS_PER_FRAME;

    // Update SFN if m_currentFrameStartTime is not equal to 0
    if (m_currentFrameStartTime != Seconds(0))
    {
        m_SFN++;
    }

    for (uint32_t slotNumber = 0; slotNumber < SLOTS_PER_FRAME; slotNumber++)
    {
        double slotStartTime = slotNumber * SLOT_DURATION_NS;
        Simulator::Schedule(NanoSeconds(slotStartTime), &Dect2020ExtMacScheduler::ProcessSlot, this, slotNumber);
    }

    for (uint32_t subslotNumber = 0; subslotNumber < numSubslotsPerFrame; subslotNumber++)
    {
        double subslotStartTime = subslotNumber * (SLOT_DURATION_NS / numSubslotsPerSlot);
        Simulator::Schedule(NanoSeconds(subslotStartTime), &Dect2020ExtMacScheduler::ProcessSubslot, this, subslotNumber);
    }

    ScheduleNextMacScheduler();
}

void
Dect2020ExtMacScheduler::ScheduleNextMacScheduler()
{
    NS_LOG_FUNCTION(this);
    m_macScheduler = m_nextMacScheduler;
    Simulator::Schedule(MicroSeconds(FRAME_DURATION_US), &Dect2020ExtMacScheduler::StartMacScheduler, this);
}

void
Dect2020ExtMacScheduler::ProcessSlot(uint32_t slotNumber)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Processing slot " << slotNumber << " at time: " << Simulator::Now());
    m_currentSlotNumber = slotNumber;
}

void
Dect2020ExtMacScheduler::ProcessSubslot(uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Processing subslot " << subslotNumber << " at time: " << Simulator::Now());
    m_currentSubslotNumber = subslotNumber;
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);

    for (uint16_t channelNumber = bandParams.nStart; channelNumber <= bandParams.nEnd; channelNumber++)
    {
        const auto& subslot = GetSubslotBySubslotNumber(channelNumber, subslotNumber);
        const auto& operation = subslot.operation;

        // Execute the operation if its type is not IDLE
        if (operation.m_operationType != Dect2020ExtMacSchedulerOperation::IDLE)
        {
            // Execute the operation periodically if its type is TX_NETWORK_BEACON or TX_CLUSTER_BEACON
            if (operation.m_operationType == Dect2020ExtMacSchedulerOperation::TX_NETWORK_BEACON
                || operation.m_operationType == Dect2020ExtMacSchedulerOperation::TX_CLUSTER_BEACON)
            {
                if ((m_SFN - operation.m_frameOffset) % operation.m_periodFrames == 0)
                {
                    operation.m_operation();
                }
            }
            else
            {
                operation.m_operation();
            }
        }
    }
}

double
Dect2020ExtMacScheduler::GetTimeTillNextFrameNs() const
{
    Time currentTime = Simulator::Now();
    Time elapsedSinceCurrentFrameStartTime = currentTime - m_currentFrameStartTime;
    Time frameDuration = MicroSeconds(FRAME_DURATION_US);

    // Get elapsed time in nanoseconds and calculate position within the current frame
    int64_t elapsedNs = elapsedSinceCurrentFrameStartTime.GetNanoSeconds();
    int64_t frameDurationNs = frameDuration.GetNanoSeconds();

    // Handle case where multiple frames have passed (module operation)
    int64_t elapsedInCurrentFrameNs = elapsedNs % frameDurationNs;
    NS_LOG_INFO("Elapsed in current frame: " << elapsedInCurrentFrameNs << "ns");

    // Calculate remaining time until next frame boundary
    int64_t timeToNextFrameNs = frameDurationNs - elapsedInCurrentFrameNs;
    NS_LOG_INFO("Current time: " << currentTime.GetNanoSeconds() << "ns, "
                        << "ns, Time to next frame: " << timeToNextFrameNs << "ns. " << "Current frame start time: " << m_currentFrameStartTime.GetNanoSeconds() << "ns");

    return static_cast<double>(timeToNextFrameNs);
}

double
Dect2020ExtMacScheduler::GetSlotStartTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    double slotStartTimeNs = m_macScheduler.at(bandParams.nStart).at(slotNumber).m_slotStartTimeNs;
    // NS_LOG_INFO("Slot start time: " << slotStartTimeNs << "ns");
    return slotStartTimeNs; 
}

double
Dect2020ExtMacScheduler::GetSlotEndTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).m_slotEndTimeNs;
}

double
Dect2020ExtMacScheduler::GetSubslotStartTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).m_subslots.at(subslotIndex).m_subslotStartTimeNs;
}

double
Dect2020ExtMacScheduler::GetSubslotEndTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).m_subslots.at(subslotIndex).m_subslotEndTimeNs;
}

Dect2020ExtMacSchedulerSubslot
Dect2020ExtMacScheduler::GetSubslotBySubslotNumber(uint16_t channelNumber, uint32_t subslotNumber) const
{
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(channelNumber).at(slotNumber).m_subslots.at(subslotIndex);
}

double
Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs(uint32_t subslotNumber) const
{
    Time currentTime = Simulator::Now();
    Time frameDuration = MicroSeconds(FRAME_DURATION_US);

    // Get the offset of the target subslot within a frame
    double targetSubslotOffsetNs = GetSubslotStartTimeOffsetNs(subslotNumber);

    // Calculate elapsed time since current frame started
    Time elapsedSinceFrameStart = currentTime - m_currentFrameStartTime;

    int64_t elapsedNs = elapsedSinceFrameStart.GetNanoSeconds();
    int64_t frameDurationNs = frameDuration.GetNanoSeconds();

    // Handle case where multiple frames have passed (modulo operation)
    int64_t elapsedInCurrentFrameNs = elapsedNs % frameDurationNs;

    // Calculate the absolute time of the target subslot start within current frame
    auto targetSubslotAbsoluteTimeNs = static_cast<int64_t>(targetSubslotOffsetNs);

    // Check if target subslot is in the future within current frame
    if (targetSubslotAbsoluteTimeNs > elapsedInCurrentFrameNs)
    {
        // Target subslot hasn't happened yet in current frame
        double timeToSubslotNs = targetSubslotAbsoluteTimeNs - elapsedInCurrentFrameNs;
        NS_LOG_INFO(timeToSubslotNs / 1000000000);
        return timeToSubslotNs;
    }
    else
    {
        // Target subslot has already passed in current frame, calculate for next frame
        double timeToNextFrameNe = frameDurationNs - elapsedInCurrentFrameNs;
        double timeToSubslotNs = timeToNextFrameNe + targetSubslotOffsetNs;
        NS_LOG_INFO(timeToSubslotNs / 1000000000);
        return timeToSubslotNs;
    }
}

uint32_t
Dect2020ExtMacScheduler::GetCurrentSlotNumber() const
{
    return m_currentSlotNumber;
}

uint32_t
Dect2020ExtMacScheduler::GetCurrentSubslotNumber() const
{
    return m_currentSubslotNumber;
}

uint32_t
Dect2020ExtMacScheduler::GetSFN() const
{
    return m_SFN;
}

std::map<uint16_t, std::vector<Dect2020ExtMacSchedulerSlot>>
Dect2020ExtMacScheduler::GetMacScheduler() const
{
    return m_macScheduler;
}

void
Dect2020ExtMacScheduler::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
}

void
Dect2020ExtMacScheduler::SetBandNumber(uint8_t bandNumber)
{
    m_bandNumber = bandNumber;
    m_bandNumberSet = true;
}

uint8_t
Dect2020ExtMacScheduler::GetBandNumber() const
{
    return m_bandNumber;
}

void
Dect2020ExtMacScheduler::SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor)
{
    m_subcarrierScalingFactor = subcarrierScalingFactor;
    m_subcarrierScalingFactorSet = true;
}

uint8_t
Dect2020ExtMacScheduler::GetSubcarrierScalingFactor() const
{
    return m_subcarrierScalingFactor;
}

void
Dect2020ExtMacScheduler::SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor)
{
    m_fourierTransformScalingFactor = fourierTransformScalingFactor;
    m_fourierTransformScalingFactorSet = true;
}

uint8_t
Dect2020ExtMacScheduler::GetFourierTransformScalingFactor() const
{
    return m_fourierTransformScalingFactor;
}

void
Dect2020ExtMacScheduler::SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager)
{
    m_channelManager = channelManager;
}

Ptr<Dect2020ExtChannelManager>
Dect2020ExtMacScheduler::GetChannelManager() const
{
    return m_channelManager;
}

void
Dect2020ExtMacScheduler::ScheduleOperationOnNextFrame(Dect2020ExtMacSchedulerOperation operation)
{
    if (operation.m_startSubslot == operation.m_endSubslot)
    {
        uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
        uint32_t subslotIndex = operation.m_startSubslot % numSubslotsPerSlot;
        uint32_t slotNumber = operation.m_startSubslot / numSubslotsPerSlot;
        m_nextMacScheduler.at(operation.m_channelNumber).at(slotNumber).m_subslots.at(subslotIndex).operation = operation;
    }
    else
    {
        // TBD
    }
}

void
Dect2020ExtMacScheduler::PrintConfiguration() const
{
    NS_LOG_INFO("Mac Scheduler Configuration:");
    NS_LOG_INFO("  Band Number: " << static_cast<uint16_t>(m_bandNumber));
    NS_LOG_INFO("  Subcarrier Scaling Factor: " << static_cast<uint16_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO("  Fourier Transform Scaling Factor: " << static_cast<uint16_t>(m_fourierTransformScalingFactor));
}

void
Dect2020ExtMacScheduler::PrintMacScheduler() const
{
    NS_LOG_INFO("Mac Scheduler:");
    for (auto it = m_macScheduler.begin(); it != m_macScheduler.end(); it++)
    {
        std::stringstream ss;
        ss << "Ch: " << it->first << " | Slots: " << it->second.size() << " | ";
        
        for (size_t slotIdx = 0; slotIdx < it->second.size(); slotIdx++)
        {
            const Dect2020ExtMacSchedulerSlot& slot = it->second[slotIdx];
            ss << "Slot[" << slot.m_slotNumber << ": " << slot.m_slotStartTimeNs << "-" 
               << slot.m_slotEndTimeNs << "ns, Subslots: " << slot.m_subslots.size() << "] { ";
            
            for (size_t subslotIdx = 0; subslotIdx < slot.m_subslots.size(); subslotIdx++)
            {
                const Dect2020ExtMacSchedulerSubslot& subslot = slot.m_subslots[subslotIdx];
                ss << "SS[idx:" << subslot.m_subslotIndex << " num:" << subslot.m_subslotNumber 
                   << " " << subslot.m_subslotStartTimeNs << "-" << subslot.m_subslotEndTimeNs << "ns] ";
            }
            ss << "} ";
        }
        NS_LOG_INFO(ss.str());
    }
}

} // namespace ns3