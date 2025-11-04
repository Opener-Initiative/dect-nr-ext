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
            NS_ASSERT_MSG(value >= MinValue && value <= MaxValue, "Value out of range");
        }
        constexpr explicit operator T() const { return m_value; }

    private:
        T m_value;
};

// Tags
struct BandNumberTag{};
struct SubcarrierScalingFactorTag{};
struct FourierTransformScalingFactorTag{};

using Dect2020ExtBandNumber = StrongType<uint8_t, BandNumberTag, 1, 21>;
using Dect2020ExtSubcarrierScalingFactor = StrongType<uint8_t, SubcarrierScalingFactorTag, 1, 8>;
using Dect2020ExtFourierTransformScalingFactor = StrongType<uint8_t, FourierTransformScalingFactorTag, 1, 8>;

} // namespace ns3

#endif // DECT2020EXT_TYPES_H