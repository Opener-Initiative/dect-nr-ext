#ifndef DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H
#define DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H

#include "ns3/header.h"

namespace ns3
{

class Dect2020ExtRandomAccessResourceIe : public Header
{
    public:
        Dect2020ExtRandomAccessResourceIe();
        virtual ~Dect2020ExtRandomAccessResourceIe();

        // Virtual Header base class methods

        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        void Print(std::ostream &os) const override;

        // Setters and Getters

        void SetRepeat(uint8_t repeat);
        uint8_t GetRepeat() const;

        void SetSystemFrameNumberFieldIncluded(bool included);
        bool GetSystemFrameNumberFieldIncluded() const;

        void SetChannelFieldIncluded(bool included);
        bool GetChannelFieldIncluded() const;

        void SetSeparateChannelFieldIncluded(bool included);
        bool GetSeparateChannelFieldIncluded() const;

        void SetStartSubslot(uint16_t subslot);
        uint16_t GetStartSubslot() const;

        void SetLengthType(bool lengthType);
        bool GetLengthType() const;

        void SetRaraLength(uint8_t length);
        uint8_t GetRaraLength() const;

        void SetMaxRachLengthType(bool type);
        bool GetMaxRachLengthType() const;

        void SetMaxRachLength(uint8_t length);
        uint8_t GetMaxRachLength() const;

        void SetCwMinSig(uint8_t cwMinSig);
        uint8_t GetCwMinSig() const;

        void SetDectDelay(bool dectDelay);
        bool GetDectDelay() const;

        void SetResponseWindow(uint8_t responseWindow);
        uint8_t GetResponseWindow() const;

        void SetCwMaxSig(uint8_t cwMaxSig);
        uint8_t GetCwMaxSig() const;

        void SetRepetition(uint8_t repetition);
        uint8_t GetRepetition() const;

        void SetValidity(uint8_t validity);
        uint8_t GetValidity() const;

        void SetSystemFrameNumberValue(uint8_t sfn);
        uint8_t GetSystemFrameNumberValue() const;

        void SetChannelAbsoluteCarrierCenterFrequency(uint16_t freq);
        uint16_t GetChannelAbsoluteCarrierCenterFrequency() const;

        void SetSeparateChannelAbsoluteCarrierCenterFrequency(uint16_t freq);
        uint16_t GetSeparateChannelAbsoluteCarrierCenterFrequency() const;

    private:
        uint8_t m_repeat;                      // 2 bits
        bool m_systemFrameNumberFieldIncluded; // 1 bit
        bool m_channelFieldIncluded;           // 1 bit
        bool m_separateChannelFieldIncluded;   // 1 bit
        uint16_t m_startSubslot;               // 8 bits. For subcarrier scaling factor <= 4,
                                               // otherwise 9 bits. Note: Only the 8 Bit variant is currently implemented
        bool m_lengthType;    // 1 bit. 0 = length in subslots, 1 = length in slots
        uint8_t m_raraLength; // 7 bits. The length of the random access resource allocation in subslots or slots
        bool m_maxRachLengthType;         // 1 bit. 0 = length in subslots, 1 = length in slots
        uint8_t m_maxRachLength;          // 4 bits
        uint8_t m_cwMin_sig;              // 3 bits
        bool m_dectDelay;                 // 1 bit
        uint8_t m_responseWindow;         // 4 bits
        uint8_t m_cwMax_sig;              // 3 bits
        uint8_t m_repitition;             // 8 bits
        uint8_t m_validity;               // 8 bits
        uint8_t m_systemFrameNumberValue; // 8 bits
        uint16_t m_channelAbsoluteCarrierCentryFrequency;         // 13 bits
        uint16_t m_separateChannelAbsoluteCarrierCentryFrequency; // 13 bits
};

} // namespace ns3

#endif // DECT2020EXT_RANDOM_ACCESS_RESOURCE_IE_H 