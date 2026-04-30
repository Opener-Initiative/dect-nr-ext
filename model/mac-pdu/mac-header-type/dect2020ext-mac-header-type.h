#ifndef DECT2020EXT_MAC_HEADER_TYPE_H
#define DECT2020EXT_MAC_HEADER_TYPE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtMacHeaderType : public Header
{
	public:
		enum MacSecurityField
		{
			MAC_SECURITY_NOT_USED = 0,     // MAC security is not used for this MAC PDU
			MAC_SECURITY_USED_NO_IE = 1,   // MAC security is used and the MAC Security IE is not present
			MAC_SECURITY_USED_WITH_IE = 2, // MAC is security used and a MAC Secucity Info IE is in MAC PDU
			RESERVED = 3                   // Reserved
		};

		enum MacHeaderTypeField
		{
			DATA_MAC_PDU_HEADER = 0,    // DATA MAC PDU header
			BEACON_HEADER = 1,          // Beacon Header
			UNICAST_HEADER = 2,         // Unicast Header
			RD_BROADCASTING_HEADER = 3, // RD Broadcasting Header
			ESCAPE = 15                 // Escape
		};

		Dect2020ExtMacHeaderType();
		virtual ~Dect2020ExtMacHeaderType();

		// Overloaded methods from Header base class

		static TypeId GetTypeId();
		TypeId GetInstanceTypeId() const override;
		void Print(std::ostream& os) const override;
		uint32_t GetSerializedSize() const override;
		void Serialize(Buffer::Iterator start) const override;
		uint32_t Deserialize(Buffer::Iterator start) override;

		// Setters and Getters

		void SetVersion(uint8_t version);
		uint8_t GetVersion() const;

		void SetMacSecurity(MacSecurityField macSecurity);
		MacSecurityField GetMacSecurity() const;

		void SetMacHeaderTypeField(MacHeaderTypeField macHeaderTypeField);
		MacHeaderTypeField GetMacHeaderTypeField() const;

	private:
		uint8_t m_version;
		MacSecurityField m_macSecurity;
		MacHeaderTypeField m_macHeaderTypeField;
};

} // namespace ns3

#endif // DECT2020EXT_MAC_HEADER_TYPE_H
