# DECT NR+ Configuration Manual

This document defines how configuration works in `dect-nr-ext` and what each MAC per-simulation option does at runtime.

## 1) Configuration layers

The implementation is configured in two layers:

1. Global configuration in `contrib/dect-nr-ext/model/config/`:
   - `dect2020ext-config-values.h`
   - `dect2020ext-config.h/.cc` (`Dect2020ExtConfig`)
2. Per-simulation configuration through `Dect2020ExtMac` API calls in simulation scripts.

Global settings establish shared radio/system behavior for all devices.  
Per-simulation settings control device role, random-access/association behavior, and staged protocol execution.

## 2) Global configuration (`model/config`)

## 2.1 `dect2020ext-config-values.h`

### Core model parameters

- `BAND_NUMBER`  
  Applied to both PHY and MAC through `Dect2020ExtConfig::ApplyToPhy` and `ApplyToMac`. Defines the DECT band context used by channel manager operations.

- `SUBCARRIER_SCALING_FACTOR`  
  Applied to PHY and MAC. Used by scheduler/time conversions and MAC PDU size calculations.

- `FOURIER_TRANSFORM_SCALING_FACTOR`  
  Applied to PHY and MAC. Used together with subcarrier scaling in transport-block size and timing calculations.

### Frame/slot timing constants

- `SLOTS_PER_FRAME`
- `SLOT_DURATION_NS`
- `FRAME_DURATION_NS`
- `FRAME_DURATION_US`
- `FRAME_DURATION_MS`

These are used across MAC scheduling, scan timing, and random-access timing calculations. They are protocol-level constants, not experiment knobs.

### Spectrum channel definitions

- `SPECTRUM_CHANNEL_TYPE`
- `PROPAGATION_DELAY_MODEL` (`ConstantSpeedPropagationDelayModel`)
- `PROPAGATION_LOSS_MODEL` (`LogDistancePropagationLossModel`)

`PROPAGATION_DELAY_MODEL` and `PROPAGATION_LOSS_MODEL` are marked as "DON'T CHANGE" in code and should be treated as fixed baseline definitions.

### Antenna default

- `RX_GAIN_INDEX`  
  Used by `Dect2020ExtConfig::CreateConfiguredIsotropicAntennaModel()`. Converted to antenna gain via `-10 + RX_GAIN_INDEX * 2` dB.

### RNG-related constant

- `USE_CPP_RNG`  
  Present in global config values. Not used by the MAC backoff configuration path documented here.

## 2.2 `Dect2020ExtConfig` runtime behavior

`Dect2020ExtConfig` constructor creates shared objects:

- `m_spectrumChannel`
- `m_channelManager`

`ApplyToPhy(phy)` sets:

- band number
- subcarrier scaling factor
- Fourier transform scaling factor
- shared spectrum channel
- shared channel manager
- per-PHY isotropic antenna instance

`ApplyToMac(mac)` sets:

- band number (`m_bandNumber`)
- subcarrier scaling factor
- Fourier transform scaling factor
- shared channel manager

`GetDefaultRandomAccessResourceIe()` and `GetDefaultAssociationControlIe()` provide default IEs for FT/PT scripts.

## 3) Per-simulation MAC configuration options

This section maps the fields in `Dect2020ExtMac` configuration block (`m_bandNumber` through `m_searchBestFtTimeMs`) to API calls and simulation effects.

## 3.1 Field-by-field runtime semantics

1. `m_bandNumber`
   - Set by: `SetBandNumber()` (normally called from global `ApplyToMac`).
   - Effect: defines band context for channel manager lookups and scan/channel operations.
   - Notes: treated as global input, not a typical per-script override.

2. `m_bandNumberSet`
   - Set by: `SetBandNumber()`.
   - Effect: initialization guard.
   - Failure behavior: `Initialize()` aborts if not set.

3. `m_subcarrierScalingFactor`
   - Set by: `SetSubcarrierScalingFactor()` (normally from global config).
   - Effect: affects packet length conversion and scheduler-related calculations.

4. `m_subcarrierScalingFactorSet`
   - Set by: `SetSubcarrierScalingFactor()`.
   - Effect: initialization guard.
   - Failure behavior: `Initialize()` aborts if not set.

5. `m_fourierTransformScalingFactor`
   - Set by: `SetFourierTransformScalingFactor()` (normally from global config).
   - Effect: used with subcarrier scaling for PHY/MAC size/timing dependent logic.

6. `m_fourierTransformScalingFactorSet`
   - Set by: `SetFourierTransformScalingFactor()`.
   - Effect: initialization guard.
   - Failure behavior: `Initialize()` aborts if not set.

7. `m_channelManager`
   - Set by: `SetChannelManager()` (normally from global config).
   - Effect: channel numbering/band parameter access, RSSI conversions, subslot count access.
   - Failure behavior: `Initialize()` aborts if not set.

8. `m_terminationPointType`
   - Set by: `SetTerminationPointType()`.
   - Effect: selects FT vs PT execution path in `Start()` and packet handlers.

9. `m_terminationPointTypeSet`
   - Set by: `SetTerminationPointType()`.
   - Effect: initialization guard.
   - Failure behavior: `Initialize()` aborts if not set.

10. `m_phy`
    - Set by: `SetPhy()`.
    - Effect: connects MAC to PHY send/receive path and interference sampling.
    - Failure behavior: `Initialize()` aborts if not set.

11. `m_configuredRandomAccessResourceIe`
    - Set by: `ConfigureRandomAccessResources()`.
    - Effect: FT-side random access resource advertisement and constraints for random-access transmission windows.
    - Role-specific requirement: mandatory for FT.

12. `m_configuredRandomAccessResourceIeSet`
    - Set by: `ConfigureRandomAccessResources()`.
    - Effect: FT start guard.
    - Failure behavior: `Start()` aborts for FT if not set.

13. `m_configuredAssociationControlIe`
    - Set by: `ConfigureAssociationControlIe()`.
    - Effect: PT association request content and keep-alive UL period logic (`DecodeUlPeriod()`).
    - Role-specific requirement: mandatory for PT.

14. `m_configuredAssociationControlIeSet`
    - Set by: `ConfigureAssociationControlIe()`.
    - Effect: PT start guard.
    - Failure behavior: `Start()` aborts for PT if not set.

15. `m_configuredRngStream`
    - Set by: `ConfigureRngStream()`.
    - Effect: overrides stream used by MAC backoff RNG (`m_backoffRandomVariable`).
    - Scope: reproducibility of random-access backoff behavior.

16. `m_configuredRngStreamSet`
    - Set by: `ConfigureRngStream()`.
    - Effect: switches RNG stream source from default node-id stream to configured stream.

17. `m_device`
    - Set by: `SetNetDevice()` (typically wired by NetDevice/MAC integration path in setup).
    - Effect: provides node identity, termination type access, and receive forwarding.
    - Failure behavior: `Initialize()` aborts if not set.

18. `m_proceedToBeaconTransmission` (default `true`)
    - Set by: `ProceedToBeaconTransmission(bool)`.
    - Effect: if `false`, execution ends after operating-channel selection; FT does not start beacon scheduling.
    - Usage: staged debugging of scan/channel-selection only.

19. `m_proceedToAssociationRequest` (default `true`)
    - Set by: `ProceedToAssociationRequest(bool)`.
    - Effect: PT ignores beacon-driven association progression when `false`.
    - Usage: isolate discovery/beacon parsing without association-request transmission.

20. `m_proceedToAssociationResponse` (default `true`)
    - Set by: `ProceedToAssociationResponse(bool)`.
    - Effect: FT accepts/records association requests but does not continue to association response handling when `false`.
    - Usage: staged debugging of FT-side receive path before response.

21. `m_proceedToDataTransmission` (default `true`)
    - Set by: `ProceedToDataTransmission(bool)`.
    - Effect: after association ACK verification, PT does not launch app/data flow if `false`.
    - Usage: stop flow after successful association.

22. `m_displayChannelEvaluations` (default `false`)
    - Set by: `DisplayChannelEvaluations(bool)`.
    - Effect: prints sorted per-channel free/possible/busy summary after operating channel selection.
    - Usage: channel-selection diagnostics.

23. `m_searchBestFtTimeMs` (default `0`)
    - Set by: `SetSearchBestFtTimeMs(double)`.
    - Effect: PT candidate collection window before `SelectBestFtCandidate()` runs.
    - Runtime rule: `0` means fallback to `2000 ms`; non-zero uses configured value.

## 3.2 Initialization and start abort matrix

### `Initialize()` hard requirements

`Initialize()` aborts if any of the following are missing:

- band number set (`m_bandNumberSet`)
- subcarrier scaling factor set (`m_subcarrierScalingFactorSet`)
- Fourier transform scaling factor set (`m_fourierTransformScalingFactorSet`)
- channel manager pointer (`m_channelManager`)
- termination point type set (`m_terminationPointTypeSet`)
- PHY pointer (`m_phy`)
- net device pointer (`m_device`)

### `Start()` role-specific hard requirements

- FT path aborts if random-access IE was not configured (`m_configuredRandomAccessResourceIeSet == false`).
- PT path aborts if association-control IE was not configured (`m_configuredAssociationControlIeSet == false`).

## 4) Recommended configuration workflow in simulation scripts

Use this order per device:

1. Create PHY/MAC/Node/NetDevice.
2. Apply global defaults:
   - `config.ApplyToPhy(phy)`
   - `config.ApplyToMac(mac)`
3. Set role and bindings:
   - `mac->SetTerminationPointType(...)`
   - `mac->SetPhy(phy)`
   - NetDevice <-> MAC/PHY/node wiring
4. Set role-specific IEs:
   - FT: `ConfigureRandomAccessResources(...)`
   - PT: `ConfigureAssociationControlIe(...)`
5. Optional advanced options:
   - `ConfigureRngStream(...)`
   - `ProceedTo...(...)`
   - `DisplayChannelEvaluations(true)`
   - `SetSearchBestFtTimeMs(...)`
6. Call `Initialize()` then `Start()`.

## 5) Practical notes

- Treat global values in `dect2020ext-config-values.h` as baseline definitions unless you are intentionally changing model internals.
- For reproducibility, configure RNG streams explicitly when comparing random-access behavior between runs.
- The `ProceedTo...` toggles are advanced experiment controls for partial protocol execution, not normal production defaults.