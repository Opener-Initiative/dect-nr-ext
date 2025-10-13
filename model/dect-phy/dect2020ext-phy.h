#ifndef DECT2020EXT_PHY_H
#define DECT2020EXT_PHY_H

#include "ns3/dect2020ext-channel.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/traced-value.h"
#include "ns3/callback.h"

#include <vector>

namespace ns3
{

class Dect2020ExtChannel;
struct Slot;
struct Subslot;
class Dect2020ExtMac;

class Dect2020ExtPhy : public Object
{
	public:
		Dect2020ExtPhy();
		virtual ~Dect2020ExtPhy();

		static TypeId GetTypeId();

		void StartFrameTimer();
		void ProcessSlot(uint32_t slot, double slotStartTime);
		void ProcessSubslot(uint32_t slotId, uint32_t subslotId, uint32_t absoluteSubslotId);
		Slot* GetCurrentSlot();
		Subslot* GetCurrentSubslot();
		void OnNewFrame(Callback<void, uint32_t, uint32_t> callback);
		void DisconnectSFNCallback();

		void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
		uint8_t GetSubcarrierScalingFactor() const;
		Time GetNextSubslotTime();

		void SetMac(Ptr<Dect2020ExtMac> mac);
	
		Callback<void, uint32_t, uint32_t> m_SFNCallback;

	private:
	    Time m_frameStartTime;
		uint8_t m_subcarrierScalingFactor;
		std::vector<int> m_frame;
		uint32_t m_currentSubslotId;
		uint32_t m_totalSubslotsPerFrame;
		Ptr<Dect2020ExtMac> m_mac;
		TracedValue<uint32_t> m_SFN; // System Frame Number traced value

		// ###################
		// #### Constants ####
		// ###################

		const double SLOT_DURATION_NS = 416666; // 0,41667 ms in nanoseconds
		const uint16_t FRAME_DURATION_US = 10000; // 10 ms in microseconds
		const uint32_t SLOTS_PER_FRAME = 24;
};

} // namespace ns3

#endif /* DECT2020EXT_PHY_H */