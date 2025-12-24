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

    /**
     * Initialize the execution grid.
     */
    void Initialize();

    // Setters and Getters
    std::map<uint16_t, std::vector<Dect2020ExtExecutionGridOperation>> GetExecutionGrid() const;

    // Configuration
    void PrintConfiguration() const;

    void PrintExecutionGrid() const;

private:
    /**
     * The execution grid for variable number of subslots..
     * The key is the absolute channel number , the value is a vector with the size of the number of subslots consisting of ExecutionGridOperation objects.
     */
    std::map<uint16_t, std::vector<Dect2020ExtExecutionGridOperation>> m_executionGrid;
    uint8_t m_bandNumber;
    uint8_t m_subcarrierScalingFactor;
    uint8_t m_fourierTransformScalingFactor;
    Ptr<Dect2020ExtChannelManager> m_channelManager;
};

} // namespace ns3

#endif // DECT2020EXT_EXECUTION_GRID_H