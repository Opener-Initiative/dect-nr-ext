# Tasks
- Ensure that all MAC PDU models use DECT2020EXT_MAC_PDU_FATAL macro
- Add range checking to all modules in MAC PDU (output errors with the macro)
- Add comments to every MAC PDU module (with names)
- Describe Dect2020ExtTypes as possible future work
- Write tests for:
	- MAC PDU
	- MAC: MAC PDU size calculation in slots and subslots
	- PHY: MAC PDU duration in nanoseconds (based on its size in slots or subslots)
- Rename HARQ to ARQ or stop and wait?
# Suggestions
1. Update the Statistics module - include delay, timing of RACH, channel occupancy
2. ARQ Protocols:
	1. Stop and Wait
	2. GobackN
	3. Selective Repeat
3. ARQ protocols on Layer 2 DLC (for flow control and error control)
4. Flow Control is to adapt the Tx transmission to the Rx speed
5. HARQ = ARQ (L2) + FEC (Turbo Coding on PHY)
6. Add probability-based decoding errors on the PHY (Packet Error Rate) - CRC abstraction
# Questions
1. During beacon transmission and data transfer, should association messages be prioritized inside a Tx queue?

# Tasks
1. Create a tutorial
2. How to change code without damaging the repository

### Summary

|Check|Result|
|---|---|
|Same `SpectrumModel` pointer in interference `m_allSignals`|✓ via ChannelManager cache|
|Same `SpectrumModel` pointer in TX PSD|✓ via ChannelManager cache|
|`GetRxSpectrumModel()` returns correct model|✗ returns nullptr — `m_spectrumModel` never assigned|
|Compatibility enforced when adding signal to interference|✗ direct `double` indexing bypasses `NS_ASSERT`|
|`m_rxSignal` in interference|dead, never used|