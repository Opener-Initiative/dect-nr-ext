#ifndef DECT2020EXT_EXECUTION_GRID_H
#define DECT2020EXT_EXECUTION_GRID_H

#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/packet.h"

#include <map>
#include <vector>

namespace ns3
{

// ##############################
// #### Forward Declarations ####
// ##############################

class Dect2020ExtChannelManager;

// ##################################
// #### Execution Grid Operation ####
// ##################################

class Dect2020ExtExecutionGridOperation
{
public:
    enum OperationType
    {
        IDLE
    };

    Dect2020ExtExecutionGridOperation();
    virtual ~Dect2020ExtExecutionGridOperation();

    // Setters and Getters
    void SetOperationType(OperationType operationType);
    OperationType GetOperationType() const;
    void SetStartSubslot(uint32_t startSubslot);
    uint32_t GetStartSubslot() const;
    void SetEndSubslot(uint32_t endSubslot);
    uint32_t GetEndSubslot() const;
    void SetResource(Ptr<Packet> resource);
    Ptr<Packet> GetResource() const;

private:
    OperationType m_operationType;
    uint32_t m_startSubslot;
    uint32_t m_endSubslot;
    Ptr<Packet> m_resource;
    //std::function m_operation;
};

// ########################
// #### Execution Grid ####
// ########################

class Dect2020ExtExecutionGrid : public Object
{
public:
    Dect2020ExtExecutionGrid();
    virtual ~Dect2020ExtExecutionGrid();

    static TypeId GetTypeId();

    // Initialization

    void Initialize();

    // Execution Grid Operations
    void StartExecutionGrid();
    void ScheduleNextExecutionGrid();
    void ProcessSubslot();

    // Setters and Getters
    std::map<uint16_t, std::vector<Dect2020ExtExecutionGridOperation>> GetExecutionGrid() const;

    // Configuration
    void PrintConfiguration() const;

    void PrintExecutionGrid() const;

private:
    std::map<uint16_t, std::vector<Dect2020ExtExecutionGridOperation>> m_executionGrid;
    std::map<uint16_t, std::vector<Dect2020ExtExecutionGridOperation>> m_nextExecutionGrid;
    Time m_frameStartTime;

    // Configuration

    uint8_t m_bandNumber;
    uint8_t m_subcarrierScalingFactor;
    uint8_t m_fourierTransformScalingFactor;
    Ptr<Dect2020ExtChannelManager> m_channelManager;

    // Constants
    const uint32_t SLOTS_PER_FRAME = 24;
    const double SLOT_DURATION_NS = 416666; // 0,41667 milliseconds in nanoseconds
    const uint32_t FRAME_DURATION_US = 10000; // 10 milliseconds in microseconds
};

} // namespace ns3

#endif // DECT2020EXT_EXECUTION_GRID_H