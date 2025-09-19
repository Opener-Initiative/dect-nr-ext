#ifndef DECT2020EXT_PHY_H
#define DECT2020EXT_PHY_H

#include "ns3/dect2020ext-channel.h"

#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3
{

class Dect2020ExtChannel;
struct Slot;
struct Subslot;

class Dect2020ExtPhy : public Object
{
	public:
		Dect2020ExtPhy();
		virtual ~Dect2020ExtPhy();

		static TypeId GetTypeId();

		void StartFrameTimer();
		void ProcessSlot(uint32_t slot, double slotStartTime);
		void ProcessSubslot(uint32_t slotId, uint32_t subslotId);
		Slot* GetCurrentSlot(uint32_t channelId);
		Subslot* GetCurrentSubslot(uint32_t channelId);
	
	private:
	    Time m_frameStartTime;

		// ###################
		// #### Constants ####
		// ###################

		const double SLOT_DURATION_NS = 416666; // 0,41667 ms in nanoseconds
		const uint16_t FRAME_DURATION_US = 10000; // 10 ms in microseconds
};

} // namespace ns3

#endif /* DECT2020EXT_PHY_H */