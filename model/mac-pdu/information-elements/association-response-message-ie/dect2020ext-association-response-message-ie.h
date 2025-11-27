#ifndef DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H
#define DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H

#include "ns3/dect2020ext-network-beacon-message-ie.h"

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtAssociationResponseMessageIE : public Header
{
    public:
        Dect2020ExtAssociationResponseMessageIE();
        virtual ~Dect2020ExtAssociationResponseMessageIE();

        // Virtual Header base class methods

        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId() const override;
        virtual uint32_t GetSerializedSize() const override;
        virtual void Serialize(Buffer::Iterator start) const override;
        virtual uint32_t Deserialize(Buffer::Iterator start) override;
        virtual void Print(std::ostream &os) const override;

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
		bool m_associationAccepted;           // 1 Bit
		bool m_harqMod;                       // 1 Bit
		uint8_t m_numberOfFlows;              // 3 Bit
		bool m_groupIdAndResourceTagIncluded; // 1 Bit
		uint8_t m_rejectCause;                // 4 Bit
		uint8_t m_rejectTimer;                // 4 Bit
		uint8_t m_harqProcessesRx;            // 3 Bit
		uint8_t m_maxHarqReRxDelay;           // 5 Bot
		uint8_t m_harqProcessesTx;            // 3 Bit
		uint8_t m_maxHarqReTxDelay;           // 5 Bit
		uint8_t m_flowId;                     // 6 Bit
		uint8_t m_groupId;                    // 7 Bit
		uint8_t m_resourceTag;                // 7 Bit
};

} // namespace ns3

#endif // DECT2020EXT_ASSOCIATION_RESPONSE_MESSAGE_IE_H 