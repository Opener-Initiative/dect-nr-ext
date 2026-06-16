# Association Procedure

> All standard clause references in this document refer to **ETSI TS 103 636-4 V2.1.1** (DECT-2020 NR Part 4: MAC layer).

## Overview

The Association Procedure establishes a unicast communication link between an RD in PT mode and an RD in FT mode. The procedure is initiated by the PT after it has selected an FT candidate through the Spectrum Management Procedure (clause 5.1.4), and completes when both sides have exchanged Association Request and Association Response messages.

**FT mode** (coordinator):
1. Broadcasts beacon messages to enable PT devices to detect it (covered in the Spectrum Management Procedure).
2. Receives and processes Association Requests from PT devices.
3. Sends Association Responses to accepted PT devices.
4. Maintains the association with each associated PT via the Keep Alive mechanism.

**PT mode** (client):
1. Scans for FT candidates and selects the best one (covered in the Spectrum Management Procedure).
2. Sends an Association Request to the selected FT via the Random Access Procedure (covered in the Random Access Procedure document).
3. Receives and processes the Association Response.
4. Maintains the association via the Keep Alive mechanism.

The Random Access Procedure and HARQ operation used during the association handshake are covered in a separate document.

> **Design choice:** A PT device can be associated to exactly **one FT** at a time. On the FT side, `Dect2020ExtMac::m_associatedPtDevices` is a vector that grows with each accepted PT, so the FT supports **multiple simultaneous PT associations** as the standard requires (clause 5.8.1). The PT-to-single-FT restriction reflects the target simulation topology where each PT device connects to exactly one coordinator.

> **Design choice - out of scope:** MAC security is entirely unimplemented. All beacon messages, Association Requests, and Association Responses are always transmitted as plain text with `MAC_SECURITY_NOT_USED`. No ciphering, integrity protection, MIC, or key exchange exists anywhere in the codebase.

---

## Simulation Control Knobs

Two boolean flags gate the association procedure at key points, both defaulting to `true`:

| Flag | Default | Effect when `false` |
|---|---|---|
| `Dect2020ExtMac::m_proceedToAssociationRequest` | `true` | PT silently ignores all received beacons; the procedure stops after background scan |
| `Dect2020ExtMac::m_proceedToAssociationResponse` | `true` | FT silently ignores all received Association Requests; no response is sent |

These flags are useful for debugging and for constructing custom simulation scenarios where only a subset of the procedure needs to execute (e.g. measuring beacon reception without triggering association).

---

## Incoming Packet Dispatch

### `Dect2020ExtMac::ReceiveFromPhy()`

The central entry point for all packets delivered by the PHY layer. Strips the PHY Control Field and MAC Header Type, then dispatches on the `MacHeaderTypeField`:

- **`BEACON_HEADER`** (PT only, gated by `m_proceedToAssociationRequest`): Calls `Dect2020ExtMac::FindOrCreateFtCandidate()` to update the FT candidate list with the sender's Short FT ID, RSSI, and PHY Control Field, then forwards to `Dect2020ExtMac::HandleBeaconPacket()` (covered in the Spectrum Management Procedure).
- **`UNICAST_HEADER`**: Forwards to `Dect2020ExtMac::HandleUnicastPacket()` for association handshake processing.
- **`DATA_MAC_PDU_HEADER`**: Forwards to `Dect2020ExtMac::HandleDataPacket()` for data processing (covered in the Random Access / Data Transmission document).

After dispatching, the stripped packet is forwarded to the net device via `m_device->Receive()`.

### `Dect2020ExtMac::HandleUnicastPacket()`

Strips the Unicast Header and validates the packet is addressed to this device via `Dect2020ExtMac::PacketIsForThisDevice()`. Silently discards the packet if it is not. Then dispatches on the IE type from the MAC Multiplexing Header:

- **`ASSOCIATION_REQUEST_MESSAGE`** (FT only, gated by `m_proceedToAssociationResponse`): Parses the Association Request Message IE, RD Capability IE, and Association Control IE; calls `Dect2020ExtMac::FindOrCreateAssociatedPtInfo()` to record the PT, then calls `Dect2020ExtMac::ProcessAssociationRequest()`.
- **`ASSOCIATION_RESPONSE_MESSAGE`** (PT only): Validates that the sender's Short ID matches the previously selected FT (fatal error if not); parses the Association Response Message IE and RD Capability IE; updates `Dect2020ExtFtInfo`; calls `Dect2020ExtMac::ProcessAssociationResponse()`.

---

## FT Candidate and PT Info Management

### `Dect2020ExtMac::FindOrCreateFtCandidate()`

Searches `Dect2020ExtMac::m_ftCandidates` for an entry matching the given Short FT ID. If found, returns a pointer to the existing entry so its fields can be updated. If not found, appends a new empty `Dect2020ExtFtInfo` and returns a pointer to it. This function is called every time a beacon is received, so FT candidate state is continuously refreshed throughout the discovery window.

### `Dect2020ExtMac::FindFtCandidate()`

Searches `Dect2020ExtMac::m_ftCandidates` by Short FT ID. If no match is found, calls `NS_FATAL_ERROR`. This function is called in contexts where the FT is guaranteed to exist (e.g. when processing an Association Response from the already-selected FT).

### `Dect2020ExtMac::FindOrCreateAssociatedPtInfo()`

Searches `Dect2020ExtMac::m_associatedPtDevices` for an entry matching the given Short PT ID. Creates a new `Dect2020ExtPtInfo` entry if not found. Called on the FT when an Association Request arrives to store or refresh the associating PT's state.

---

## Association Request (PT Side)

### `Dect2020ExtMac::SendAssociationRequest()`

Called from `Dect2020ExtMac::EvaluateClusterBeacon()` when the selected FT's next Cluster Beacon is received. Transitions the association status to `ASSOCIATION_PENDING`, pushes the built Association Request packet onto the TX queue, and calls `Dect2020ExtMac::InitiateHarq(false)` to hand it off to the Random Access Procedure for transmission. The remainder of the handshake (RACH window, HARQ ACK verification, and `Dect2020ExtMac::MonitorTxQueue()` startup) is handled by the HARQ/Random Access procedure.

### `Dect2020ExtMac::BuildAssociationRequest()`

Builds the Association Request MAC PDU. The PDU is structured as follows (outer to inner):

| Layer | Content |
|---|---|
| MAC Header Type | `UNICAST_HEADER`, `MAC_SECURITY_NOT_USED` |
| Unicast Header | Receiver = FT's Long RD ID, Transmitter = PT's Long RD ID |
| MAC Multiplexing Header | IE type = `ASSOCIATION_REQUEST_MESSAGE` |
| Association Request Message IE | Setup cause = 0 (initial association), number of flows = 0 |
| RD Capability IE | Populated from the PT's `Dect2020ExtNetDevice` configuration fields |
| Association Control IE | Configured externally via `Dect2020ExtMac::ConfigureAssociationControlIe()`; includes `CB_M`, `ULPeriod`, and `DLdataReception` |

> **Design choice:** `CB_M` should always be set to 0 in the `AssociationControlIe` configuration (requiring the PT to maintain Cluster Beacon monitoring after association). While the `CB_M` value is correctly serialized and transmitted in the PDU, it is never acted upon - the model always continues to receive and process beacon messages regardless of the `CB_M` value.

---

## Association Response (FT Side)

### `Dect2020ExtMac::ProcessAssociationRequest()`

Called after `Dect2020ExtMac::HandleUnicastPacket()` has parsed and stored the Association Request. Always accepts the request without any policy check. Calculates the response subslot as:

```
responseAbsSubslot = currentAbsSubslot + harqFeedbackDelay + 1
```

where `harqFeedbackDelay` is read from the PT's `Dect2020ExtRdCapabilityIe`. The formula assumes `DECT_Delay = 0` (hardcoded). Schedules `Dect2020ExtMac::SendAssociationResponse()` at the computed subslot offset.

> **Known missing feature:** The standard (clause 5.8.5) requires the FT to evaluate whether it can accept the association before responding. This evaluation is not implemented - the FT accepts every Association Request unconditionally. There is no rejection path, no `rejectTime` timer, and the `TODO` comment in the source marks this for future implementation.

### `Dect2020ExtMac::SendAssociationResponse()`

Checks PHY availability before transmitting. If the PHY is busy, the response is **silently dropped** and the dropped-response counter is incremented. The PT will detect the missing HARQ ACK and retransmit the Association Request, at which point the FT will update the PT info and send a new response.

If the PHY is available, builds the response via `Dect2020ExtMac::BuildAssociationResponse()` and sends it with PHY Control Field Type 2, setting the ACK flag and echoing the PT's HARQ process number.

### `Dect2020ExtMac::BuildAssociationResponse()`

Builds the Association Response MAC PDU. The PDU is always built with `SetAssociationAccepted(true)`. Structure:

| Layer | Content |
|---|---|
| MAC Header Type | `UNICAST_HEADER`, `MAC_SECURITY_NOT_USED` |
| Unicast Header | Receiver = PT's Long RD ID, Transmitter = FT's Long RD ID |
| MAC Multiplexing Header | IE type = `ASSOCIATION_RESPONSE_MESSAGE` |
| Association Response Message IE | `AssociationAccepted = true`, `HarqMod = false`, number of flows = 0 |
| RD Capability IE | Populated from the FT's `Dect2020ExtNetDevice` configuration fields |

---

## Association Completion (PT Side)

### `Dect2020ExtMac::ProcessAssociationResponse()`

Called by `Dect2020ExtMac::HandleUnicastPacket()` when the PT receives the Association Response. Performs a sanity check asserting that the association status is `ASSOCIATION_PENDING` (fatal error otherwise). If the response is accepted (always true in the current implementation):

1. Finds the corresponding HARQ process by number and resets it.
2. Transitions the association status to `ASSOCIATED` via `Dect2020ExtMac::ChangeAssociationStatus()`.
3. Stores the FT's Long RD ID in `Dect2020ExtMac::m_associatedFtNetDeviceLongRdId`.

> **Known missing feature:** If `GetAssociationAccepted()` returns `false`, a `NS_FATAL_ERROR` is raised. There is no graceful handling of a rejected association response (re-initiating RD selection, observing the `rejectTime` timer, etc.) since the FT never rejects in the current implementation.

After `ProcessAssociationResponse()` completes, control returns to `Dect2020ExtMac::VerifyHarqAck()` (the RACH response window callback), which starts `Dect2020ExtMac::MonitorTxQueue()` and `Dect2020ExtMac::StartKeepAlivePeriod()` to begin the data transmission and maintenance phases.

### `Dect2020ExtMac::ChangeAssociationStatus()`

Helper that updates `Dect2020ExtMac::m_associationStatus` and logs the transition. Used at every state transition in the association state machine:

```
NOT_ASSOCIATED → ASSOCIATION_PREPARING → WAITING_FOR_SELECTED_FT → ASSOCIATION_PENDING → ASSOCIATED
```

---

## Association Maintenance

Corresponds to standard clause **5.8.7**. The maintenance timer infrastructure is partially implemented.

### `Dect2020ExtMac::StartKeepAlivePeriod()`

Records `m_lastRxTime = Simulator::Now()` and schedules `Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded()` after the `UL_PERIOD` decoded from `m_configuredAssociationControlIe`. Called once from `Dect2020ExtMac::VerifyHarqAck()` after the association is confirmed.

### `Dect2020ExtMac::RestartKeepAlivePeriod()`

Updates `Dect2020ExtMac::m_lastRxTime` to `Simulator::Now()`. Called every time a HARQ ACK is received (i.e. on each successful uplink/downlink exchange), effectively restarting the inactivity window.

### `Dect2020ExtMac::VerifyKeepAlivePeriodNotExceeded()`

Fires on each timer tick and compares `Simulator::Now() - m_lastRxTime` against `UL_PERIOD`:

- If the period **has been exceeded**: logs `"Keep Alive Period exceeded"` and reschedules itself for another `UL_PERIOD`. No Keep Alive IE is sent and the association is not torn down.
- If the period **has not been exceeded**: computes the remaining time and reschedules itself for exactly that duration.

> **Known missing feature - partially implemented:** The timer logic correctly tracks inactivity and fires at the right time, but the action on expiry is incomplete. The standard (clause 5.8.7) requires the PT to send a Keep Alive IE (or a new Association Request if parameter modification is desired) when the timer expires. This is not implemented due to time constraints, as fully implementing it would introduce coupling with the Random Access and data transmission procedures. The infrastructure is left in place for future completion.

---

## Association Release

Corresponds to standard clause **5.8.6**.

Association Release is **entirely unimplemented**. There is no `SendAssociationRelease()` function, no Association Release message type is built or parsed, and the commented-out `VerifyAssociatedStatus()` function (which would have detected association loss from beacon timeout) is not active. Once established, an association persists for the remainder of the simulation. This is out of scope for the current simulation goals, where the purpose is to measure steady-state network behavior rather than model connection teardown.

---

## Summary of Deviations from the Standard

| # | Area | Clause | Description | Classification |
|---|---|---|---|---|
| 1 | Association rejection | 5.8.5 | FT always accepts every request; no policy check, no `rejectTime` timer | Known missing feature |
| 2 | PT rejection handling | 5.8.5 | PT raises fatal error if response is rejected; no re-initiation of RD selection | Known missing feature |
| 3 | Association Release | 5.8.6 | Entirely unimplemented; association persists for the full simulation | Out of scope |
| 4 | Keep Alive IE | 5.8.7 | Timer fires correctly but no Keep Alive IE is sent on expiry; association never torn down | Known missing feature - partial implementation |
| 5 | MAC security | 5.8.4/5.8.5 | All messages sent as plain text; no ciphering or integrity protection | Out of scope |
| 6 | `CB_M` bit handling | 5.8.4/5.8.5 | `CB_M` is transmitted but never acted upon; beacon reception always maintained | Design choice |
| 7 | PT single-FT association | 5.8.1 | PT can only associate with one FT; standard allows multiple simultaneous associations | Design choice |
| 8 | `DECT_Delay` | 5.8.5 | Response timing always uses `DECT_Delay = 0`; not configurable | Design choice |
| 9 | Short RD ID collision detection | 5.8.3 | Not checked during association initiation | Known missing feature |
| 10 | Response drop on PHY collision | 5.8.5 | Association Response silently dropped if PHY busy; PT retransmits via HARQ | Design choice |
