#ifndef DECT2020EXT_BEACON_HEADER_H
#define DECT2020EXT_BEACON_HEADER_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtBeaconHeader : public Header
{
	public:
		Dect2020ExtBeaconHeader();
		virtual ~Dect2020ExtBeaconHeader();

		// Overloaded methods from Header base class

		static TypeId GetTypeId();
		TypeId GetInstanceTypeId() const override;
		void Print(std::ostream& os) const override;
		uint32_t GetSerializedSize() const override;
		void Serialize(Buffer::Iterator start) const override;
		uint32_t Deserialize(Buffer::Iterator start) override;

		// Setters and Getters

		void SetNetworkId(uint32_t networkId);
		uint32_t GetNetworkId() const;

		void SetTransmitterAddress(uint32_t transmitterId);
		uint32_t GetTransmitterAddress() const;

	private:
		uint32_t m_networkId; // 24 bits relevant
		uint32_t m_transmitterAddress; // 32 bits
};

} // namespace ns3

#endif // DECT2020EXT_BEACON_HEADER_H