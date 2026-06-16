# Random Access Procedure, HARQ Operation, and Data Transmission

> All standard clause references in this document refer to **ETSI TS 103 636-4 V2.1.1** (DECT-2020 NR Part 4: MAC layer).

## Overview

The Random Access Procedure is the only transmission mechanism used by PT devices in this implementation. It is used for both the Association Request (control) and all subsequent data packets. The FT responds to every received uplink packet with a HARQ ACK sent on a scheduled downlink resource; this ACK is not subject to RACH contention.

The implementation uses **Stop-and-Wait ARQ**: a single HARQ process (`HARQ_MAX_PROCESSES = 1`, process number 0) is available at any time. The PT transmits one packet, waits for the ACK, and only advances to the next packet after the acknowledgement has been verified by `Dect2020ExtMac::VerifyHarqAck()`. `Dect2020ExtMac::VerifyHarqAck()` is the **sole serialization point** - it is the only function that decides what happens next (retransmit, send next packet, or wait for queue).

The `bool data` flag carried through the procedure distinguishes the two packet types:
- `data = false`: Association Request (control packet - one-shot)
- `data = true`: Data MAC PDU (repeating data cycle)

The `bool random` flag on `m_phy->Send()` controls the PHY header format:
- `random = false`: Beacon packets (PHY Control Field Type 1)
- `random = true`: Packets sent via Random Access (PHY Control Field Type 2)

---

## Configuration and Critical Constraints

RACH resources are announced in the Cluster Beacon via the `Dect2020ExtRandomAccessResourceIe` and configured externally via `Dect2020ExtMac::ConfigureRandomAccessResources()`. The following parameters govern the entire procedure:

| Parameter | IE field | Description |
|---|---|---|
| `StartSubslot` | `GetStartSubslot()` | First subslot of the RACH window in a frame |
| `RaraLength` | `GetRaraLength()` | Number of subslots in the RACH window |
| `LengthType` | `GetLengthType()` | **Must always be `false` (subslots)**. Slots are not supported; a fatal error is raised if this is set to `true` |
| `MaxRachLength` | `GetMaxRachLength()` | Maximum packet length for a single RACH transmission in subslots |
| `ResponseWindow` | `GetResponseWindow()` | Subslots after TX start within which an ACK must arrive |
| `CwMinSig` | `GetCwMinSig()` | `CW_MIN = 8 × CwMinSig` |
| `CwMaxSig` | `GetCwMaxSig()` | `CW_MAX = 256 × CwMaxSig` |

> **IMPORTANT - Critical configuration constraint:** The response window must always be configured as:
>
> **`responseWindow = 2 × MaxRachLength + harqFeedbackDelay + 1`**
>
> This budgets one `MaxRachLength` for the request TX, one for the response TX, one HARQ feedback delay, and one subslot for propagation. If the configured value is smaller than the real exchange duration, `Dect2020ExtMac::VerifyHarqAck()` will fire before the ACK arrives, declare a false collision, double `CW_CURRENT`, and retransmit - leading to duplicate transmissions after the device is already associated. No runtime check enforces this formula; correctness relies entirely on the simulation user.

> **IMPORTANT - Frame boundary constraint:** RACH resources must always be configured to fit entirely within a single frame. `Dect2020ExtMac::CheckIfRachTxFitsInRemainingResources()` verifies that the full TX + response window cycle fits in the remaining RACH window, which indirectly prevents the response subslot from overflowing the frame (see [D4 in the Summary](#summary-of-deviations-from-the-standard)). If RACH resources are configured to cross a frame boundary, `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs()` can throw `std::out_of_range` and crash the simulation.

---

## Random Access Transmission

### `Dect2020ExtMac::InitiateHarq()`

This function is responsible for triggering the HARQ Operation and the Random Access Procedure. It is always called by PT devices. It controls when `Dect2020ExtMac::InitiateRandomAccess()` is called and how the packet is dequeued from the front of the TX queue `Dect2020ExtMac::m_txQueue`. The dect-nr-ext implementation is designed to transmit packets using the Random Access Procedure by first putting them into the TX queue, initiating the HARQ Operation, and then initiating the Random Access Procedure. The result of this function is correctly scheduling `Dect2020ExtMac::InitiateRandomAccess()` with a prepared HARQ process for the transmission. `Dect2020ExtMac::InitiateRandomAccess()` must always be called either inside or at the start of the RACH resources. The RACH resources are defined as RACH start subslot + RACH length so that:

1. If RACH start subslot is 10, it is possible to get the time in nanoseconds till the start of the RACH resources in the current frame by calling `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs()` which will return the time in nanoseconds till subslot 10 start.
2. If RACH start subslot is 10 and RACH length is 30, it is possible to calculate the total RACH resource duration by calling `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs()` with RACH start subslot + RACH length as a parameter. RACH start subslot + RACH length = 40, but the actual subslots included in the RACH resources are from subslot 10 to subslot 39. This way, it is safe to call `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs()` to get the full duration of the RACH resources.

The function does the following actions:

1. Call `Dect2020ExtMac::FindFreeHarqProcess()` to find a free HARQ process. Currently, only 1 HARQ process is available (with process number 0). The function will raise `NS_FATAL_ERROR` if a free HARQ process was not found. There must always be a free HARQ process when `Dect2020ExtMac::InitiateHarq()` is called. Stop-and-Wait ARQ protocol is used in the context of the HARQ Operation during the Random Access Procedure.
2. Pop a packet from the front of `Dect2020ExtMac::m_txQueue`.
3. Prepare the HARQ process:
   1. Set the state to `WAITING_ACK`.
   2. Set NDI (New Data Indication) to `true` (new data with no previous versions). NDI is always `true`, even on retransmissions - see [F8 in the Summary](#summary-of-deviations-from-the-standard).
   3. Assign the TX packet to the process (to track which packet is currently transmitted and which process it belongs to).
   4. Assign the boolean `data` value to the process to indicate whether this is a data packet or an Association Request Message.
4. Build the PHY Control Field Type 2.
5. Conduct a sanity check to assert that the RACH length is defined in subslots (fatal error if `LengthType == true`).
6. Correctly schedule `Dect2020ExtMac::InitiateRandomAccess()` with a new context to fulfil one of these conditions:
   1. If the RACH resources are yet to come in the current frame → schedule `Dect2020ExtMac::InitiateRandomAccess()` at the start of the RACH resources in the current frame.
   2. If the device is already inside or at the start of the RACH resources → execute `Dect2020ExtMac::InitiateRandomAccess()` immediately.
   3. If the device is outside of (past) the RACH resources → schedule `Dect2020ExtMac::InitiateRandomAccess()` at the next RACH resources in the next frame.

### `Dect2020ExtMac::InitiateRandomAccess()`

This function is designed to purely control the Random Access Procedure. It is supposed to be called either inside or at the start of the RACH resources. `Dect2020ExtMac::InitiateHarq()` is responsible for correctly triggering `Dect2020ExtMac::InitiateRandomAccess()`.

The function directly follows the DECT-2020 NR standard and does the following actions:

1. Get the remaining RACH resource duration in microseconds. Using `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs()` with `raraStartSubslot + raraLength` as a parameter correctly returns the full duration when called at the start of the RACH resources, or only the remaining duration when called from inside the window.
2. Calculate all possible start positions (`START_POSITION_INTERVAL_US = STF_US + GI_US = 83.335 µs` for subcarrier scaling factor 1). Start positions are at `0, 1, 2, 3, …` times `(STF + GI)` from the current point in the RACH window.
3. If a new Random Access context is created (`newContext = true`):
   1. Calculate `CW_MIN = 8 × CwMinSig` and `CW_MAX = 256 × CwMaxSig` from the Random Access Resource IE.
   2. Set `CW_CURRENT = CW_MIN`. A new context indicates that the Random Access Procedure is being conducted for a new packet. This means `CW_MIN`, `CW_MAX`, and `CW_CURRENT` are reset for each new packet taken from the TX queue. Retransmissions due to collisions or missing ACK reuse the same context and do not reset `CW_CURRENT`.
   3. Assign the new context to the corresponding HARQ process via `process->randomAccessContext`.
4. If an existing context is reused (`newContext = false`):
   1. Update the start positions (since they are recalculated each time a retransmission or deferral happens, as the remaining RACH duration changes).
5. Call `Dect2020ExtMac::InitiateBackoff()`.

### `Dect2020ExtMac::InitiateBackoff()`

This function initiates the backoff and handles the case where all available start positions are exceeded. If positions are exhausted, the procedure defers to the next frame by scheduling `Dect2020ExtMac::InitiateRandomAccess()` with the existing context (`newContext = false`). It is important to restart from `Dect2020ExtMac::InitiateRandomAccess()` rather than trying to patch up existing positions, to avoid timing issues: it is safer to recalculate start positions fresh for the new RACH window than to track whether existing positions still apply.

The function does the following actions:

1. Conduct a sanity check via `Dect2020ExtMac::CheckIfMacPduFitsInMaxRandomAccessTxTime()` to assert that the packet fits within the Maximum Random Access TX time. A fatal error is raised if it does not - this indicates a misconfiguration.
2. Draw a random backoff value uniformly distributed between 0 and `CW_CURRENT` (inclusive). This value is the start position **index**.
3. If the backoff value exceeds the available start positions (i.e. the drawn index is out of range for the current RACH window), defer: schedule `Dect2020ExtMac::InitiateRandomAccess()` at the start of the RACH resources in the next frame with the existing context.

   > **Known possible deviation (unresolved - F4):** The standard (clause 5.3.3) states the timer `rachBackOff` should be kept running across RACH occurrences, freezing between windows and resuming in the next one (a single persistent countdown). The implementation instead draws a fresh random value each time a deferral occurs. On a busy-LBT re-initiation this is standard-correct (clause 5.3.3 says "initiate the backoff process" on busy), but on a defer-due-to-positions-exceeded path, the residual count is discarded and re-randomized rather than carried forward. This changes the per-attempt backoff distribution under contention.

4. Calculate the backoff duration in microseconds from `randomAccessContext->startPositions[backoffValue]`.
5. Schedule `Dect2020ExtMac::PerformLbt()` with the backoff delay (`rachBackOff` timer).

### `Dect2020ExtMac::PerformLbt()`

The function performs the LBT (Listen Before Talk) protocol by sensing the energy on the channel 5 times within one `START_POSITION_INTERVAL_US = STF + GI`. The sampling points are fixed offsets from the start of the LBT window:

1. `0.0 µs`: Start of STF
2. `STF_US / 2.0 µs`: Middle of STF
3. `STF_US µs`: Start of GI
4. `STF_US + (GI_US / 2.0) µs`: Middle of GI
5. `STF_US + GI_US µs`: End of GI

The function resets the LBT sampling context (`lbtSampleCount`, `lbtRssiSamples`), then loops through the sampling points and uses them as delays to schedule `Dect2020ExtMac::SampleLbtRssi()`.

> **Known possible deviation (unresolved - F3):** The standard (clause 5.3.1/5.3.3) states that for start position `k`, the channel must be sensed free for `MINIMUM_LBT_PERIOD` immediately **before** position `k`, and transmission begins **at** position `k`. The implementation senses the channel **at** position `k` (across the interval `[k·I, k·I + (STF+GI)]`) and transmits at the end of that window - effectively at position `k+1`. The essential invariants hold (a full `MINIMUM_LBT_PERIOD` is sensed before transmitting, two PTs with the same backoff value still collide), but the absolute alignment to the nominal start-position grid is shifted by one interval.

### `Dect2020ExtMac::SampleLbtRssi()`

The function does the following actions:

1. Get the current channel number and calculate the channel index relative to the band's first valid channel.
2. Sample the channel power from the PHY interference model (`Dect2020ExtPhy::m_interference->m_allSignals`) and convert it from watts to dBm.
3. Store the sample in `randomAccessContext->lbtRssiSamples` and increment `lbtSampleCount`.
4. If all 5 samples are collected (`lbtSampleCount >= lbtTotalSamples`), call `Dect2020ExtMac::EvaluateLbtChannel()`.

### `Dect2020ExtMac::EvaluateLbtChannel()`

The function does the following actions:

1. Conduct a sanity check to assert the device is not currently transmitting another packet (`PHY state == TX` → fatal error). This check exists because a past implementation error caused double-transmit; the `TX` state here should be unreachable under correct operation.
2. Check if the full RACH TX cycle fits into the remaining RACH resources via `Dect2020ExtMac::CheckIfRachTxFitsInRemainingResources()`. The fit check compares the **response window duration** (not just the MAC PDU TX duration) against the remaining RACH time - ensuring the entire request + response exchange completes inside the RACH window.

   > **Design choice (F1):** The standard (clause 5.3.3) requires only the MAC PDU to fit in the remaining resources. The implementation requires the full response window to fit, which is a stricter, self-consistent model: `Dect2020ExtMac::VerifyHarqAck()` is scheduled `responseWindow × subslotDuration` after TX start, so gating on the response window guarantees the whole exchange stays inside the window.

3. If the RACH TX **fits** in the remaining resources:
   1. Compute the average RSSI across the 5 LBT samples and classify the channel as `free` (≤ `RSSI_THRESHOLD_MIN = −85 dBm`), `possible` (`RSSI_THRESHOLD_MIN` < RSSI ≤ `RSSI_THRESHOLD_MAX = −50 dBm`), or `busy` (> `RSSI_THRESHOLD_MAX`).
   2. If the channel is `free` or `possible` **and** the PHY has not entered `RX` in the meantime (race condition: the PHY may transition to `RX` between the last LBT sample and this evaluation):
      1. Transmit the packet via `m_phy->Send(packet, phyControlFieldType2, random=true)`.
      2. Schedule `Dect2020ExtMac::VerifyHarqAck()` with a delay of `responseWindow × subslotDurationNs`.
   3. If the channel is `busy` or the PHY is in `RX` → re-initiate the Random Access Procedure immediately with the **same context** (`newContext = false`). `CW_CURRENT` is **not** changed on a busy-channel outcome (per clause 5.3.3: doubling only happens on a missing response).
4. If the RACH TX **does not fit** in the remaining resources → defer the TX to the next RACH occurrence in the next frame with the same context.

### `Dect2020ExtMac::CheckIfMacPduFitsInMaxRandomAccessTxTime()`

Checks whether the packet size (in subslots) is within the Maximum Random Access TX time from the Random Access Resource IE. Called from `Dect2020ExtMac::InitiateBackoff()` as a configuration sanity check. A mismatch raises a fatal error.

### `Dect2020ExtMac::CheckIfRachTxFitsInRemainingResources()`

Checks whether the full TX + response cycle (`responseWindow × subslotDurationNs`) fits within the remaining RACH window (`GetTimeTillSubslotStartNs(raraStartSubslot + raraLength)`). This is the gate in `Dect2020ExtMac::EvaluateLbtChannel()` that prevents transmission when there is insufficient time left. It also provides the indirect mitigation against the D4 frame-overflow risk, since a properly sized response window and RACH window guarantee the response subslot stays inside the frame.

---

## HARQ Operation and Response Handling

### `Dect2020ExtMac::VerifyHarqAck()` - The Single Decision Point

This function is the **sole serialization point** for the stop-and-wait cycle. It fires exactly once per transmission at the response-window boundary (scheduled by `Dect2020ExtMac::EvaluateLbtChannel()`), and makes the single authoritative decision on what happens next.

The function does the following actions:

1. Find the HARQ process by process number from the PHY Control Field Type 2 (fatal error if not found).
2. Check the process state:
   - **If `state == WAITING_ACK`** (no ACK was received before the response window expired):
     1. Apply exponential backoff: `CW_CURRENT = min(CW_CURRENT × 2, CW_MAX)`. This is the only place `CW_CURRENT` is doubled - CW does not change on busy-channel outcomes.
     2. Schedule `Dect2020ExtMac::InitiateRandomAccess()` with the same context (`newContext = false`) using the same RACH scheduling logic as `Dect2020ExtMac::InitiateHarq()` (current frame / immediate / next frame).
   - **If `state == FREE`** (ACK was already received and the process was reset by `Dect2020ExtMac::HandleHarqAck()`):
     - If `data == false` (Association Request acknowledged):
       1. Assert that `m_associationStatus == ASSOCIATED` (fatal error otherwise - the device should be associated before this path is reached).
       2. If `m_proceedToDataTransmission == true`: call `Dect2020ExtMac::LaunchApp()` and `Dect2020ExtMac::StartKeepAlivePeriod()` to begin the data transmission phase.
     - If `data == true` (Data packet acknowledged):
       1. If `m_txQueue` is non-empty: call `Dect2020ExtMac::InitiateHarq(true)` to transmit the next packet.
       2. If `m_txQueue` is empty: call `Dect2020ExtMac::MonitorTxQueue()` to poll for the next packet.

> **Previously a critical bug (D1 - fixed):** Before this fix, `Dect2020ExtMac::HandleHarqAck()` directly called `InitiateHarq(next)` on ACK reception, immediately re-arming the single HARQ process to `WAITING_ACK` for the next packet. The stale `VerifyHarqAck` still pending for the current packet then fired, found `state == WAITING_ACK`, and entered the retransmit block - either causing a null-pointer dereference (if the next packet's context wasn't built yet) or misattributing to the next packet and launching a duplicate RACH chain. This was **deterministic**, not a race: the response window is intentionally oversized so the ACK always arrives before `VerifyHarqAck` fires. The fix collapsed cycle progression to this single decision point. **Trade-off (accepted):** the next packet now always waits until the response-window boundary even when the ACK arrives early - up to `responseWindow` extra latency per packet.

> **Previously a critical bug (F-DATA-3 - fixed):** `Dect2020ExtMac::ProcessAssociationResponse()` used to call `LaunchApp()` directly on reception of the Association Response. This introduced the same D1 pattern at the association→data handoff boundary: the Association Request's pending `VerifyHarqAck` would fire after `ProcessAssociationResponse()` had already re-armed the process for the first data packet, causing a null-context dereference or misattribution. The fix moved `LaunchApp()` and `StartKeepAlivePeriod()` here.

### `Dect2020ExtMac::HandleHarqAck()`

Called on the PT side by `Dect2020ExtMac::HandleDataPacket()` when a DATA MAC PDU is received (which is always treated as an ACK - see [D6/D7 in the Summary](#summary-of-deviations-from-the-standard)).

The function only does two things:
1. Find the HARQ process by process number and call `process->Reset()` to free it (transition to `FREE` state, clear the packet reference and context).
2. Call `Dect2020ExtMac::RestartKeepAlivePeriod()` to reset the inactivity timer.

The function deliberately does **not** call `InitiateHarq()` or make any decision about the next transmission. That is exclusively the responsibility of `Dect2020ExtMac::VerifyHarqAck()`.

### `Dect2020ExtMac::SendHarqAck()`

Called on the FT side to send the downlink ACK after a data packet is received. Scheduled by `Dect2020ExtMac::HandleDataPacket()` at `currentAbsSubslot + harqFeedbackDelay + 1` (with `DECT_Delay = 0` assumed, same as the Association Response).

1. Checks PHY availability with `m_phy->IsAvailable()`. If the PHY is busy, the ACK is **silently dropped** and a drop counter is incremented. The PT will detect the missing ACK when `VerifyHarqAck()` fires and retransmit.
2. If available, sends the ACK with `m_phy->Send(packet, phyControlFieldType2, random=true)`.

> **Design choice (D8):** The downlink ACK is sent without LBT or backoff - it is a scheduled FT response, not a contention-based transmission. This is consistent with the Association Response behavior and with the standard's model where the FT responds in its own resources.

### `Dect2020ExtMac::HandleDataPacket()`

Entry point for all `DATA_MAC_PDU_HEADER` packets received from the PHY. Validates the packet is addressed to this device via the Unicast Header.

- **PT branch:** Calls `Dect2020ExtMac::HandleHarqAck()`. Any received DATA MAC PDU is treated as a positive ACK regardless of content. There is no NACK or CRC check (see [D6/D7 in the Summary](#summary-of-deviations-from-the-standard)).
- **FT branch:** Looks up the transmitting PT in `Dect2020ExtMac::m_associatedPtDevices` by Long RD ID. If found, builds a DATA MAC PDU ACK and schedules `Dect2020ExtMac::SendHarqAck()` at `currentAbsSubslot + harqFeedbackDelay + 1`. If not found, raises `NS_FATAL_ERROR`.

> **Design choice (D3 - kept intentionally):** The `NS_FATAL_ERROR` on an unregistered PT is an intentional invariant assertion. In the intended scenario, a PT only sends data after association, which registers it in `m_associatedPtDevices`. The assertion is kept as a crash on unexpected behavior, consistent with the NS-3 convention for invariant violations, despite being inconsistent with the graceful-drop policy used elsewhere (beacons, responses).

---

## Data Transmission

### `Dect2020ExtMac::LaunchApp()`

Called from `Dect2020ExtMac::VerifyHarqAck()` after the Association Request is acknowledged. Creates the `Dect2020ExtApp` application object, configures it (packet generation period: 5 ms, packet limit: 50), and calls `Start()`.

`Dect2020ExtApp::Start()` immediately enqueues the first packet by calling `Dect2020ExtMac::EnqueuePacket()` and then re-arms itself every 5 ms. After `Start()` returns, `Dect2020ExtMac::LaunchApp()` asserts that `m_txQueue` is non-empty (fatal error if the queue is empty - this indicates `Start()` failed to enqueue) and then calls `Dect2020ExtMac::InitiateHarq(true)` to transmit the first data packet and kick off the stop-and-wait data cycle.

> **Previously a minor latent bug (M-data - fixed):** The original `LaunchApp()` called `InitiateHarq()` without an empty-queue guard. `InitiateHarq()` calls `m_txQueue.front()` / `pop()` without its own empty check, so an empty queue would cause undefined behavior. The fix adds a `NS_FATAL_ERROR` guard after `Start()` to ensure the queue is never empty when `InitiateHarq()` is called from this path.

### `Dect2020ExtMac::EnqueuePacket()`

Builds a dummy DATA MAC PDU and pushes it onto `Dect2020ExtMac::m_txQueue`. The packet contains:
- A Unicast Header with receiver = `m_selectedFtCandidate.longFtId` and transmitter = `m_longRadioDeviceId`
- A MAC Header Type with `DATA_MAC_PDU_HEADER` and `MAC_SECURITY_NOT_USED`

The sequence number is always 0 and NDI is always `true`. There is no payload beyond the headers. See [D5 in the Summary](#summary-of-deviations-from-the-standard) regarding duplicate detection.

### `Dect2020ExtMac::MonitorTxQueue()`

Called from `Dect2020ExtMac::VerifyHarqAck()` when the TX queue is empty at ACK time. Polls `m_txQueue` every 1 ms:
- If the queue is empty → reschedules itself after 1 ms.
- If the queue is non-empty → logs "Detected a new packet in Tx queue" and calls `Dect2020ExtMac::InitiateHarq(true)`.

The 1 ms polling interval is a configurable design choice rather than an event-driven notification, acceptable for the current simulation scale.

> **Previously a bug (D2 - fixed):** The packet-detected branch originally had `// InitiateHarq()` commented out, so upon detecting a packet the function logged the message and did nothing, causing the data cycle to stall permanently whenever the queue emptied. The fix enables the `InitiateHarq(true)` call in the non-empty branch.

---

## Data Cycle Summary

The complete stop-and-wait data cycle, from association acknowledgement onward:

```
VerifyHarqAck(data=false)        ← Association Request ACK verified
  → LaunchApp()                  ← creates app, enqueues first packet
    → InitiateHarq(true)         ← pops packet, arms HARQ process, schedules RACH
      → InitiateRandomAccess()   ← computes start positions, creates context, CW_CURRENT = CW_MIN
        → InitiateBackoff()      ← draws random position index in [0, CW_CURRENT]
          → PerformLbt()         ← senses channel 5× over STF+GI
            → EvaluateLbtChannel()
              [free/possible]    → m_phy->Send(packet) + schedule VerifyHarqAck(t+responseWindow)
              [busy/RX]          → re-initiate with same context (no CW change)
              [no fit]           → defer to next frame with same context

FT: HandleDataPacket()           ← receives packet, schedules SendHarqAck at +harqDelay+1
  → SendHarqAck()                ← sends ACK (drops if PHY busy)

PT: HandleDataPacket() / HandleHarqAck()
  → process->Reset()             ← frees HARQ process (state FREE)
  → RestartKeepAlivePeriod()

VerifyHarqAck(data=true)         ← fires at t + responseWindow
  [state == FREE, queue non-empty]  → InitiateHarq(true)  ← next packet
  [state == FREE, queue empty]      → MonitorTxQueue()    ← wait for next packet
  [state == WAITING_ACK]            → CW×2, retransmit    ← no ACK received
```

---

## Summary of Deviations from the Standard

| # | Finding | Area | Clause | Description | Classification |
|---|---|---|---|---|---|
| F1 | Fit-check uses response window | 5.3.3 | Fit-check compares response window (not MAC PDU TX duration) against remaining RACH; requires full request+response exchange to fit | Design choice |
| F2 | Response window unenforced | 5.3.4 | `responseWindow = 2 × MaxRachLength + harqDelay + 1` must be configured correctly; no runtime check | Known missing validation - critical config constraint |
| F3 | LBT senses at position k, transmits at k+1 | 5.3.1/5.3.3 | Grid alignment shifted by one `STF+GI`; essential collision/sensing invariants preserved | Known possible deviation - unresolved |
| F4 | Backoff re-drawn on deferral | 5.3.3 | `rachBackOff` re-randomized instead of frozen/resumed across RACH occurrences on position-exceeded deferral | Known possible deviation - unresolved |
| F5 | No RACH failure/abort | 5.3.4 | `dectRachResourceFailure`, `maxRetx`, and `reTxCount` not implemented; device retransmits indefinitely (CW saturates at `CW_MAX`) | Intentional simplification |
| F7 | First-wins PHY collision model | 5.3 | No SINR; first packet received wins; collision yields one success + one retry rather than mutual loss | Modeling choice |
| F8 | NDI always `true` / fixed redundancy version | 5.5 | Retransmissions signal `NDI = true` (new data) and do not advance the redundancy version; inert because FT does no soft combining | Intentional simplification |
| M1 | `CW_CURRENT` reset on success | 5.3.4 | Not explicitly reset to `CW_MIN` on success, but the entire context is discarded (`process->Reset()`) and a new context initializes `CW_CURRENT = CW_MIN` for the next packet | Compliant by design |
| M2 | Fixed MCS | 5.3.3 | Standard requires "highest MCS that maximizes throughput"; implementation uses fixed `m_mcs = 0` | Design choice |
| D1 | Stale `VerifyHarqAck` on reused process | 5.3.4/5.5 | Pending verifier for packet A would fire against packet B after ACK for A re-armed the single HARQ process; deterministic null-context crash or duplicate TX | **Fixed** - `VerifyHarqAck` is now the sole serialization point; `HandleHarqAck` only resets the process |
| D2 | `MonitorTxQueue` stall | - | Packet-detected branch had `InitiateHarq` commented out; data cycle stalled permanently when queue emptied | **Fixed** |
| D3 | FT fatal error on data from unregistered PT | - | `NS_FATAL_ERROR` if the transmitting PT is not in `m_associatedPtDevices`; intentional invariant assertion | Kept intentionally - runtime crash risk |
| D4 | Response subslot frame overflow | - | `currentAbsSubslot + harqDelay + 1` can overflow the frame → `std::out_of_range` crash; indirectly prevented by the fit-check and frame-boundary config constraint | Known runtime crash risk - to be fixed |
| D5 | No duplicate detection | 5.5/5.6 | All packets built with `SequenceNumber = 0` and `NDI = true`; FT cannot detect retransmissions; delivered counts over-count on ACK loss | Design choice |
| D6/D7 | No NACK / no CRC; any DATA PDU = ACK | 5.3.4/5.5 | `HandleHarqAck` resets the process on reception without inspecting feedback or CRC; no downlink-data path; NACK and "CRC fail → retransmit" paths not implemented | Design choice |
| D8 | ACK without LBT | 5.3.4 | Downlink ACK sent as a scheduled response with only an availability check, no backoff or LBT | Design choice - correct and consistent |
| F-DATA-1 | Dead `data`/`random` fields in context | - | `Dect2020ExtRandomAccessContext::data` and `::random` were never set or read (real flag travels via `process->data`) | **Fixed** - fields removed |
| F-DATA-2 | PHY-carried `data` parameter | - | `data` argument to `m_phy->Send()` was unused on the RX path; receiver routes by MAC header type; only `random` is needed | **Fixed** - `data` removed from `m_phy->Send()`, signal parameters, `EndRx`, and `ReceiveFromPhy()` |
| F-DATA-3 | Association→data handoff race | - | `ProcessAssociationResponse()` called `LaunchApp()` on ACK reception, colliding with the pending association `VerifyHarqAck`; same class as D1 | **Fixed** - `LaunchApp()` and `StartKeepAlivePeriod()` moved to `VerifyHarqAck()` |
| M-data | Unguarded `InitiateHarq` in `LaunchApp` | - | `InitiateHarq` called `m_txQueue.front()` without an empty-queue guard; `packetLimit = 0` would cause undefined behavior | **Fixed** - `NS_FATAL_ERROR` guard added after `Start()` |
