The information is taken and cited from the official ns-3 Spectrum Module documentation: https://www.nsnam.org/docs/models/html/spectrum.html

The Spectrum module in ns-3 is designed to simulate the frequency-dependent aspects of radio communication id ns-3. In DECT-2020 NR implementation, the Spectrum module is used for transmissions (`Dect2020ExtPhy` inherits from `ns3::SpectrumPhy`), `Dect2020ExtInterference` (used to calculate power from all signals at Rx), and RSSI measurements.
#### Signal Model
The signal simulation is covered by the `ns3::SpectrumSignalParameters` class. The signal parameters provide the following information for a signal transmitted or received by PHY layers:
Original values from `ns3::SpectrumSignalParameters`:
- A pointer to the transmitting PHY device
- A pointer to the transmitting antenna
- A pointer to the transmitting mobility model
- The duration of signal
- The Power Spectral Density (PSD) of the signal, which is assumed to be constant for the duration of the signal
Values added for the `Dect2020Ext` module:
- A packet which is transmitted
- The channel number where the transmission is happening
- Optional data

The PSD is represented as a set of discrete scalar values each corresponding to a certain sub-band in frequency. The set of frequency sub-bands to which the PSD refers to is defined by an instance of the `ns3::SpectrumModel` class. The PSD itself is implemented as an instance of the `ns3::SpectrumValue` class which contains a reference to the associated `ns3::SpectrumModel` class instance. The `ns3::SpectrumValue` class provides several arithmetic operators to allow to perform calculations with PSD instances. Additionally, the `ns3::SpectrumConverter` class provides means for the conversion of `ns3::SpectrumValue` instances from one `ns3::SpectrumModel` to another.
#### Channel/PHY Interface
There are two types of `ns3::SpectrumChannel` models: `ns3::SingleModelSpectrumChannel` and `MultiModelSpectrumChannel`. In DECT-2020 NR implementation, `ns3::SingleModelSpectrumChannel` is currently used. The following functionality is used in the implementation:
- Propagation loss modeling through a model based on `ns3::PropagationLossModel`. Only linear models (where the loss value does not depend on the transmission power) can be used. These models are single-frequency in the sens that the loss value is applied equally to all components of the power spectral density.
- Propagation delay modeling through a model based on `ns3::PropagationDelayModel`. The delay in independent of frequency and applied to the signal as a whole. Delay modeling is implemented by scheduling the `StartRx` event with a delay respect to the `StartTx` event.

In the current simulation, `ns3::SpectrumModel` is not a participant in any computation. It is a shared contract that lets every PSD produced in the simulation (TX PSD, interference accumulator, noise floor) use the same indexing - so that `(*psd)[5]` means "channel 1662" to every PHY, the TX path, and the interference model simultaneously.

The actual fl, fc, fh numbers inside the `BandInfo` structs are metadata that would only become relevant if you switched to a frequency-dependent propagation loss model (`ns3::SpectrumPropagationLossModel`) or started calling `Integral` to integrate power over bandwidth. Until then, only the count (number of bands = size of the array) and the UID (identity check) matter.

