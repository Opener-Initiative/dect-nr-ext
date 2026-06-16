# Spectrum Management Procedure

## Overview

The Spectrum Management Procedure governs how an RD selects operating channels and coordinates radio resources. The procedure differs depending on whether the RD operates in FT mode or PT mode.

**FT mode** (coordinator):
1. Performs a background scan to select the best operating channel.
2. Transmits Network and Cluster Beacons on the selected channel so that PT devices can discover and join.

**PT mode** (client):
1. Scans frequency channels to discover FT candidates by receiving their beacons.
2. Selects the best FT candidate and initiates association.

A single RD operating simultaneously in both FT and PT modes is **not supported**. Each RD operates in exactly one mode.

---

## Background Scan (FT Mode)

Corresponds to standard clause **5.1.2**. The background scan is triggered once at startup via `Dect2020ExtMac::InitializeNetwork()` and runs exactly once - there is no periodic re-scan.

### `Dect2020ExtMac::BackgroundScan()`

Creates a shared `Dect2020ExtBackgroundScanContext` that accumulates results across all channels. Iterates over every channel in the configured band (from `nStart` to `nEnd` as reported by the `Dect2020ExtChannelManager`) and schedules one `Dect2020ExtMac::MeasureRSSI1OnChannel()` call per channel, staggered by 10 ms (one frame duration) to avoid overlapping measurements.

### `Dect2020ExtMac::MeasureRSSI1OnChannel()`

For a given channel, schedules a `Dect2020ExtMac::StartSubslotScan()` and a `Dect2020ExtMac::EndSubslotScan()` for every subslot in one frame. The scan duration is exactly `SCAN_MEAS_DURATION = 24` slots (one full frame), which satisfies the standard's minimum measurement duration requirement. The number of subslots per slot is determined by the configured subcarrier scaling factor via the `Dect2020ExtChannelManager`.

### `Dect2020ExtMac::StartSubslotScan()`

Called at the start of each subslot. Tunes the PHY to the target channel by calling `Dect2020ExtMac::SetOperatingChannelNumber()`, then schedules 5 evenly-spaced `Dect2020ExtMac::SampleRSSIFromInterference()` calls within the subslot duration.

### `Dect2020ExtMac::SampleRSSIFromInterference()`

Reads the instantaneous interference power for the target channel directly from the PHY interference model (`m_interference->m_allSignals`) and converts it from watts to dBm. The result is appended to the per-subslot sample vector in the `Dect2020ExtRssi1OnChannelContext`.

> **Design choice - deviation from standard:** The standard (clause 5.1.2) requires using `max(RSSI-1)` within the subslot duration. The implementation instead computes the **average** of the 5 collected samples. This is considered acceptable in a simulation environment where interference levels are relatively stable.

### `Dect2020ExtMac::EndSubslotScan()`

Called at the end of each subslot. Stores the channel's context into the shared `Dect2020ExtBackgroundScanContext`. When the final subslot of the final channel completes, calls `Dect2020ExtMac::OperatingChannelSelection()` to evaluate results and pick a channel.

### `Dect2020ExtMac::OperatingChannelSelection()`

Processes all collected RSSI samples and selects the operating channel. The steps are:

1. **Average RSSI per subslot:** For each subslot on each channel, compute the mean of its 5 RSSI samples.
2. **Classify each subslot** using the thresholds defined in the standard:
   - `free` if average RSSI ≤ `RSSI_THRESHOLD_MIN` (−85 dBm)
   - `possible` if `RSSI_THRESHOLD_MIN` < average RSSI ≤ `RSSI_THRESHOLD_MAX` (−50 dBm)
   - `busy` if average RSSI > `RSSI_THRESHOLD_MAX`
3. **Rank channels** by their number of `free` subslots in descending order.
4. **Select** the top-ranked channel and store it as `Dect2020ExtMac::m_operatingChannelNumber`.
5. **Proceed** to `Dect2020ExtMac::StartBeaconTransmission()` (if `Dect2020ExtMac::m_proceedToBeaconTransmission` is `true`).

> **Known deviation - to be fixed:** The standard specifies a hierarchical selection algorithm: prefer channels where *all* subslots are free; if none exist, prefer the channel with the fewest `busy` subslots; break ties by fewest `possible` subslots. The implementation uses a simpler sort by most `free` subslots, which can select a different channel than the standard algorithm would. This will be corrected in a future revision.

> **Known missing feature (not planned):** The constant `SCAN_SUITABLE = 0.75` is defined but never evaluated. The standard allows the RD to stop the scan early and proceed to beacon transmission as soon as one channel has ≥ 75 % free+possible subslots. The implementation always completes the full sweep across all channels before selecting.

> **Known missing feature (not planned):** The `scanStatusValid` timer (`SCAN_STATUS_VALID = 300 s`) is defined but never started. The standard requires the RD to track scan result validity and re-initiate the background scan when the timer expires or when the channel load exceeds `CHANNEL_LOADED` (not defined in this implementation). The scan result is considered valid for the entire simulation duration.

### `Dect2020ExtMac::DisplayChannelEvaluations()`

An optional debug helper. When enabled via `Dect2020ExtMac::DisplayChannelEvaluations(true)`, prints a formatted table of all channel evaluations (channel ID, free/possible/busy/total subslot counts, and free percentage) to the NS-3 log at `INFO` level after channel selection completes.

---

## Beacon Transmission (FT Mode)

Corresponds to standard clause **5.1.5**. After channel selection, the FT transmits Network and Cluster Beacons periodically on the selected operating channel.

> **Design choice:** Beacon transmissions are always **single-channel**. The Network Beacon is only ever sent on `Dect2020ExtMac::m_operatingChannelNumber`. The standard permits the FT to transmit the Network Beacon on multiple channels to reduce PT search time; this multi-channel sweep is not implemented. PT devices compensate by actively cycling through all band channels during discovery (see [Network Discovery - PT Mode](#network-discovery-and-ft-selection-pt-mode)).

> **Design choice:** Beacon subslot positions within a frame are **fixed**. The Network Beacon is always placed at **subslot 0** (start of the first slot) and the Cluster Beacon is always placed at **subslot 4** (start of the second slot). These offsets are hardcoded and are not negotiated or configurable at runtime.

### `Dect2020ExtMac::StartBeaconTransmission()`

Registers two periodic operations with the MAC scheduler:

| Beacon | Subslot | Default Period |
|---|---|---|
| Network Beacon | 0 | 100 ms (`NETWORK_BEACON_PERIOD_100MS`) |
| Cluster Beacon | 4 | 50 ms (`CLUSTER_BEACON_PERIOD_50MS`) |

Both operations are scheduled to start on the next frame (`SFN + 1`). The scheduler calls `Dect2020ExtMac::StartNetworkBeaconTransmission()` and `Dect2020ExtMac::StartClusterBeaconTransmission()` at the appropriate subslot boundaries on every configured period.

### `Dect2020ExtMac::StartNetworkBeaconTransmission()` / `Dect2020ExtMac::StartClusterBeaconTransmission()`

Thin scheduler callbacks. Each immediately schedules the corresponding `Dect2020ExtMac::Send*BeaconOnChannel()` call at time offset zero (i.e. within the same simulation time step).

### `Dect2020ExtMac::SendNetworkBeaconOnChannel()` / `Dect2020ExtMac::SendClusterBeaconOnChannel()`

Perform two PHY state checks before transmitting:

- If the PHY is in `TX` state: **fatal error** - this would indicate a scheduler conflict.
- If the PHY is in `RX` state: the beacon is **silently dropped** and the dropped-beacon counter is incremented. There is no retry or deferral.

> **Design choice:** On a PHY collision (another packet is being received at the scheduled beacon subslot), the beacon is dropped rather than deferred. Deferring would require tracking the end of the incoming reception and re-scheduling within the same subslot, adding complexity with limited simulation value.

If the PHY is idle, the beacon packet is built (`Dect2020ExtMac::BuildNetworkBeacon()` / `Dect2020ExtMac::BuildClusterBeacon()`) and passed to the PHY via `Dect2020ExtMac::m_phy->Send()`.

**Network Beacon content** (built by `Dect2020ExtMac::BuildNetworkBeacon()`):
- Network Beacon period: 100 ms
- Cluster Beacon period: 50 ms
- Next cluster channel: `Dect2020ExtMac::m_operatingChannelNumber`
- Time to next Cluster Beacon: computed as the subslot-offset difference between subslot 0 and subslot 4
- TX power and power constraint fields: not included

**Cluster Beacon content** (built by `Dect2020ExtMac::BuildClusterBeacon()`):
- Cluster channel number
- System Frame Number (SFN)
- Random Access Resource IE (configured externally by the user)

---

## Network Discovery and FT Selection (PT Mode)

Corresponds to standard clause **5.1.4**. When an RD starts in PT mode, it scans all channels to collect FT candidates, then selects the best one.

### `Dect2020ExtMac::DiscoverNetworks()`

Cycles through all channels in the band, advancing one channel every 10 ms (one frame). On each cycle it sets `Dect2020ExtMac::m_operatingChannelNumber` to the next channel, which causes the PHY to listen on that channel. The loop stops automatically when the association status advances to `WAITING_FOR_SELECTED_FT`, `ASSOCIATION_PENDING`, or `ASSOCIATED`.

### `Dect2020ExtMac::HandleBeaconPacket()`

Called when the PHY delivers a received beacon. Parses the Beacon Header to extract the FT's Network ID and Long RD ID, then branches on the beacon type:

- **Network Beacon:** Extracts the next cluster channel number and the time-to-next-cluster-beacon from the Network Beacon Message IE. This information is stored in `Dect2020ExtFtInfo` for the corresponding FT candidate and used to tune to the correct channel when waiting for the cluster beacon.
- **Cluster Beacon:** Extracts the Random Access Resource IE and Cluster Beacon Message IE, then calls `Dect2020ExtMac::EvaluateClusterBeacon()`. If no Network Beacon from this FT has been received previously, the cluster channel is assumed to be the current operating channel.

### `Dect2020ExtMac::EvaluateClusterBeacon()`

Handles the FT candidate state machine on the PT side:

- If already `ASSOCIATED`: ignores the beacon.
- If `NOT_ASSOCIATED` (first cluster beacon ever received): transitions to `ASSOCIATION_PREPARING`, resumes `Dect2020ExtMac::DiscoverNetworks()` to keep collecting candidates, and schedules `Dect2020ExtMac::SelectBestFtCandidate()` after `searchBestFtTimeMs` (default: **2000 ms**, configurable via `Dect2020ExtMac::SetSearchBestFtTimeMs()`).
- If `WAITING_FOR_SELECTED_FT` and the beacon is from the previously selected FT: schedules `Dect2020ExtMac::SendAssociationRequest()` immediately.

### `Dect2020ExtMac::SelectBestFtCandidate()`

After the discovery window expires, selects the best FT from all entries in `Dect2020ExtMac::m_ftCandidates`:

1. Iterates all discovered FT candidates.
2. Selects the candidate with the **highest RSSI-2** value.
3. Switches `Dect2020ExtMac::m_operatingChannelNumber` to the selected FT's cluster channel.
4. Transitions to `WAITING_FOR_SELECTED_FT` and waits for the next Cluster Beacon from the selected FT to trigger the Association Request.

If `Dect2020ExtMac::m_ftCandidates` is empty (no FT was heard during the discovery window), the PT logs a warning and remains in `NOT_ASSOCIATED` with **no automatic retry**. It will stay in this state for the rest of the simulation.

> **Known missing feature (not planned):** The standard requires a minimum quality gate before an FT is accepted: `RSSI-2 ≥ MIN_SENSITIVITY_LEVEL + MIN_QUALITY`. This check is not performed. All heard FT candidates are accepted regardless of signal strength, and the one with the highest RSSI-2 is always selected.

> **Known missing feature (not planned):** The standard specifies that if a cluster beacon contains a Route Info IE, the PT should calculate its route cost to each FT and prefer the lower-cost path, falling back to highest RSSI-2 only on a tie. Route cost calculation is not implemented; the PT always selects strictly by highest RSSI-2.

---

## Last Minute Scan (Clause 5.1.3)

The Last Minute Scan procedure (pre-transmission RSSI-1 validation when `DECT_PROTECTED` is true) is **entirely unimplemented**. Neither `DECT_PROTECTED`, `dectProTime`, nor any pre-transmission subslot status check exists in the codebase. This is a known omission and is not planned for implementation.

---

## Joining Beacon (Clause 5.1.5)

The Joining Beacon message type, which enables secure authentication procedures, is **out of scope**. No authentication procedure is implemented. Only Network and Cluster Beacons are supported.

---

## Summary of Deviations from the Standard

| # | Area | Clause | Description | Classification |
|---|---|---|---|---|
| 1 | RSSI-1 measurement | 5.1.2 | Average of 5 samples used instead of `max(RSSI-1)` | Design choice |
| 2 | Channel selection algorithm | 5.1.2 | Sorted by most free subslots; standard requires hierarchical free→busy→possible ranking | Known deviation — to be fixed |
| 3 | `SCAN_SUITABLE` early-stop | 5.1.2 | Constant defined but not evaluated; scan always completes full sweep | Known missing feature |
| 4 | `scanStatusValid` timer | 5.1.2 | Not started; scan results never expire; no re-scan triggered | Known missing feature |
| 5 | `CHANNEL_LOADED` re-scan | 5.1.2 | Not defined or evaluated | Known missing feature |
| 6 | Last Minute Scan | 5.1.3 | Entirely unimplemented (`DECT_PROTECTED`, `dectProTime` absent) | Known missing feature |
| 7 | PT minimum quality gate | 5.1.4 | `MIN_SENSITIVITY_LEVEL + MIN_QUALITY` check not performed | Known missing feature |
| 8 | PT route cost selection | 5.1.4 | Route Info IE not evaluated; always selects by highest RSSI-2 | Known missing feature |
| 9 | PT no-FT retry | 5.1.4 | PT stays NOT_ASSOCIATED permanently if no FT found; no retry | Intended behavior |
| 10 | Single-channel Network Beacon | 5.1.5 | Network Beacon sent on operating channel only; no multi-channel sweep | Design choice |
| 11 | Fixed beacon subslot positions | 5.1.5 | Network Beacon at subslot 0, Cluster Beacon at subslot 4 — hardcoded | Design choice |
| 12 | Beacon drop on PHY collision | 5.1.5 | Beacon dropped when PHY is in RX state; no retry or deferral | Design choice |
| 13 | Joining Beacon | 5.1.5 | Not implemented | Out of scope |
| 14 | Optimum power level selection | 5.1.6 | TX power is fixed; no power control | Design choice |
| 15 | Simultaneous FT+PT mode | 5.1.1 | Single mode only; RD cannot act as FT and PT simultaneously | Design choice |
| 16 | Short RD ID collision detection | 5.1.2 | Not checked during background scan | Known missing feature |

---

## Decision Log

- **Average vs. max RSSI-1:** Averaging 5 intra-subslot samples was chosen over `max()` for implementation simplicity. In a simulation, interference levels are deterministic and stable within a subslot, so the practical difference is minimal.
- **Single-scan at startup:** The background scan runs once and results are permanent. This avoids the complexity of scan refresh, timer management, and channel re-selection mid-operation, which are not required by the target simulation scenarios.
- **Fixed beacon subslots:** Hardcoding subslot 0 (Network Beacon) and subslot 4 (Cluster Beacon) simplifies scheduler configuration and ensures predictable beacon timing without requiring dynamic slot negotiation.
- **Single-channel beacon:** Sending the Network Beacon only on the operating channel is simpler and sufficient because the PT performs an active channel sweep. Multi-channel beacon sweeping would only reduce PT discovery latency, which is not a primary concern in current simulation scenarios.
- **Beacon drop on collision:** Deferring a beacon transmission after a received packet ends requires fine-grained timing within an already-occupied subslot. Dropping is acceptable because missed beacons are periodic and the next period will retry.
- **PT discovery window:** The default 2000 ms window for `Dect2020ExtMac::SelectBestFtCandidate()` is configurable and was chosen to be large enough to hear at least one beacon period from every FT in the band before committing to a candidate.
- **Channel selection algorithm (to be fixed):** The current sort-by-free approach is a simplification. The standard-compliant hierarchical algorithm (all-free → fewest-busy → fewest-possible) will be implemented in a future revision.
- **No-FT-found behavior:** When no FT candidates are heard, the PT stays stuck in `NOT_ASSOCIATED`. This is intentional — in the target simulation scenarios the network is always present, so an automatic retry would only mask a misconfiguration.
- **PT quality gate and route cost omitted:** In controlled simulation environments, all FTs are within range and signal quality is guaranteed by topology design. The quality gate and route cost logic add complexity without changing the outcome in the target scenarios.
- **Last Minute Scan omitted:** `DECT_PROTECTED` mode and `dectProTime` are irrelevant to the target simulation scenarios, which do not model protected DECT spectrum. The feature is omitted entirely.
