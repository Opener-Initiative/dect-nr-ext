// PHY Control Field: Type 1, Header Format: 000

#ifndef DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_H
#define DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_H

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

		void SetPacketLengthType(bool packetLengthType);
		bool GetPacketLengthType() const;

		void SetPacketLength(uint8_t packetLength);
		uint8_t GetPacketLength() const;

		void SetShortNetworkId(uint8_t shortNetworkId);
		uint8_t GetShortNetworkId() const;

		void SetTransmitterIdentity(uint16_t transmitterIdentity);
		uint16_t GetTransmitterIdentity() const;

		void SetTransmitPower(uint8_t transmitPower);
		uint8_t GetTransmitPower() const;

        void SetDFMCS(uint8_t DFMCS);
        uint8_t GetDFMCS() const;

	private:
		uint8_t m_headerFormat = 0; 	// 3 bits are set to 000 for type 1
		bool m_packetLengthType;        // 1 bit
		uint8_t m_packetLength;         // 4 bits
		uint8_t m_shortNetworkId;       // 8 bits
		uint16_t m_transmitterIdentity; // 16 bits
		uint8_t m_transmitPower;        // 4 bits
		uint8_t m_DFMCS;                // 3 bits
};

} // namespace ns3

#endif // DECT2020EXT_PHY_CONTROL_FIELD_TYPE_1_H
