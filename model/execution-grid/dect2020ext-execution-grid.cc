#include "dect2020ext-execution-grid.h"

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
    uint32_t numChannels = (bandParams.nEnd - bandParams.nStart) + 1; // Number of channels

    // Initialize the execution grid with slots and subslot for each channel
    for (uint32_t ch = 0; ch < numChannels; ch++)
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
    // TODO: Initialize the next execution grid
}

void
Dect2020ExtExecutionGrid::StartExecutionGrid()
{
    NS_LOG_FUNCTION(this);

    m_frameStartTime = Simulator::Now();
    uint32_t numSubslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
    uint32_t numSubslotsPerChannel = numSubslotsPerSlot * SLOTS_PER_FRAME;

    uint32_t subslot = 0;

    // Schedule all subslots for this frame
    for (; subslot < numSubslotsPerChannel; subslot++)
    {
        double slotStartTime = subslot * (SLOT_DURATION_NS / numSubslotsPerSlot);
        Simulator::Schedule(NanoSeconds(slotStartTime), &Dect2020ExtExecutionGrid::ProcessSubslot, this);
    }
    NS_LOG_INFO("Scheduled " << subslot << " subslots for this frame");
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
Dect2020ExtExecutionGrid::ProcessSubslot()
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("Processing subslot at time: " << Simulator::Now());
}

std::map<uint16_t, std::vector<Dect2020ExtExecutionGridSlot>>
Dect2020ExtExecutionGrid::GetExecutionGrid() const
{
    return m_executionGrid;
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