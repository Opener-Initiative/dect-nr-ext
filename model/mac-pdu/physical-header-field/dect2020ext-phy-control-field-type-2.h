#ifndef DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2
#define DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtPHYControlFieldType2 : public Header
{
	public:
		// PHY Control Field Type 2 format
		enum Dect2020ExtHeaderFormat : uint8_t
		{
			FORMAT_000 = 0,
			FORMAT_001 = 1
		};

		Dect2020ExtPHYControlFieldType2();
		Dect2020ExtPHYControlFieldType2(Dect2020ExtHeaderFormat headerFormat);
		virtual ~Dect2020ExtPHYControlFieldType2();

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
		void SetReceiverIdentity(uint16_t receiverIdentity);
		uint16_t GetReceiverIdentity() const;
		void SetNumberOfSpatialStreams(uint8_t numberOfSpatialStreams);
		uint8_t GetNumberOfSpatialStreams() const;
		void SetDFRedundancyVersion(uint8_t dFRedundancyVersion);
		uint8_t GetDFRedundancyVersion() const;
		void SetDFNewDataIndication(uint8_t dFNewDataIndication);
		uint8_t GetDFNewDataIndication() const;
		void SetDFHARQProcessNumber(uint8_t dFHARQProcessNumber);
		uint8_t GetDFHARQProcessNumber() const;
		void SetFeedbackFormat(uint8_t feedbackFormat);
		uint8_t GetFeedbackFormat() const;
		void SetFeedbackInfo(uint16_t feedbackInfo);
		uint16_t GetFeedbackInfo() const;

	private:
		uint8_t m_headerFormat; // 3 relevant bits
		uint8_t m_packetLengthType; // 1 relevant bit
		uint8_t m_packetLength; // 4 relevant bits
		uint8_t m_shortNetworkID; // 8 relevant bits
		uint16_t m_transmitterIdentity; // 16 relevant bits
		uint8_t m_transmitPower; // 4 relevant bits
		uint8_t m_DFMCS; // 4 relevant bits
		uint16_t m_receiverIdentity; // 16 relevant bits
		uint8_t m_numberOfSpatialStreams; // 2 relevant bits
		uint8_t m_DFRedundancyVersion; // 2 relevant bits
		uint8_t m_DFNewDataIndication; // 1 relevant bit
		uint8_t m_DFHARQProcessNumber; // 3 relevant bits
		uint8_t m_feedbackFormat; // 4 relevant bits
		uint16_t m_feedbackInfo; // 12 relevant bits

};

} // namespace ns3

#endif // DECT2020EXT_PHY_CONTROL_FIELD_TYPE_2