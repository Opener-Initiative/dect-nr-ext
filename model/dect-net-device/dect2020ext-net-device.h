#ifndef DECT2020EXT_NET_DEVICE_H
#define DECT2020EXT_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/traced-callback.h"

namespace ns3
{

class Dect2020ExtNetDevice : public NetDevice
{
	public:

		Dect2020ExtNetDevice();
		virtual ~Dect2020ExtNetDevice();

		static TypeId GetTypeId();

		enum TerminationPointType
		{
			PT, // Portable Termination
			FT // Fixed Termination
		};

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

	private:
		Ptr<Node> m_node;
		uint32_t m_ifIndex;
		uint16_t m_mtu;
		bool m_linkUp;
		Ptr<Channel> m_channel;
		TracedCallback<> m_linkChanges;
		ReceiveCallback m_receiveCallback;
		PromiscReceiveCallback m_promiscCallback;
		Address m_address;
};

} // namespace ns3

#endif /* DECT2020EXT_NET_DEVICE_H */
