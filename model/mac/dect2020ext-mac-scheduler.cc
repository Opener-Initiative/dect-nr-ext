#include "dect2020ext-mac-scheduler.h"

#include "ns3/dect2020ext-config-values.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

#include <cstdint>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtMacScheduler");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtMacScheduler);

static void
PrintFrameBanner(const std::string& label)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = (w.ws_col > 0) ? static_cast<int>(w.ws_col) : 80;
    int fillLen = width - static_cast<int>(label.size()) - 1;
    std::string fill = (fillLen > 0) ? " " + std::string(fillLen, '=') : "";
    NS_LOG_INFO(label << fill);
}

// ##################################
// #### Mac Scheduler Operation ####
// ##################################

Dect2020ExtMacSchedulerOperation::Dect2020ExtMacSchedulerOperation()
{
    operationType = OperationType::IDLE;
    resource = nullptr;
    periodFrames = 0;
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
        .SetGroupName("Dect2020Ext");

    return tid;
}

void
Dect2020ExtMacScheduler::Initialize()
{
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
            slot.slotNumber = slotNumber;
            slot.slotStartTimeNs = slotStartTime;
            slot.slotEndTimeNs = slotEndTime;

            for (uint32_t subslotIndex = 0; subslotIndex < numSubslotsPerSlot; subslotIndex++, subslotNumber++)
            {
                double subslotStartTime = subslotNumber * (SLOT_DURATION_NS / numSubslotsPerSlot);
                double subslotEndTime = (subslotNumber + 1) * (SLOT_DURATION_NS / numSubslotsPerSlot);

                Dect2020ExtMacSchedulerOperation operation;
                operation.operationType = Dect2020ExtMacSchedulerOperation::IDLE;
                operation.channelNumber = absoluteChannelNumber;
                operation.startSubslot = subslotNumber;
                operation.endSubslot = subslotNumber;

                Dect2020ExtMacSchedulerSubslot subslot;
                subslot.subslotIndex = subslotIndex;
                subslot.subslotNumber = subslotNumber;
                subslot.subslotStartTimeNs = subslotStartTime;
                subslot.subslotEndTimeNs = subslotEndTime;
                subslot.operation = operation;

                slot.subslots.push_back(subslot);
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
    NS_ABORT_MSG_IF(!m_bandNumberSet, "Band number is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_subcarrierScalingFactorSet, "Subcarrier scaling factor is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_fourierTransformScalingFactorSet, "Fourier transform scaling factor is not set for Dect2020ExtMacScheduler");
    NS_ABORT_MSG_IF(!m_channelManager, "Channel Manager is not set for Dect2020ExtMacScheduler");

    m_currentFrameStartTime = Simulator::Now();

    if (m_currentFrameStartTime != Seconds(0))
    {
        m_SFN++;
    }

    if (m_dummy)
    {
        PrintFrameBanner("FRAME START | SFN: " + std::to_string(m_SFN) + " | T: " + std::to_string(Simulator::Now().GetMilliSeconds()) + " ms");
    }

    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t numSubslotsPerFrame = numSubslotsPerSlot * SLOTS_PER_FRAME;

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
    m_macScheduler = m_nextMacScheduler;
    // Simulator::Schedule(MicroSeconds(FRAME_DURATION_US), &Dect2020ExtMacScheduler::PrintFrameEnd, this);
    Simulator::Schedule(MicroSeconds(FRAME_DURATION_US), &Dect2020ExtMacScheduler::StartMacScheduler, this);
}

void
Dect2020ExtMacScheduler::PrintFrameEnd()
{
    if (m_dummy)
    {
        PrintFrameBanner("FRAME END   | SFN: " + std::to_string(m_SFN) + " | T: " + std::to_string(Simulator::Now().GetMilliSeconds()) + " ms");
    }
}

void
Dect2020ExtMacScheduler::ProcessSlot(uint32_t slotNumber)
{
    NS_LOG_DEBUG("Processing slot " << slotNumber << " at time: " << Simulator::Now());
    m_currentSlotNumber = slotNumber;
}

void
Dect2020ExtMacScheduler::ProcessSubslot(uint32_t subslotNumber)
{
    NS_LOG_DEBUG("Processing subslot " << subslotNumber << " at time: " << Simulator::Now());
    m_currentSubslotNumber = subslotNumber;
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);

    for (uint16_t channelNumber = bandParams.nStart; channelNumber <= bandParams.nEnd; channelNumber++)
    {
        const auto& subslot = GetSubslotBySubslotNumber(channelNumber, subslotNumber);
        const auto& operation = subslot.operation;

        // Execute the operation if its type is not IDLE
        if (operation.operationType != Dect2020ExtMacSchedulerOperation::IDLE)
        {
            // Execute the operation periodically if its type is TX_NETWORK_BEACON or TX_CLUSTER_BEACON
            if (operation.operationType == Dect2020ExtMacSchedulerOperation::TX_NETWORK_BEACON
                || operation.operationType == Dect2020ExtMacSchedulerOperation::TX_CLUSTER_BEACON)
            {
                if ((m_SFN - operation.frameOffset) % operation.periodFrames == 0)
                {
                    operation.operation();
                }
            }
            else
            {
                operation.operation();
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
    NS_LOG_DEBUG("Elapsed in current frame: " << elapsedInCurrentFrameNs << "ns");

    // Calculate remaining time until next frame boundary
    int64_t timeToNextFrameNs = frameDurationNs - elapsedInCurrentFrameNs;
    NS_LOG_DEBUG("Current time: " << currentTime.GetNanoSeconds() << "ns, "
                        << "ns, Time to next frame: " << timeToNextFrameNs << "ns. " << "Current frame start time: " << m_currentFrameStartTime.GetNanoSeconds() << "ns");

    return static_cast<double>(timeToNextFrameNs);
}

double
Dect2020ExtMacScheduler::GetSlotStartTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    double slotStartTimeNs = m_macScheduler.at(bandParams.nStart).at(slotNumber).slotStartTimeNs;
    return slotStartTimeNs; 
}

double
Dect2020ExtMacScheduler::GetSlotEndTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).slotEndTimeNs;
}

double
Dect2020ExtMacScheduler::GetSubslotStartTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).subslots.at(subslotIndex).subslotStartTimeNs;
}

double
Dect2020ExtMacScheduler::GetSubslotEndTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(bandParams.nStart).at(slotNumber).subslots.at(subslotIndex).subslotEndTimeNs;
}

Dect2020ExtMacSchedulerSubslot
Dect2020ExtMacScheduler::GetSubslotBySubslotNumber(uint16_t channelNumber, uint32_t subslotNumber) const
{
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_macScheduler.at(channelNumber).at(slotNumber).subslots.at(subslotIndex);
}

double
Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs(uint32_t subslotNumber) const
{
    if (subslotNumber < m_currentSubslotNumber)
    {
        NS_FATAL_ERROR("The requested subslot number has already passed in the current frame. Requested subslot number: " << subslotNumber);
    }

    Time currentTime = Simulator::Now();

    double targetSubslotOffsetNs = GetSubslotStartTimeOffsetNs(subslotNumber);

    int64_t elapsedNs = (currentTime - m_currentFrameStartTime).GetNanoSeconds();
    auto timeToSubslotNs = static_cast<double>(static_cast<int64_t>(targetSubslotOffsetNs) - elapsedNs);

    return timeToSubslotNs;
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
Dect2020ExtMacScheduler::GetSfn() const
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
    if (operation.startSubslot == operation.endSubslot)
    {
        uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
        uint32_t subslotIndex = operation.startSubslot % numSubslotsPerSlot;
        uint32_t slotNumber = operation.startSubslot / numSubslotsPerSlot;
        m_nextMacScheduler.at(operation.channelNumber).at(slotNumber).subslots.at(subslotIndex).operation = operation;
    }
    else
    {
        // TBD
    }
}

void
Dect2020ExtMacScheduler::PrintConfiguration() const
{
    NS_LOG_DEBUG("Mac Scheduler Configuration:");
    NS_LOG_DEBUG("  Band Number: " << static_cast<uint16_t>(m_bandNumber));
    NS_LOG_DEBUG("  Subcarrier Scaling Factor: " << static_cast<uint16_t>(m_subcarrierScalingFactor));
    NS_LOG_DEBUG("  Fourier Transform Scaling Factor: " << static_cast<uint16_t>(m_fourierTransformScalingFactor));
}

void
Dect2020ExtMacScheduler::PrintMacScheduler() const
{
    std::cout << "Mac Scheduler:";
    for (auto it = m_macScheduler.begin(); it != m_macScheduler.end(); it++)
    {
        std::stringstream ss;
        ss << "Ch: " << it->first << " | Slots: " << it->second.size() << " | ";
        
        for (size_t slotIdx = 0; slotIdx < it->second.size(); slotIdx++)
        {
            const Dect2020ExtMacSchedulerSlot& slot = it->second[slotIdx];
            ss << "Slot[" << slot.slotNumber << ": " << slot.slotStartTimeNs << "-" 
               << slot.slotEndTimeNs << "ns, Subslots: " << slot.subslots.size() << "] { ";
            
            for (size_t subslotIdx = 0; subslotIdx < slot.subslots.size(); subslotIdx++)
            {
                const Dect2020ExtMacSchedulerSubslot& subslot = slot.subslots[subslotIdx];
                ss << "SS[idx:" << subslot.subslotIndex << " num:" << subslot.subslotNumber 
                   << " " << subslot.subslotStartTimeNs << "-" << subslot.subslotEndTimeNs << "ns] ";
            }
            ss << "} ";
        }
        std::cout << ss.str();
    }
}

void
Dect2020ExtMacScheduler::Dummy(bool value)
{
    m_dummy = value;
}

} // namespace ns3