The `dect-nr-ext` implementation must be configured in both ways: through the global `Dect2020ExtConfig` model (together with `dect2020ext-config-values.h`) located in `/model/config` and custom options that are configured directly inside simulation scripts and can be modified on-demand. The global configuration (`Dect2020ExtConfig` and `dect2020ext-config-values.h`) is applied to all simulations and some variables in `dect2020ext-config-values.h` must not be changed under normal circumstances.

The global configuration model is divided into two parts: `dect2020ext-config-values.h` (a single header file) and `Dect2020ExtConfig` (config class). `dect2020ext-config-values.h` contains configuration variables that are used by `Dect2020ExtConfig` to configure `dect-nr-ext` models. Also, some models use constant values from `dect2020ext-config-values.h` for calculations.

The `Dect2020ExtConfig` class creates two objects that are shared by all `Dect2020Ext` models:
- `ns3::SpectrumChannel`
- `Dect2020ExtChannelManager`
`Dect2020ExtConfig` also creates an Isotropic Antenna for every PHY.
`Dect2020ExtConfig` has `ApplyToPhy` and `ApplyToMac` functions that bootstrap the values from `dect2020ext-config-values` to the corresponding `Dect2020ExtPhy` and `Dect2020ExtMac` models.
#### Values required for `Dect2020ExtMac` configuration:
Provided by `Dect2020ExtConfig`:
- Band number (currently only band 1 is supported)
- Subcarrier scaling factor (currently only value 1 is supported)
- Fourier transform scaling factor (currently only value 1 is supported)
- Pointer to `Dect2020ExtChannelManager`
Provided directly in the simulation file:
- Termination Point Type
- Pointer to `Dect2020ExtPhy` (that belongs to the MAC)
- Pointer to `Dect2020ExtNetDevice` (that belongs to the MAC)
- Boolean value for proceeding to beacon transmission (if the simulation should stop at the background scan)
- If the device is FT:
	- Random Access Resource IE
- If the device is PT:
	- Association Control IE