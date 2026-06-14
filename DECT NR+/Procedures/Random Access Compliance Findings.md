# Random Access Procedure — DECT-2020 NR Compliance Findings

**Reference standard:** ETSI TS 103 636-4 V2.1.1 (2024-10), clause 5.3 (Random Access procedure).
**Module under review:** `contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc` (+ `dect2020ext-mac.h`, `model/phy/dect2020ext-phy.cc`, `model/mac/dect2020ext-mac-scheduler.cc`).
**Deliverable:** findings only — no code was changed.

## Scope and assumptions

- Random access is used **only for the association procedure** (PT → FT Association Request, FT → PT Association Response acting as the random-access response per §5.3.4).
- Scenarios in scope: **single PT / single FT** *and* **multiple PTs contending against one FT** (each PT selects exactly one FT candidate). Collisions are therefore expected and the contention/backoff/LBT logic must be correct.
- Reviewed path: `SendAssociationRequest` → `InitiateHarq` → `InitiateRandomAccess` → `InitiateBackoff` → `PerformLbt` → `SampleLbtRssi` → `EvaluateLbtChannel` → `m_phy->Send` → `VerifyHarqAck`; FT side `ReceiveFromPhy` → `HandleUnicastPacket` → `ProcessAssociationRequest` → `SendAssociationResponse`; PT side `ReceiveFromPhy` → `HandleUnicastPacket` → `ProcessAssociationResponse`.
- Relevant constants (subcarrier scaling factor 1): `STF_US = 64.815`, `GI_US = 18.52`, `MINIMUM_LBT_PERIOD = START_POSITION_INTERVAL = STF + GI = 83.335 µs`; `RSSI_THRESHOLD_MIN = -85 dBm`, `RSSI_THRESHOLD_MAX = -50 dBm`; `CW_MIN = 8 · CwMinSig`, `CW_MAX = 256 · CwMaxSig`; one HARQ process (`HARQ_MAX_PROCESSES = 1`, stop-and-wait).

## Verdict summary

The MAC-level random-access state machine is **substantially compliant** with §5.3 for the association use case: parameters are taken from the Random Access Resource IE; start positions are computed at integer multiples of `STF+GI`; backoff is uniform in `[0, CW_CURRENT]`; LBT senses for `MINIMUM_LBT_PERIOD` before transmitting; the "free/possible/busy" decision uses the §5.1.2 thresholds; on **busy** the backoff is re-initiated (no CW change, per §5.3.3); on a **missing response** `CW_CURRENT` is doubled with a `CW_MAX` cap and a new procedure is initiated (per §5.3.4); on **success** the context is discarded. The findings below are the deviations, design choices, and edge cases identified during the review.

| #   | Finding                                                                                            | Classification                              | Severity | Status |
| --- | -------------------------------------------------------------------------------------------------- | ------------------------------------------- | -------- | ------ |
| F1  | "Fits in remaining RACH" gate checks the response window, not the MAC PDU TX duration              | Not standard compliant → design choice      | —        |        |
| F2  | Response-window size is a sound but unenforced configuration assumption                            | TODO (add runtime check)                    | Medium   |        |
| F3  | LBT senses the interval *at* start position `k`, transmits at `k+1`                                | Possible deviation from the standard ???    | Low      |        |
| F4  | Backoff is re-drawn each RACH occurrence instead of one persistent `rachBackOff`                   | Possible deviation (unresolved — to verify) | Medium   |        |
| F5  | No RACH failure/abort (`dectRachResourceFailure`, `maxRetx`) → infinite retransmission             | Intentional simplification (known)          | —        |        |
| F6  | Inconsistent `m_phy->Send` guards; beacons ignore `RX`; `NS_FATAL_ERROR` on `TX` can crash the sim | Bug + TODO                                  | Medium   | Done   |
| F7  | PHY collision model is first-wins capture with no SINR                                             | Modeling-choice                             | —        |        |
| F8  | `NDI=true` / fixed redundancy version reused on retransmissions                                    | Intentional simplification                  | —        |        |
| M1  | `CW_CURRENT` not explicitly reset to `CW_MIN` on success                                           | Compliant-by-design                         | —        |        |
| M2  | Fixed `m_mcs = 0` instead of "highest MCS"                                                         | Design choice                               | —        |        |
| M4  | `StartRx` dereferences `dectParams` before its null check                                          | TODO fix                                    | Low      | Done   |

---

## Detailed findings

### F1 — "Fits in remaining RACH resources" checks the response window, not the MAC PDU TX duration

**Standard (§5.3.3):** *"if the prepared MAC PDU fits to the remaining random access resources: initiate random access transmission; else: defer to the next occurrence of random access resources."* The quantity that must fit is the **MAC PDU transmission duration**.

**Implementation:** `CheckIfRachTxFitsInRemainingResources` (≈`dect2020ext-mac.cc:1468`) compares the **response window** against the remaining RACH time:

```cpp
uint32_t responseWindow = randomAccessContext->randomAccessResourceIe.GetResponseWindow();
double remainingRachTimeNs = m_macScheduler->GetTimeTillSubslotStartNs(startSubslot + length);
double requiredTimeNs = responseWindow * subslotDurationNs;
return remainingRachTimeNs >= requiredTimeNs;
```

The MAC PDU TX duration is checked only against the *Maximum Random Access TX time* parameter (`CheckIfMacPduFitsInMaxRandomAccessTxTime`), never against the time remaining in the window.

**Assessment:** This is a deliberate model where the **entire request + response exchange must be contained inside the RACH window**, which the standard does not require (the FT response is normally received after the RACH window). It is internally self-consistent: `VerifyHarqAck` is scheduled `responseWindow · subslotDuration` from the moment `Send` is called (i.e., from TX start), so the response-window clock already subsumes the request TX duration, and gating on the response window keeps the whole exchange inside the window.

**Classification: Not standard compliant → design choice.** No fix required; documented intentional divergence.

---

### F2 — Response-window sizing is a sound but unenforced configuration assumption

**Standard (§5.3.4):** the response window is the time within which a random-access response can be expected; absence of a response is treated as a collision.

**Implementation:** the collision/no-collision decision is purely a function of `process->state` when `VerifyHarqAck` fires (`dect2020ext-mac.cc:1096`). The FT sends its Association Response only at `currentAbsSubslot + harqDelay + 1` subslots after it finishes receiving the request (`ProcessAssociationRequest`, ≈`:1990`). The response is therefore processed by the PT at roughly:

```
T_send + D_request_tx + (harqDelay + 1)·subslot + D_response_tx
```

while `VerifyHarqAck` fires at `T_send + responseWindow·subslot`.

**Configured value (per author):** `responseWindow = 2 × RACH_MAX_Tx + 3 (harqDelay) + 1 (propagation)` subslots — budgeting one `RACH_MAX_Tx` for the request TX and one for the response TX (devices cannot exceed `RACH_MAX_Tx`), plus the HARQ delay and a propagation margin. This correctly covers the exchange measured from TX start.

**Risk if violated:** if the configured response window is ever smaller than the real exchange, then **even a single PT with no contention** sees `state == WAITING_ACK` at verify time, declares a non-existent collision, doubles `CW_CURRENT`, and retransmits. The genuine Association Response then arrives *after* the second RA has launched; because `m_associationStatus` is still `ASSOCIATION_PENDING`, `ProcessAssociationResponse` accepts it and resets the process, while the already-scheduled second RA still transmits a **duplicate Association Request after the device is associated** and arms another `VerifyHarqAck`.

**Classification: TODO.** *Implement a checking mechanism to always verify that the configured response window is big enough* (i.e., `responseWindow ≥ ⌈(D_request_tx + D_response_tx)/subslot⌉ + harqDelay + 1`), failing fast at configuration time rather than silently mis-detecting collisions at runtime.

---

### F3 — LBT senses the interval *at* start position `k` and transmits at `k+1`

**Standard (§5.3.1 / §5.3.3):** start positions are at `0, 1, 2, … × (STF+GI)` from the RACH start; the channel must be sensed for `MINIMUM_LBT_PERIOD = STF+GI` **immediately before** the start position, and transmission begins **at** the start position.

**Implementation:** for backoff value `k`, `InitiateBackoff` schedules `PerformLbt` at `k · (STF+GI)` (the start position itself); `PerformLbt`/`SampleLbtRssi` then sample over `[k·I, k·I + (STF+GI)]` (offsets `0, STF/2, STF, STF + GI/2, STF + GI`, ≈`:1323`), and `EvaluateLbtChannel` transmits at the **end** of that window — effectively at position `k+1`.

**Assessment:** the essential invariants hold — the device still senses a full `MINIMUM_LBT_PERIOD` immediately before transmitting, and two PTs still collide iff they pick the same backoff `k` (both shifted by one interval). Only the absolute alignment to the nominal start-position grid is shifted by one `(STF+GI)`.

**Classification: Possible deviation from the standard ???** Author intends to sense before the rachBackOff timer; the "sense at `k`, transmit at `k+1`" scheme is accepted but flagged for re-confirmation.

---

### F4 — Backoff is re-drawn each RACH occurrence instead of one persistent `rachBackOff`

**Standard (§5.3.3):** *"keep rachBackOff running during random access resources."* A single random value `∈ [0, CW_CURRENT]` (in units of `STF+GI`) counts down **only while RACH resources are active**, freezing between occurrences and resuming in the next one, until it expires.

**Implementation:** `InitiateBackoff` (≈`:1262`) draws a **fresh** `m_backoffRandomVariable->GetInteger(0, CW_CURRENT)` every time it runs. When the drawn value does not fit in the current window (`backoffValue >= startPositions.size()`), it defers to the next frame by re-entering `InitiateRandomAccess(newContext=false)` → `InitiateBackoff`, which draws **another** fresh value rather than carrying over the residual count.

> Note: on a **busy** LBT outcome, re-drawing *is* standard-correct (§5.3.3: "initiate the backoff process as defined above"). The deviation is specifically the *defer-because-it-did-not-fit* path re-randomizing instead of resuming.

**Effect:** under contention the per-attempt backoff distribution differs from the standard's single persistent countdown, changing collision statistics.

**Classification: Possible deviation (unresolved).** Author to verify later whether the residual `rachBackOff` should be frozen/resumed across frames.

---

### F5 — No RACH failure/abort: infinite retransmission

**Standard (§5.3.4):** on a missing response, double `CW_CURRENT` (capped at `CW_MAX`) **and** start `dectRachResourceFailure`; on its expiry, stop RACH, release the association, and re-select an FT (§5.1.4).

**Implementation:** the no-response branch of `VerifyHarqAck` doubles CW (`std::min(CW_CURRENT * 2, CW_MAX)`, correct) and unconditionally re-initiates random access, with **no termination condition**:
- `dectRachResourceFailure` is not implemented.
- `Dect2020ExtHarqProcess::reTxCount` / `maxRetx` exist but are **never read or incremented** in the RA path (dead fields).
- `VerifyPendingAssociationStatus()` (would drop to `NOT_ASSOCIATED`) is defined but **never scheduled**.

Consequently, if the selected FT never produces a processable response (offline, repeatedly dropped responses, or persistent collisions), the PT loops forever (CW saturates at `CW_MAX`, so it stabilizes but never gives up).

**Classification: Intentional simplification (known limitation).** No fix expected; documented. (If revisited: wire `maxRetx` as a retransmission cap, or add a `dectRachResourceFailure`-style timeout that returns to `NOT_ASSOCIATED`/re-selection.)

---

### F6 — Inconsistent `m_phy->Send` guards; beacons ignore `RX`; fatal-error on `TX` can crash the sim

**Observed guards across all `m_phy->Send` sites:**
- `EvaluateLbtChannel` (RA TX, ≈`:1363`): `NS_FATAL_ERROR` if PHY `== TX`; treats `== RX` as busy (re-initiates); transmits if IDLE.
- `SendNetworkBeaconOnChannel` / `SendClusterBeaconOnChannel` (≈`:519` / `:537`): `NS_FATAL_ERROR` if PHY `== TX`; **no `RX` check**.
- `SendAssociationResponse` (≈`:2016`): graceful **drop** (`return`) if `!IsAvailable()` (not IDLE).

**Problem 1 — reachable simulation crash on the FT.** The FT transmits an Association Response of duration `D_resp`. Beacon transmissions are scheduled at fixed subslots each beacon period. If a beacon event fires while the response TX is in progress (PHY `== TX`) — or vice versa — it hits `NS_FATAL_ERROR` and aborts the whole simulation instead of dropping/deferring like `SendAssociationResponse`. The response time is `harqDelay+1` subslots after a request that lands in RACH and may wrap into the next frame (`% subslotsPerFrame` in `HandleDataPacket`), so overlap with a beacon subslot is reachable. (The RA-TX `== TX` fatal in `EvaluateLbtChannel` is effectively unreachable for an association-only PT, which only transmits during RA and never overlaps its own TX — low severity there.)

**Problem 2 — beacons do not check `RX` (half-duplex violation).** If the FT is mid-reception of an Association Request and a beacon fires, the beacon `Send` proceeds and overwrites the PHY state to `TX`, yet the in-progress `EndRx` still delivers that request to the MAC — the FT both "transmits" and "successfully receives" at once, which can produce spurious associations. This is especially relevant when multiple FTs beacon on the same channel at the same time.

**Classification: Bug.**
- **Fix (beacon):** check the PHY state before sending a beacon; if the PHY is `RX` (e.g., another FT transmitting beacons on the same channel at the same time), **drop the packet and log with `NS_LOG_INFO`** rather than clobbering the reception.
- **TODO (general):** implement a single availability check used by **every** function that calls `m_phy->Send` — all senders must first verify the PHY is available and then send, gracefully dropping/deferring (and logging) instead of using `NS_FATAL_ERROR`.

---

### F7 — PHY collision model is first-wins capture with no SINR

**Implementation:** `Dect2020ExtPhy::StartRx` (≈`dect2020ext-phy.cc:264`) accumulates interference via `AddSignal`, then drops an incoming packet only if the PHY is not IDLE ("Dropping packet due to collision") and otherwise decodes it successfully. There is no SINR test against the accumulated interference.

**Effect on RACH contention:** when two PTs transmit at the same start position, both `StartRx` calls occur at the same instant; whichever the scheduler processes first puts the FT into `RX` and is decoded successfully, and the second is dropped. A genuine 2-PT collision thus yields **one successful association + one timeout/retry** rather than mutual loss, so the RACH collision rate is optimistic. The MAC-level recovery (`VerifyHarqAck` → CW doubling → retry) is itself correct per §5.3.4 and fires for the losing PT.

**Classification: Modeling-choice.** Documented fidelity note; the §5.3 MAC procedure is unaffected.

---

### F8 — `NDI=true` / fixed redundancy version reused on retransmissions

**Implementation:** `InitiateHarq` (≈`:931`) builds the PHY Control Field Type 2 once with `ndi = true` and a fixed redundancy version; `VerifyHarqAck` re-initiates with that same field, so every retransmitted Association Request is signaled as `NDI = new data` with an unchanging redundancy version. Per HARQ (§5.5), a retransmission should indicate `NDI = 0` and typically advance the redundancy version for soft combining.

**Why inert here:** the FT performs no soft combining — it treats each received Association Request independently (re-creates/updates `PtInfo` and re-responds) — so association still functions correctly.

**Classification: Intentional simplification.**

---

## Minor items

- **M1 — `CW_CURRENT` reset on success.** §5.3.4 says to set `CW_CURRENT = CW_MIN` on a successful response. The code does not do this explicitly, but `ProcessAssociationResponse` calls `process->Reset()`, discarding the whole `Dect2020ExtRandomAccessContext`, so it is moot for association-only. Additionally, **`CW_CURRENT` is always reset to `CW_MIN` for each new packet taken from the Tx queue** (not for retransmissions): each new packet runs through `InitiateRandomAccess(newContext=true)`, which initializes `CW_CURRENT = CW_MIN`. **Classification: compliant-by-design.**
- **M2 — Fixed MCS.** §5.3.3 says to "select the highest MCS … that maximizes throughput"; the implementation uses a fixed `m_mcs = 0`. **Classification: design choice.**
- **M4 — `StartRx` null-deref order.** `Dect2020ExtPhy::StartRx` dereferences `dectParams` (≈`dect2020ext-phy.cc:270`, `dectParams->m_currentChannelNumber`) **before** the `if (!dectParams)` null check (≈`:283`). Unreachable with DECT-only signals, but a latent null-dereference if a non-DECT signal (e.g., a `WaveformGenerator`) is ever introduced. **Classification: TODO fix** — move the null check above the first dereference.

---

## Decision log

- **Deliverable = findings report, no code changes** — author wants to classify each item before any edit; "some parts of the standard are not implemented on purpose."
- **Evaluated under multi-PT contention** (and single-PT) — that is the only regime where LBT/backoff/CW/collision logic is meaningfully exercised; PT selects exactly one FT candidate.
- **F1 marked design choice, not bug** — author deliberately requires the whole request+response exchange to fit in the RACH window; it is internally consistent because the response window is anchored at TX start.
- **F2 marked TODO (runtime check)** — the response-window sizing formula is sound, but it is an unenforced config-time assumption whose violation causes false collisions and duplicate post-association requests.
- **F3 accepted but flagged "possible deviation ???"** — sense-before-TX and equal-backoff-collision semantics are preserved; only the start-position alignment is shifted by one `STF+GI`.
- **F4 left unresolved** — author unsure whether `rachBackOff` should persist/resume across RACH occurrences vs. re-randomize; to be verified later.
- **F5 accepted as intentional simplification** — no failure timer / retransmission cap; infinite retransmission is a known, accepted limitation.
- **F6 confirmed as a real bug** — beacons must check PHY state (drop + `NS_LOG_INFO` on `RX`), and a centralized "check availability before `m_phy->Send`" mechanism should replace the `NS_FATAL_ERROR` guards for all senders.
- **F7 accepted as modeling-choice** — first-wins/no-SINR PHY capture; RACH collision rate is optimistic but the MAC recovery matches §5.3.4.
- **F8 accepted as intentional simplification** — static `NDI`/redundancy version is inert because the FT does no soft combining.
- **M-items:** M1 compliant-by-design (new packet ⇒ `CW_MIN`), M2 design choice, M4 TODO fix; the no-op `SendHarqAck` was explicitly excluded from this report at the author's request.
