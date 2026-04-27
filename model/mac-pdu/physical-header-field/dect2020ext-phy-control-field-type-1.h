// PHY Control Field: Type 1, Header Format: 000

#ifndef DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1
#define DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtPhyControlFieldType1 : public Header
{
	public:
		Dect2020ExtPhyControlFieldType1();
		virtual ~Dect2020ExtPhyControlFieldType1();

		// Overloaded Methods from the Header class

		static TypeId GetTypeId();
		TypeId GetInstanceTypeId() const override;
		void Print(std::ostream& os) const override;
		uint32_t GetSerializedSize() const override;
		void Serialize(Buffer::Iterator start) const override;
		uint32_t Deserialize(Buffer::Iterator start) override;

		// Setters and Getters

		uint8_t GetHeaderFormat() const;
		void SetPacketLengthType(uint8_t packetLengthType);
		uint8_t GetPacketLengthType() const;
		void SetPacketLength(uint8_t packetLength);
		uint8_t GetPacketLength() const;
		void SetShortNetworkID(uint8_t shortNetworkID);
		uint8_t GetShortNetworkID() const;
		void SetTransmitterIdentity(uint16_t transmitterIdentity);
		uint16_t GetTransmitterIdentity() const;
		void SetTransmitPower(uint8_t transmitPower);
		uint8_t GetTransmitPower() const;
		void SetDFMCS(uint8_t dFMCS);
		uint8_t GetDFMCS() const;

	private:
		uint8_t m_headerFormat; // 3 relevant bits are set to 000 for type 1
		uint8_t m_packetLengthType; // 1 relevant bit
		uint8_t m_packetLength; // 4 relevant bits
		uint8_t m_shortNetworkID; // 8 relevant bits
		uint16_t m_transmitterIdentity; // 16 relevant bits
		uint8_t m_transmitPower; // 4 relevant bits
		uint8_t m_DFMCS; // 3 relevant bits
};

} // namespace ns3

#endif // DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1