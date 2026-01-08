#include "dect2020ext-execution-grid.h"

#include "ns3/dect2020ext-mac.h"

#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExecutionGrid");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtExecutionGrid);

// ##################################
// #### Execution Grid Operation ####
// ##################################

Dect2020ExtExecutionGridOperation::Dect2020ExtExecutionGridOperation()
{
    m_operationType = OperationType::IDLE;
    m_startSubslot = 0;
    m_endSubslot = 0;
    m_resource = nullptr;
}

Dect2020ExtExecutionGridOperation::~Dect2020ExtExecutionGridOperation()
{
}


void
Dect2020ExtExecutionGridOperation::SetOperationType(OperationType operationType)
{
    m_operationType = operationType;
}

Dect2020ExtExecutionGridOperation::OperationType
Dect2020ExtExecutionGridOperation::GetOperationType() const
{
    return m_operationType;
}

void
Dect2020ExtExecutionGridOperation::SetStartSubslot(uint32_t startSubslot)
{
    m_startSubslot = startSubslot;
}

uint32_t
Dect2020ExtExecutionGridOperation::GetStartSubslot() const
{
    return m_startSubslot;
}

void
Dect2020ExtExecutionGridOperation::SetEndSubslot(uint32_t endSubslot)
{
    m_endSubslot = endSubslot;
}

uint32_t
Dect2020ExtExecutionGridOperation::GetEndSubslot() const
{
    return m_endSubslot;
}

void
Dect2020ExtExecutionGridOperation::SetResource(Ptr<Packet> resource)
{
    m_resource = resource;
}

Ptr<Packet>
Dect2020ExtExecutionGridOperation::GetResource() const
{
    return m_resource;
}

// #########################################
// #### Dect2020ExtExecutionGridSubslot ####
// #########################################

Dect2020ExtExecutionGridSubslot::Dect2020ExtExecutionGridSubslot()
{
}

Dect2020ExtExecutionGridSubslot::~Dect2020ExtExecutionGridSubslot()
{
}

// ######################################
// #### Dect2020ExtExecutionGridSlot ####
// ######################################

Dect2020ExtExecutionGridSlot::Dect2020ExtExecutionGridSlot()
{
}

Dect2020ExtExecutionGridSlot::~Dect2020ExtExecutionGridSlot()
{
}

// ########################
// #### Execution Grid ####
// ########################

Dect2020ExtExecutionGrid::Dect2020ExtExecutionGrid()
{
}

Dect2020ExtExecutionGrid::~Dect2020ExtExecutionGrid()
{
}

TypeId
Dect2020ExtExecutionGrid::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtExecutionGrid")
        .SetParent<Object>()
        .AddConstructor<Dect2020ExtExecutionGrid>()
        .SetGroupName("Dect2020Ext")

        .AddAttribute("config_BandNumber",
                        "DECT-2020 band number configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtExecutionGrid::m_bandNumber),
                        MakeUintegerChecker<uint8_t>(1, 21))

        .AddAttribute("config_SubcarrierScalingFactor",
                        "DECT-2020 subcarrier scaling factor configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtExecutionGrid::m_subcarrierScalingFactor),
                        MakeUintegerChecker<uint8_t>(1, 8))

        .AddAttribute("config_FourierTransformScalingFactor",
                        "DECT-2020 Fourier transform scaling factor configuration",
                        UintegerValue(1),
                        MakeUintegerAccessor(&Dect2020ExtExecutionGrid::m_fourierTransformScalingFactor),
                        MakeUintegerChecker<uint8_t>(1, 8))

        .AddAttribute("config_ChannelManager",
                        "Dect2020ExtChannelManager configuration",
                        PointerValue(nullptr),
                        MakePointerAccessor(&Dect2020ExtExecutionGrid::m_channelManager),
                        MakePointerChecker<Dect2020ExtChannelManager>());
    return tid;
}

void
Dect2020ExtExecutionGrid::Initialize()
{
    NS_LOG_FUNCTION(this);

    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot(); // Get the number of subslots per slot based on the subcarrier scaling factor
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber); // Get current band parameters
    uint16_t numChannels = (bandParams.nEnd - bandParams.nStart) + 1; // Number of channels

    // Initialize the execution grid with slots and subslot for each channel
    for (uint16_t ch = 0; ch < numChannels; ch++)
    {

        uint16_t absoluteChannelNumber = bandParams.nStart + ch;
        uint32_t subslotNumber = 0;
        std::vector<Dect2020ExtExecutionGridSlot> slots;

        for (uint32_t slotNumber = 0; slotNumber < SLOTS_PER_FRAME; slotNumber++)
        {
            double slotStartTime = slotNumber * SLOT_DURATION_NS;
            double slotEndTime = (slotNumber + 1) * SLOT_DURATION_NS;
            Dect2020ExtExecutionGridSlot slot;
            slot.m_slotNumber = slotNumber;
            slot.m_slotStartTimeNs = slotStartTime;
            slot.m_slotEndTimeNs = slotEndTime;

            for (uint32_t subslotIndex = 0; subslotIndex < numSubslotsPerSlot; subslotIndex++, subslotNumber++)
            {
                double subslotStartTime = subslotNumber * (SLOT_DURATION_NS / numSubslotsPerSlot);
                double subslotEndTime = (subslotNumber + 1) * (SLOT_DURATION_NS / numSubslotsPerSlot);
                Dect2020ExtExecutionGridSubslot subslot;
                subslot.m_subslotIndex = subslotIndex;
                subslot.m_subslotNumber = subslotNumber;
                subslot.m_subslotStartTimeNs = subslotStartTime;
                subslot.m_subslotEndTimeNs = subslotEndTime;

                slot.m_subslots.push_back(subslot);
            }
            slots.push_back(slot);
        }
        m_executionGrid[absoluteChannelNumber] = slots;
    }
    m_nextExecutionGrid = m_executionGrid;
}

void
Dect2020ExtExecutionGrid::StartExecutionGrid()
{
    NS_LOG_FUNCTION(this);

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
        Simulator::Schedule(NanoSeconds(slotStartTime), &Dect2020ExtExecutionGrid::ProcessSlot, this, slotNumber);
    }

    for (uint32_t subslotNumber = 0; subslotNumber < numSubslotsPerFrame; subslotNumber++)
    {
        double subslotStartTime = subslotNumber * (SLOT_DURATION_NS / numSubslotsPerSlot);
        Simulator::Schedule(NanoSeconds(subslotStartTime), &Dect2020ExtExecutionGrid::ProcessSubslot, this, subslotNumber);
    }

    ScheduleNextExecutionGrid();
}

void
Dect2020ExtExecutionGrid::ScheduleNextExecutionGrid()
{
    NS_LOG_FUNCTION(this);
    m_executionGrid = m_nextExecutionGrid;
    Simulator::Schedule(MicroSeconds(FRAME_DURATION_US), &Dect2020ExtExecutionGrid::StartExecutionGrid, this);
}

void
Dect2020ExtExecutionGrid::ProcessSlot(uint32_t slotNumber)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Processing slot " << slotNumber << " at time: " << Simulator::Now());
    m_currentSlotNumber = slotNumber;
}

void
Dect2020ExtExecutionGrid::ProcessSubslot(uint32_t subslotNumber)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Processing subslot " << subslotNumber << " at time: " << Simulator::Now());
    m_currentSubslotNumber = subslotNumber;
}

double
Dect2020ExtExecutionGrid::GetTimeTillNextFrameNs() const
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
Dect2020ExtExecutionGrid::GetSlotStartTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    double slotStartTimeNs = m_executionGrid.at(bandParams.nStart).at(slotNumber).m_slotStartTimeNs;
    // NS_LOG_INFO("Slot start time: " << slotStartTimeNs << "ns");
    return slotStartTimeNs; 
}

double
Dect2020ExtExecutionGrid::GetSlotEndTimeOffsetNs(uint32_t slotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    return m_executionGrid.at(bandParams.nStart).at(slotNumber).m_slotEndTimeNs;
}

double
Dect2020ExtExecutionGrid::GetSubslotStartTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_executionGrid.at(bandParams.nStart).at(slotNumber).m_subslots.at(subslotIndex).m_subslotStartTimeNs;
}

double
Dect2020ExtExecutionGrid::GetSubslotEndTimeOffsetNs(uint32_t subslotNumber) const
{
    BandParameters bandParams = m_channelManager->GetBandParameters(m_bandNumber);
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t subslotIndex = subslotNumber % numSubslotsPerSlot; // Get the index of the subslot in the slot
    uint32_t slotNumber = subslotNumber / numSubslotsPerSlot; // Get the slot number
    return m_executionGrid.at(bandParams.nStart).at(slotNumber).m_subslots.at(subslotIndex).m_subslotEndTimeNs;
}

void
Dect2020ExtExecutionGrid::ScheduleOperation(Dect2020ExtExecutionGridOperation operation)
{

}

std::map<uint16_t, std::vector<Dect2020ExtExecutionGridSlot>>
Dect2020ExtExecutionGrid::GetExecutionGrid() const
{
    return m_executionGrid;
}

void
Dect2020ExtExecutionGrid::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
}

void
Dect2020ExtExecutionGrid::PrintConfiguration() const
{
    NS_LOG_INFO("Execution Grid Configuration:");
    NS_LOG_INFO("  Band Number: " << static_cast<uint16_t>(m_bandNumber));
    NS_LOG_INFO("  Subcarrier Scaling Factor: " << static_cast<uint16_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO("  Fourier Transform Scaling Factor: " << static_cast<uint16_t>(m_fourierTransformScalingFactor));
}

void
Dect2020ExtExecutionGrid::PrintExecutionGrid() const
{
    NS_LOG_INFO("Execution Grid:");
    for (auto it = m_executionGrid.begin(); it != m_executionGrid.end(); it++)
    {
        std::stringstream ss;
        ss << "Ch: " << it->first << " | Slots: " << it->second.size() << " | ";
        
        for (size_t slotIdx = 0; slotIdx < it->second.size(); slotIdx++)
        {
            const Dect2020ExtExecutionGridSlot& slot = it->second[slotIdx];
            ss << "Slot[" << slot.m_slotNumber << ": " << slot.m_slotStartTimeNs << "-" 
               << slot.m_slotEndTimeNs << "ns, Subslots: " << slot.m_subslots.size() << "] { ";
            
            for (size_t subslotIdx = 0; subslotIdx < slot.m_subslots.size(); subslotIdx++)
            {
                const Dect2020ExtExecutionGridSubslot& subslot = slot.m_subslots[subslotIdx];
                ss << "SS[idx:" << subslot.m_subslotIndex << " num:" << subslot.m_subslotNumber 
                   << " " << subslot.m_subslotStartTimeNs << "-" << subslot.m_subslotEndTimeNs << "ns] ";
            }
            ss << "} ";
        }
        NS_LOG_INFO(ss.str());
    }
}

} // namespace ns3