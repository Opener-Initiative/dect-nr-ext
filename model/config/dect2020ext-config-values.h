#ifndef DECT2020EXT_CONFIG_VALUES_H
#define DECT2020EXT_CONFIG_VALUES_H

#include "ns3/single-model-spectrum-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"

#include <cstdint>

namespace ns3
{

// Numerologies:
// Dect2020ExtChannelManager
// Dect2020ExtPhy

constexpr uint8_t BAND_NUMBER = 1;
constexpr uint8_t SUBCARRIER_SCALING_FACTOR = 1;
constexpr uint8_t FOURIER_TRANSFORM_SCALING_FACTOR = 1;

// Spectrum channel

using SPECTRUM_CHANNEL_TYPE = SingleModelSpectrumChannel;
using PROPAGATION_DELAY_MODEL = ConstantSpeedPropagationDelayModel;
using PROPAGATION_LOSS_MODEL = LogDistancePropagationLossModel;

// Antenna

constexpr uint8_t RX_GAIN_INDEX = 5;

} // namespace ns3

#endif