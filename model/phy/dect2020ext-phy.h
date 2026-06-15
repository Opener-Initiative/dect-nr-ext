#ifndef DECT2020EXT_PHY_H
#define DECT2020EXT_PHY_H

#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-phy-control-field-type-2.h"
#include "ns3/dect2020ext-interference.h"

#include "ns3/spectrum-phy.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/traced-value.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtMac;
class Dect2020ExtPhyControlFieldType1;
class Dect2020ExtPhyControlFieldType2;
class Dect2020ExtInterference;

class Dect2020ExtPhy : public SpectrumPhy
{
	public:

	    enum PhyState
		{
			IDLE,
			RX,
			TX
		};

		Dect2020ExtPhy();
		virtual ~Dect2020ExtPhy();

		static TypeId GetTypeId();

		void Start();

		// ###### Dect2020ExtPhy Setters and Getters ######

		void SetMac(Ptr<Dect2020ExtMac> mac);
		void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);
		Ptr<Dect2020ExtChannelManager> GetChannelManager() const;
		uint16_t GetMcsTransportBlockSize(uint8_t mu, uint8_t beta, uint8_t mcsIndex);
		void SetReceiveCallback(Callback<void, Ptr<Packet>, double, bool> callback);
		PhyState GetState() const;

		// ###### Configuration ######

		void SetBandNumber(uint8_t bandNumber);
		uint8_t GetBandNumber() const;
		void SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor);
		uint8_t GetSubcarrierScalingFactor() const;
		void SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor);
		uint8_t GetFourierTransformScalingFactor() const;
		void PrintConfiguration() const;

		// SpectrumPhy Overloaded Methods

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

		template <typename PhyControlFieldT>
		void Send(Ptr<Packet> packet, PhyControlFieldT phyControlField, bool random);

		template <typename PhyControlFieldT>
		double CalculateTxDurationNs(PhyControlFieldT physicalHeader);

		void ChangeState(PhyState newState);
		void EndRx(Ptr<Packet> packet, double rssiDbm, bool random);
		bool IsAvailable();

		// ###### Interference ######

		Ptr<Dect2020ExtInterference> m_interference;

		std::string PHY_IDENTITY;

		// ###### Test ######

		bool m_phyTest = false;

	private:
		uint32_t m_currentSlot;
		uint32_t m_currentSubslot;
		uint16_t m_currentAbsoluteSubslot;
	    Time m_frameStartTime;
		std::vector<int> m_frame;
		uint32_t m_currentSubslotId;
		uint32_t m_totalSubslotsPerFrame;
		static const std::vector<uint16_t> m_singleSlotSingleStreamTransportBlockSizesMu1Beta1;
		Ptr<const SpectrumModel> m_spectrumModel;
		PhyState m_currentState;

		// ###### Configuration ######

		uint8_t m_bandNumber;
		bool m_bandNumberSet = false;

		uint8_t m_subcarrierScalingFactor;
		bool m_subcarrierScalingFactorSet = false;

		uint8_t m_fourierTransformScalingFactor;
		bool m_fourierTransformScalingFactorSet = false;

		Callback<void, Ptr<Packet>, double, bool> m_receiveCallback;
		bool m_receiveCallbackSet = false;

		// Configuration pointers

		Ptr<Dect2020ExtMac> m_mac;
		Ptr<Dect2020ExtChannelManager> m_channelManager;
		Ptr<SpectrumChannel> m_channel;
		Ptr<Object> m_antenna;
		Ptr<NetDevice> m_device;
		Ptr<MobilityModel> m_mobilityModel;
};

} // namespace ns3

#endif /* DECT2020EXT_PHY_H */