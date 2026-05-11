#ifndef DECT2020EXT_RD_CAPABILITY_IE_H
#define DECT2020EXT_RD_CAPABILITY_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtRdCapabilityIe : public Header
{
    public:
        Dect2020ExtRdCapabilityIe();
        virtual ~Dect2020ExtRdCapabilityIe();

        // Virtual Header base class methods

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        void Print(std::ostream& os) const override;

        // Setters and Getters

        void SetNumOfPhyCapabilities(uint8_t val);
        uint8_t GetNumOfPhyCapabilities() const;

        void SetRelease(uint8_t val);
        uint8_t GetRelease() const;

        void SetGroupAssignment(bool enabled);
        bool GetGroupAssignment() const;

        void SetPaging(bool enabled);
        bool GetPaging() const;

        void SetOperatingModes(uint8_t val);
        uint8_t GetOperatingModes() const;

        void SetMesh(bool enabled);
        bool GetMesh() const;

        void SetScheduledAccessDataTransfer(bool enabled);
        bool GetScheduledAccessDataTransfer() const;

        void SetMacSecurity(uint8_t val);
        uint8_t GetMacSecurity() const;

        void SetDlcServiceType(uint8_t val);
        uint8_t GetDlcServiceType() const;

        void SetRdPowerClass(uint8_t val);
        uint8_t GetRdPowerClass() const;

        void SetMaxNssFoRx(uint8_t val);
        uint8_t GetMaxNssFoRx() const;

        void SetRxForTxDiversity(uint8_t val);
        uint8_t GetRxForTxDiversity() const;

        void SetRxGain(uint8_t val);
        uint8_t GetRxGain() const;

        void SetMaxMcs(uint8_t val);
        uint8_t GetMaxMcs() const;

        void SetSoftBufferSize(uint8_t val);
        uint8_t GetSoftBufferSize() const;

        void SetNumOfHarqProcesses(uint8_t val);
        uint8_t GetNumOfHarqProcesses() const;

        void SetHarqFeedbackDelay(uint8_t val);
        uint8_t GetHarqFeedbackDelay() const;

        void SetDDelay(bool enabled);
        bool GetDDelay() const;

        void SetHalfDulp(bool enabled);
        bool GetHalfDulp() const;

    private:
        uint8_t m_numOfPhyCapabilities; // 3 bits
        uint8_t m_Release; // 5 bits
        bool m_groupAssignment; // 1 bit
        bool m_paging; // 1 bit
        uint8_t m_operatingModes; // 2 bits
        bool m_mesh; // 1 bit 
        bool m_scheduledAccessDataTransfer; // 1 bit
        uint8_t m_macSecurity; // 3 bits
        uint8_t m_dlcServiceType; // 3 bits
        uint8_t m_rdPowerClass; // 3 bits
        uint8_t m_maxNssFoRx; // 3 bits
        uint8_t m_rxForTxDiversity; // 3 bits
        uint8_t m_rxGain; // 4 bits
        uint8_t m_maxMcs; // 4 bits
        uint8_t m_softBufferSize; // 4 bits
        uint8_t m_numOfHarqProcesses; // 2 bits
        uint8_t m_harqFeedbackDelay; // 4 bits
        bool m_dDelay; // 1 bit
        bool m_halfDulp; // 1 bit
        // The following 5 Oktets are not used (and not implemented)in the current implementation
};

} // namespace ns3

#endif // DECT2020EXT_RD_CAPABILITY_IE_H 