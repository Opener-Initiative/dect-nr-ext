#ifndef DECT2020EXT_ASSOCIATION_CONTROL_IE_H
#define DECT2020EXT_ASSOCIATION_CONTROL_IE_H

#include "ns3/header.h"

namespace ns3
{


class Dect2020ExtAssociationControlIE : public Header
{
    public:
        Dect2020ExtAssociationControlIE();
        virtual ~Dect2020ExtAssociationControlIE();

		// Overloaded Methods from the Header class

		static TypeId GetTypeId();
		TypeId GetInstanceTypeId() const override;
		void Print(std::ostream& os) const override;
		uint32_t GetSerializedSize() const override;
		void Serialize(Buffer::Iterator start) const override;
		uint32_t Deserialize(Buffer::Iterator start) override;

        // Setters and Getters

        void SetCbM(uint8_t cbM);
        uint8_t GetCbM() const;

        void SetDlDataReception(uint8_t dlDataReception);
        uint8_t GetDlDataReception() const;

        void SetUlPeriod(uint8_t ulPeriod);
        uint8_t GetUlPeriod() const;

    private:
        uint8_t m_CB_M; // 1 relevant bits
        uint8_t m_dlDataReception; // 3 relevant bits
        uint8_t m_ULPeriod; // 4 relevant bits
};

} // namespace ns3

#endif