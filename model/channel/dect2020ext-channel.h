#ifndef DECT2020EXT_CHANNEL_H
#define DECT2020EXT_CHANNEL_H

#include "ns3/object.h"

#include <vector>

namespace ns3
{

enum SubslotStatus
{
    FREE = 1,
    POSSIBLE = 2,
    BUSY = 3
};

struct Subslot
{
    uint32_t subslotId; // [0 - 15]
    SubslotStatus status; // [FREE, POSSIBLE, BUSY]
    double rssi; // Received Signal Strength Indicator in dBm
    double subslotDurationNs; // Duration of a Subslot in ns
};

struct Slot
{
    uint32_t slotId; // [0 - 23]
    std::vector<Subslot> subslots;
};

class Dect2020ExtChannel : public Object
{
  public:
    Dect2020ExtChannel();
    virtual ~Dect2020ExtChannel();

    static TypeId GetTypeId();

    void AddSlot(Slot slot);

    uint32_t m_channelNumber; // #ETSI 103 636-2 V1.5.1, Section 5.4.2 (n)
    std::vector<Slot> m_slots;
    std::vector<Slot> m_slotsLastFrame;
    double m_centerFrequency;
};

} // namespace ns3

#endif /* DECT2020EXT_CHANNEL_H */