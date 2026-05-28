#ifndef DECT2020EXT_TYPES_H
#define DECT2020EXT_TYPES_H

#include "ns3/assert.h"

#include <stdint.h>

namespace ns3
{

template <typename T, typename Tag, T MinValue, T MaxValue>
class StrongType
{
    public:
        constexpr explicit StrongType(T value) : m_value(value)
        {
            NS_ASSERT_MSG(value >= MinValue && value <= MaxValue,
                          "Type " << typeid(Tag).name() << " value out of range ["
                          << MinValue << ", " << MaxValue << "]: " << value);
        }
        constexpr explicit operator T() const { return m_value; }

    private:
        T m_value;
};

// Tags
struct BandNumberTag{};
struct SubcarrierScalingFactorTag{};
struct FourierTransformScalingFactorTag{};
struct Band1ChannelNumberTag{};

// Data type that represents the DECT-2020 NR band number. Currently, only band 1 is supported.
using Dect2020ExtBandNumber = StrongType<uint8_t, BandNumberTag, 1, 1>;

// Data type that represents the DECT-2020 NR subcarrier scaling factor. Currently, only values between 1 and 8 are supported.
using Dect2020ExtSubcarrierScalingFactor = StrongType<uint8_t, SubcarrierScalingFactorTag, 1, 8>;

// Data type that represents the DECT-2020 NR Fourier transform scaling factor. Currently, only values between 1 and 8 are supported.
using Dect2020ExtFourierTransformScalingFactor = StrongType<uint8_t, FourierTransformScalingFactorTag, 1, 8>;

// Data type that represents the DECT-2020 NR band 1 channel ID. Currently, only values between 1657 and 1677 are supported.
using Dect2020ExtBand1ChannelNumber = StrongType<uint16_t, Band1ChannelNumberTag, 1657, 1677>;

} // namespace ns3

#endif // DECT2020EXT_TYPES_H