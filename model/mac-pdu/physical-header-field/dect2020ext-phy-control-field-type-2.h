// PHY Control Field: Type 2, Header Format: 000
// PHY Control Field: Type 2, Header Format: 001

#ifndef DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2_H
#define DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtPhyControlFieldType2 : public Header
{
	public:
		// PHY Control Field Type 2 format
		enum PhyControlFieldType2Format
		{
			FORMAT_000 = 0,
			FORMAT_001 = 1
		};

		Dect2020ExtPhyControlFieldType2();
		Dect2020ExtPhyControlFieldType2(PhyControlFieldType2Format headerFormat);
		virtual ~Dect2020ExtPhyControlFieldType2();

		// Overloaded Methods from the Header class

		static TypeId GetTypeId();
		TypeId GetInstanceTypeId() const override;
		void Print(std::ostream& os) const override;
		uint32_t GetSerializedSize() const override;
		void Serialize(Buffer::Iterator start) const override;
		uint32_t Deserialize(Buffer::Iterator start) override;

		// Setters and Getters

		void SetHeaderFormat(PhyControlFieldType2Format format);
		PhyControlFieldType2Format GetHeaderFormat() const;

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

		void SetDfmcs(uint8_t dFMCS);
		uint8_t GetDfmcs() const;

		void SetReceiverIdentity(uint16_t receiverIdentity);
		uint16_t GetReceiverIdentity() const;

		void SetNumberOfSpatialStreams(uint8_t numberOfSpatialStreams);
		uint8_t GetNumberOfSpatialStreams() const;

		void SetDfRedundancyVersion(uint8_t dfRedundancyVersion);
		uint8_t GetDfRedundancyVersion() const;

		void SetDfNewDataIndication(bool dFNewDataIndication);
		bool GetDfNewDataIndication() const;

		void SetDfHarqProcessNumber(uint8_t dfHarqProcessNumber);
		uint8_t GetDfHarqProcessNumber() const;

		void SetFeedbackFormat(uint8_t feedbackFormat);
		uint8_t GetFeedbackFormat() const;

		void SetFeedbackInfo(uint16_t feedbackInfo);
		uint16_t GetFeedbackInfo() const;

	private:
		PhyControlFieldType2Format m_headerFormat; // 3 relevant bits
		bool m_packetLengthType = false;      // 2 relevant bit
		uint8_t m_packetLength = 0;           // 4 relevant bits
		uint8_t m_shortNetworkId = 0;         // 8 relevant bits
		uint16_t m_transmitterIdentity = 0;   // 16 relevant bits
		uint8_t m_transmitPower = 0;          // 4 relevant bits
		uint8_t m_dfmcs = 0;    	  	      // 4 relevant bits
		uint16_t m_receiverIdentity = 0;      // 16 relevant bits
		uint8_t m_numberOfSpatialStreams = 0; // 2 relevant bits
		uint8_t m_dfRedundancyVersion = 0;    // 2 relevant bits
		bool m_dfNewDataIndication = false;   // 1 relevant bit
		uint8_t m_dfHarqProcessNumber = 0;    // 3 relevant bits
		uint8_t m_feedbackFormat = 0;         // 4 relevant bits
		uint16_t m_feedbackInfo = 0;          // 12 relevant bits
};

} // namespace ns3

#endif // DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2_H