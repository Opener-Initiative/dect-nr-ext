#ifndef DECT2020EXT_MAC_SCHEDULER_H 
#define DECT2020EXT_MAC_SCHEDULER_H 

#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/packet.h"

#include <map>
#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtMac;

// #################################
// #### Mac Scheduler Operation ####
// #################################

class Dect2020ExtMacSchedulerOperation
{
    public:
        enum OperationType
        {
            TEST, // Only for testing !!!
            IDLE,
            TX_NETWORK_BEACON,
            TX_CLUSTER_BEACON
        };

        Dect2020ExtMacSchedulerOperation();
        virtual ~Dect2020ExtMacSchedulerOperation();

        OperationType operationType;
        uint16_t channelNumber;
        uint32_t startSubslot;
        uint32_t endSubslot;
        Ptr<Packet> resource;
        std::function<void()> operation;
        uint32_t periodFrames;
        uint32_t frameOffset;
};

// ###############################
// #### Mac Scheduler Subslot ####
// ###############################

class Dect2020ExtMacSchedulerSubslot
{
    public:
        Dect2020ExtMacSchedulerSubslot();
        virtual ~Dect2020ExtMacSchedulerSubslot();

        double subslotStartTimeNs; // Start time of the subslot in nanoseconds
        double subslotEndTimeNs; // End time of the subslot in nanoseconds
        uint32_t subslotIndex; // Index of the subslot in the slot
        uint32_t subslotNumber; // Number of the subslot in the channel
        Dect2020ExtMacSchedulerOperation operation;
};

// ############################
// #### Mac Scheduler Slot ####
// ############################

class Dect2020ExtMacSchedulerSlot
{
    public:
        Dect2020ExtMacSchedulerSlot();
        virtual ~Dect2020ExtMacSchedulerSlot();
        
        std::vector<Dect2020ExtMacSchedulerSubslot> subslots;
        double slotStartTimeNs; // Start time of the slot in nanoseconds
        double slotEndTimeNs; // End time of the slot in nanoseconds
        uint32_t slotNumber; // Number of the slot in the channel
};

// ########################
// #### Mac Scheduler ####
// ########################

class Dect2020ExtMacScheduler : public Object
{
    public:
        Dect2020ExtMacScheduler();
        virtual ~Dect2020ExtMacScheduler();

        static TypeId GetTypeId();

        // Initialization

        void Initialize();

        // Mac Scheduler Operations

        void StartMacScheduler();
        void ScheduleNextMacScheduler();
        void PrintFrameEnd();
        void ProcessSlot(uint32_t slotNumber);
        void ProcessSubslot(uint32_t subslotNumber);

        // Time and Slot/Subslot Getters

        double GetTimeTillNextFrameNs() const;

        // Relative to the frame
        double GetSlotStartTimeOffsetNs(uint32_t slotNumber) const;

        // Relative to the frame
        double GetSlotEndTimeOffsetNs(uint32_t slotNumber) const;

        // Relative to the frame
        double GetSubslotStartTimeOffsetNs(uint32_t subslotNumber) const;

        // Relative to the frame
        double GetSubslotEndTimeOffsetNs(uint32_t subslotNumber) const;

        // Get MAC Scheduler subslot (Dect2020ExtMacSchedulerSubslot) by the absolute subslot number
        Dect2020ExtMacSchedulerSubslot GetSubslotBySubslotNumber(uint16_t channelNumber, uint32_t subslotNumber) const;

        // Get time in nanoseconds till subslot start.
        // Asserts if the subslot is in the current frame and has not passed yet.
        double GetTimeTillSubslotStartNs(uint32_t subslotNumber) const;

        // Get time in nanoseconds till subslot end.
        // Asserts if the subslot is in the current frame and has not passed yet.
        double GetTimeTillSubslotEndNs(uint32_t subslotNumber) const;

        // Get the current slot number (m_currentSlotNumber)
        uint32_t GetCurrentSlotNumber() const;

        // Get the current subslot number (m_currentSubslotNumber)
        uint32_t GetCurrentSubslotNumber() const;

        // Setters and Getters

        uint32_t GetSfn() const;

        std::map<uint16_t, std::vector<Dect2020ExtMacSchedulerSlot>> GetMacScheduler() const;

        void SetMac(Ptr<Dect2020ExtMac> mac);

        void SetBandNumber(uint8_t bandNumber);
        uint8_t GetBandNumber() const;

        void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
        uint8_t GetSubcarrierScalingFactor() const;

        void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
        uint8_t GetFourierTransformScalingFactor() const;

        void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
        Ptr<Dect2020ExtChannelManager> GetChannelManager() const;

        // Scheduling

        void ScheduleOperationOnNextFrame(Dect2020ExtMacSchedulerOperation operation);

        // Configuration

        void PrintConfiguration() const;
        void PrintMacScheduler() const;
        void Dummy(bool value);

    private:
        std::map<uint16_t, std::vector<Dect2020ExtMacSchedulerSlot>> m_macScheduler;
        std::map<uint16_t, std::vector<Dect2020ExtMacSchedulerSlot>> m_nextMacScheduler;
        Time m_currentFrameStartTime;
        uint32_t m_SFN = 0; // System Frame Number
        Ptr<Dect2020ExtMac> m_mac;
        uint32_t m_currentSlotNumber;
        uint32_t m_currentSubslotNumber;

        // Configuration

        uint8_t m_bandNumber;
        bool m_bandNumberSet = false;

        uint8_t m_subcarrierScalingFactor;
        bool m_subcarrierScalingFactorSet = false;

        uint8_t m_fourierTransformScalingFactor;
        bool m_fourierTransformScalingFactorSet = false;

        Ptr<Dect2020ExtChannelManager> m_channelManager;

        bool m_dummy = false;
};

} // namespace ns3

#endif //  DECT2020EXT_MAC_SCHEDULER_H