#ifndef DECT2020EXT_PHY_H
#define DECT2020EXT_PHY_H

#include "ns3/dect2020ext-channel.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"

#include "ns3/spectrum-phy.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/traced-value.h"
#include "ns3/callback.h"

#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtChannel;
struct Slot;
struct Subslot;
class Dect2020ExtMac;
class Dect2020ExtPHYControlFieldType1;

class Dect2020ExtPhy : public SpectrumPhy
{
	public:
		Dect2020ExtPhy();
		virtual ~Dect2020ExtPhy();

		static TypeId GetTypeId();

		// Frame Timer and SFN

		void StartFrameTimer();
		void ProcessSlot(uint32_t slot, double slotStartTime);
		void ProcessSubslot(uint32_t slotId, uint32_t subslotId, uint32_t absoluteSubslotId);
		Slot* GetCurrentSlot();
		Subslot* GetCurrentSubslot();
		void OnNewFrame(Callback<void, uint32_t, uint32_t> callback);
		void DisconnectSFNCallback();

		// Dect2020ExtPhy Setters and Getters

		void SetMac(Ptr<Dect2020ExtMac> mac);
		uint32_t GetCurrentSFN() const;
		void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
		Ptr<Dect2020ExtChannelManager> GetChannelManager() const;
		uint16_t GetMcsTransportBlockSize(uint8_t mu, uint8_t beta, uint8_t mcsIndex);

		// Configuration Setters and Getters
		void SetBandNumber(uint8_t bandNumber);
		uint8_t GetBandNumber(void) const;
		void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
		uint8_t GetSubcarrierScalingFactor(void) const;
		void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
		uint8_t GetFourierTransformScalingFactor(void) const;

		// Inherited from SpectrumPhy

		void SetDevice(Ptr<NetDevice> d) override;
		Ptr<NetDevice> GetDevice() const override;
		void SetMobility(Ptr<MobilityModel> m) override;
		Ptr<MobilityModel> GetMobility() const override;
		void SetChannel(Ptr<SpectrumChannel> c) override;
		Ptr<SpectrumChannel> GetChannel() const; // Not inherited from SpectrumPhy
		Ptr<const SpectrumModel> GetRxSpectrumModel() const override;
		void SetAntenna(Ptr<Object> antenna); // Not inherited from SpectrumPhy
		Ptr<Object> GetAntenna() const override;
		void StartRx(Ptr<SpectrumSignalParameters> params) override;

		// Dect2020ExtPhy Methods

		void Send(Ptr<Packet> packet, Dect2020ExtPHYControlFieldType1 physicalHeader);
		double CalculateTxDurationNs(Dect2020ExtPHYControlFieldType1 physicalHeader);

		// Callbacks

		Callback<void, uint32_t, uint32_t> m_SFNCallback;

	private:
	    // Dect2020ExtPhy Private Members

		Ptr<Dect2020ExtChannelManager> m_channelManager;
	    Time m_frameStartTime;
		uint8_t m_subcarrierScalingFactor;
		uint8_t m_bandNumber;
		uint8_t m_fourierTransformScalingFactor;
		std::vector<int> m_frame;
		uint32_t m_currentSubslotId;
		uint32_t m_totalSubslotsPerFrame;
		Ptr<Dect2020ExtMac> m_mac;
		TracedValue<uint32_t> m_SFN; // System Frame Number traced value

		static const std::vector<uint16_t> m_singleSlotSingleStreamTransportBlockSizesMu1Beta1;

		Ptr<NetDevice> m_device;
		Ptr<MobilityModel> m_mobilityModel;
		Ptr<SpectrumChannel> m_channel;
		Ptr<const SpectrumModel> m_spectrumModel; // TODO: Determine if this is RxSpectrumModel
		Ptr<Object> m_antenna;

		// ###################
		// #### Constants ####
		// ###################

		const double SLOT_DURATION_NS = 416666; // 0,41667 ms in nanoseconds
		const uint16_t FRAME_DURATION_US = 10000; // 10 ms in microseconds
		const uint32_t SLOTS_PER_FRAME = 24;
};

} // namespace ns3

#endif /* DECT2020EXT_PHY_H */