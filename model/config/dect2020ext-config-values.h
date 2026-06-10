#ifndef DECT2020EXT_CONFIG_VALUES_H
#define DECT2020EXT_CONFIG_VALUES_H

#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"

#include <cstdint>

namespace ns3
{

// ###### Values applicable to models ######

constexpr uint8_t BAND_NUMBER = 1;
constexpr uint8_t SUBCARRIER_SCALING_FACTOR = 1;
constexpr uint8_t FOURIER_TRANSFORM_SCALING_FACTOR = 1;

// ###### Constants that can be used on demand ######

constexpr uint32_t SLOTS_PER_FRAME = 24;
constexpr double SLOT_DURATION_NS = 416666; // 0,41667 milliseconds in nanoseconds
constexpr uint32_t FRAME_DURATION_NS = 10000000; // 10 milliseconds in nanoseconds
constexpr uint32_t FRAME_DURATION_US = 10000; // 10 milliseconds in microseconds
constexpr uint32_t FRAME_DURATION_MS = 10;

// ###### Spectrum channel ######

using SPECTRUM_CHANNEL_TYPE = SingleModelSpectrumChannel;
using PROPAGATION_DELAY_MODEL = ConstantSpeedPropagationDelayModel;
using PROPAGATION_LOSS_MODEL = LogDistancePropagationLossModel;

// ###### Antenna ######

constexpr uint8_t RX_GAIN_INDEX = 5;

// ###### MAC Random Access Procedure ######

constexpr bool USE_CPP_RNG = false; // Use random values every simulation from C++

} // namespace ns3

#endif
