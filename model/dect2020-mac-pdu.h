#ifndef DECT2020_MAC_PDU_H
#define DECT2020_MAC_PDU_H

namespace ns3
{

class Dect2020MacPdu
{
    public:
        enum MacHeaderType
        {
            DATA_MAC_PDU_HEADER,
            BEACON_HEADER,
            UNICAST_HEADER,
            RD_BROADCASTING_HEADER,
            ESCAPE
        };

        enum MacSecurity
        {
            MAC_SECURITY_NOT_USED,
            MAC_SECURITY_USED_NO_IE,
            MAC_SECURITY_USED_WITH_IE,
            RESERVED
        };

        Dect2020MacPdu(MacHeaderType type);
        ~Dect2020MacPdu();

        // Setters and Getters

        MacHeaderType GetMacHeaderType() const;
        void SetMacHeaderType(MacHeaderType type);
        
        MacSecurity GetMacSecurity() const;
        void SetMacSecurity(MacSecurity security);

    private:
        MacHeaderType m_macHeaderType;
        MacSecurity m_macSecurity;
};

}

#endif