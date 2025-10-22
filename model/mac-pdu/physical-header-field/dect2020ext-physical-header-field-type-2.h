#ifndef DECT2020EXT_PHYSICAL_HEADER_FIELD
#define DECT2020EXT_PHYSICAL_HEADER_FIELD

#include "ns3/address.h"
#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3
{

// TODO: Implement Physical Layer Control Field Type 1

class Dect2020ExtPhysicalHeaderField : public Header
{
	public:
		Dect2020ExtPhysicalHeaderField();
		virtual ~Dect2020ExtPhysicalHeaderField();

		// Physical Header Field type
		enum Dect2020ExtPhysicalHeaderFieldType
		{
			TYPE1, // 40 bit physical layer control field
			TYPE2 // 80 bit physical layer control field
		};

		// Overloaded Methods from the Header class
		static TypeId GetTypeId(void);
		virtual TypeId GetInstanceTypeId(void) const;
		virtual void Print(std::ostream& os) const;
		virtual uint32_t GetSerializedSize(void) const;
		virtual void Serialize(Buffer::Iterator start) const;
		virtual uint32_t Deserialize(Buffer::Iterator start);

		// Setters and Getters
		void SetPacketLengthType(uint8_t packetLengthType);
		uint8_t GetPacketLengthType();
		void SetPacketLength(uint8_t packetLength);
		uint8_t GetPacketLength();
		void SetShortNetworkID(uint8_t shortNetworkID);
		uint8_t GetShortNetworkID();
		void SetTransmitterIdentity(uint16_t transmitterIdentity);
		uint16_t GetTransmitterIdentity();
		void SetTransmitPower(uint8_t transmitPower);
		uint8_t GetTransmitPower();
		void SetDFMCS(uint8_t dFMCS);
		uint8_t GetDFMCS();

		void SetPhysicalHeaderFieldType(Dect2020ExtPhysicalHeaderFieldType physicalHeaderFieldType);
		Dect2020ExtPhysicalHeaderFieldType GetPhysicalHeaderFieldType() const;

	private:
		// Physical Header Field type 1

		uint8_t m_headerFormat; // 3 relevant bits
		Dect2020ExtPhysicalHeaderFieldType m_type; // 1 relevant bit
		uint8_t m_packetLength; // 4 relevant bits
		uint8_t m_shortNetworkID; // 8 relevant bits
		uint16_t m_transmitterIdentity; // 16 relevant bits
		uint8_t m_transmitPower; // 4 relevant bits
		uint8_t m_DFMCS; // 3 relevant bits

		// Additional fields for Physical Header Field type 2
		uint16_t m_receiverIdentity; // 16 relevant bits
		uint8_t m_spatialStreams; // 2 relevant bits
		uint8_t m_DFRedundancyVersion; // 2 relevant bits
		uint8_t m_DFNewDataIndication; // 1 relevant bit
		uint8_t m_DFHarqProcessNr; // 3 relevant bits
		uint8_t m_feedbackFormat; // 4 relevant bits
		uint32_t m_feedbackInfo; // 12 relevant bits

};

} // namespace ns3

#endif // DECT2020EXT_PHYSICAL_HEADER_FIELD