#ifndef DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H
#define DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtAssociationResponseMessageIe : public Header
{
    public:
        Dect2020ExtAssociationResponseMessageIe();
        virtual ~Dect2020ExtAssociationResponseMessageIe();

        // Virtual Header base class methods

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        void Print(std::ostream &os) const override;

        // Setters and Getters

		void SetAssociationAccepted(bool accepted);
		bool GetAssociationAccepted() const;

		void SetHarqMod(bool harqMod);
		bool GetHarqMod() const;

		void SetNumberOfFlows(uint8_t numberOfFlows);
		uint8_t GetNumberOfFlows() const;

		void SetGroupIdAndResourceTagIncluded(bool included);
		bool GetGroupIdAndResourceTagIncluded() const;

		void SetRejectCause(uint8_t cause);
		uint8_t GetRejectCause() const;

		void SetRejectTimer(uint8_t timer);
		uint8_t GetRejectTimer() const;

		void SetHarqProcessesRx(uint8_t value);
		uint8_t GetHarqProcessesRx() const;

		void SetMaxHarqReRxDelay(uint8_t delay);
		uint8_t GetMaxHarqReRxDelay() const;

		void SetHarqProcessesTx(uint8_t value);
		uint8_t GetHarqProcessesTx() const;

		void SetMaxHarqReTxDelay(uint8_t delay);
		uint8_t GetMaxHarqReTxDelay() const;

		void SetFlowId(uint8_t flowId);
		uint8_t GetFlowId() const;

		void SetGroupId(uint8_t groupId);
		uint8_t GetGroupId() const;

		void SetResourceTag(uint8_t resourceTag);
		uint8_t GetResourceTag() const;

    private:
		bool m_associationAccepted;           // 1 bit
		bool m_harqMod;                       // 1 bit
		uint8_t m_numberOfFlows;              // 3 bits
		bool m_groupIdAndResourceTagIncluded; // 1 bit
		uint8_t m_rejectCause;                // 4 bits
		uint8_t m_rejectTimer;                // 4 bits
		uint8_t m_harqProcessesRx;            // 3 bits
		uint8_t m_maxHarqReRxDelay;           // 5 bits
		uint8_t m_harqProcessesTx;            // 3 bits
		uint8_t m_maxHarqReTxDelay;           // 5 bits
		uint8_t m_flowId;                     // 6 bits
		uint8_t m_groupId;                    // 7 bits
		uint8_t m_resourceTag;                // 7 bits
};

} // namespace ns3

#endif // DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H 