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

		uint8_t m_randomAccessResourceLength = 10;

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
