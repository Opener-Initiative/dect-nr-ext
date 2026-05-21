#ifndef DECT2020EXT_NET_DEVICE_H
#define DECT2020EXT_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/traced-callback.h"
#include "ns3/channel.h"
#include "ns3/packet.h"

namespace ns3
{

// Forward declarations

class Dect2020ExtMac;
class Dect2020ExtPhy;

class Dect2020ExtNetDevice : public NetDevice
{
	public:

		Dect2020ExtNetDevice();
		virtual ~Dect2020ExtNetDevice();

		static TypeId GetTypeId();

		enum Dect2020ExtTerminationPointType
		{
			PT,// Portable Termination
			FT // Fixed Termination
		};

		// Dect2020ExtNetDevice methods

		void SetMac(Ptr<Dect2020ExtMac> mac);
		Ptr<Dect2020ExtMac> GetMac() const;
		void SetPhy(Ptr<Dect2020ExtPhy> phy);
		Ptr<Dect2020ExtPhy> GetPhy() const;
		void SetBandNumber(uint8_t bandNumber);
		uint8_t GetBandNumber() const;
		void SetTerminationPointType(Dect2020ExtTerminationPointType terminationPointType);
		Dect2020ExtTerminationPointType GetTerminationPointType() const;
		void SetBootstrappingVariables(bool isFT);

		// NetDevice virtual methods

		void SetIfIndex(const uint32_t index) override;
		uint32_t GetIfIndex() const override;
		Ptr<Channel> GetChannel() const override;
		void SetAddress(Address address) override;
		Address GetAddress() const override;
		bool SetMtu(const uint16_t mtu) override;
		uint16_t GetMtu() const override;
		bool IsLinkUp() const override;
		void AddLinkChangeCallback(Callback<void> callback) override;
		bool IsBroadcast() const override;
		Address GetBroadcast() const override;
		bool IsMulticast() const override;
		Address GetMulticast(Ipv4Address multicastGroup) const override;
		Address GetMulticast(Ipv6Address addr) const override;
		bool IsBridge() const override;
		bool IsPointToPoint() const override;
		bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;
		bool SendFrom(Ptr<Packet> packet,
					const Address& source,
					const Address& dest,
					uint16_t protocolNumber) override;
		Ptr<Node> GetNode() const override;
		void SetNode(Ptr<Node> node) override;
		bool NeedsArp() const override;
		void SetReceiveCallback(ReceiveCallback cb) override;
		void SetPromiscReceiveCallback(PromiscReceiveCallback cb) override;
		bool SupportsSendFrom() const override;

	void Receive(Ptr<Packet> packet);

	// --- Bootstrapping variables ---

	// Random Access Resoure IE Values --> ETSI TS 103636 4 V 2.1.1 Table 6.4.3.4
	uint8_t m_randomAccessResourceLength = 10;

	// RD Capability Values --> ETSI TS 103636 4 V 2.1.1 Table 6.4.3.5-1
	uint8_t m_numOfPHYCapabilities;
	uint8_t m_release; // Indicates release of the radio device. Coded values: reserved, Release 1,
	                   // Release 2, Release 3, Release 4, rest of the values are reserved
	bool m_supportGroupAssignment ; // Indicates if the device supports group assignment
	bool m_supportPaging;       // Indicates if the device supports paging
	uint8_t m_operatingModes;      // Indicates the operating modes of the device
	bool m_mesh;
	bool m_scheduledAccessDataTransfer; // Indicates if the device supports scheduled access data
	                                     // transfer
	uint8_t m_macSecurity; // Indicates the MAC security level of the device
	uint8_t m_dlcServiceType; // Indicates the DLC service type of the device
	uint8_t m_rdPowerClass; // Indicates the RD power class of the device
	uint8_t m_maxNssFoRx; // Indicates the maximum number of spatial streams for reception
	uint8_t m_rxForTxDiversity; // Indicates if the device supports RX for TX diversity
	uint8_t m_rxGain; // Indicates the RX gain of the device
	uint8_t m_maxMcs; // Indicates the maximum MCS of the device
	uint8_t m_softBufferSize; // Indicates the size of the soft buffer of the device
	uint8_t m_numOfHarqProcesses; // Indicates the number of HARQ processes of the device
	uint8_t m_harqFeedbackDelay; // Indicates the HARQ feedback delay of the device
	bool m_dDelay; // Indicates if the device supports D-Delay
	bool m_halfDulp; // Indicates if the device supports half-duplex operation

	double m_txPowerDbm = 10; // Default transmission power in dBm

	TracedCallback<Ptr<const Packet>> m_txTrace;
	TracedCallback<Ptr<const Packet>> m_rxTrace;

	private:
		Dect2020ExtTerminationPointType m_terminationPointType;

		Ptr<Node> m_node;
		uint32_t m_ifIndex;
		uint16_t m_mtu;
		bool m_linkUp;
		Ptr<Channel> m_channel;
		TracedCallback<> m_linkChanges;
		ReceiveCallback m_receiveCallback;
		PromiscReceiveCallback m_promiscCallback;
		Address m_address;
		Ptr<Dect2020ExtMac> m_mac;
		Ptr<Dect2020ExtPhy> m_phy;
		uint8_t m_bandNumber;
};

} // namespace ns3

#endif /* DECT2020EXT_NET_DEVICE_H */
