#ifndef DECT2020EXT_ASSOCIATION_CONTROL_IE_H
#define DECT2020EXT_ASSOCIATION_CONTROL_IE_H

#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3
{


class Dect2020ExtAssociationControlIE : public Header
{
    public:
        Dect2020ExtAssociationControlIE();
        virtual ~Dect2020ExtAssociationControlIE();

        // DlDataReception encoding
        enum DlDataReception : uint8_t
        {
            MS_0 = 0,
            MS_5 = 1,
            MS_10 = 2,
            MS_20 = 3,
            MS_40 = 4,
            MS_80 = 5
        };

        // ULPeriod encoding
        enum ULPeriod : uint8_t
        {
            S_10 = 0,
            S_20 = 1,
            S_48 = 2,
            S_90 = 3,
            MIN_5 = 4,
            MIN_10 = 5,
            MIN_30 = 6,
            H_1 = 7,
            H_6 = 8,
            H_12 = 9,
            H_24 = 10,
            H_48 = 11
        };

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

        void SetDlDataReception(DlDataReception dlDataReception);
        uint8_t GetDlDataReception() const;

        void SetUlPeriod(ULPeriod ulPeriod);
        uint8_t GetUlPeriod() const;

        // Association Control IE methods
        Time DecodeDlDataReception() const;
        Time DecodeULPeriod() const;

    private:
        uint8_t m_CB_M; // 1 relevant bits
        uint8_t m_dlDataReception; // 3 relevant bits
        uint8_t m_ULPeriod; // 4 relevant bits
};

} // namespace ns3

#endif