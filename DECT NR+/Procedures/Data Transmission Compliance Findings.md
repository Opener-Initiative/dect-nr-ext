# Data Transmission over Random Access — DECT-2020 NR Compliance Findings

**Reference standard:** ETSI TS 103 636-4 V2.1.1 (2024-10), clause 5.3 (Random Access procedure) + §5.5 (HARQ).
**Companion document:** `Random Access Compliance Findings.md` (association procedure). The data uplink reuses the **identical** random-access code path, so the association findings carry over (see *Inherited findings*).
**Module under review:** `contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc` (+ `model/application/dect2020ext-app.cc`, `model/phy/dect2020ext-phy.cc`, `model/mac/dect2020ext-mac-scheduler.cc`).
**Deliverable:** findings only — no code was changed.

## Scope and assumptions

- After association completes, the PT runs **data transmission** using the **same** random-access procedure as the Association Request/Response, but with dummy DATA_MAC_PDU packets and an explicit HARQ-feedback **ACK** (DATA_MAC_PDU with feedback set) sent by the FT. The model is: *"data transmission is the same as sending Association Requests and waiting for responses; on ACK, pop the next packet and repeat."*
- **Stop-and-wait ARQ**: a single HARQ process (`HARQ_MAX_PROCESSES = 1`); the device transmits the next packet only after the current one is acknowledged.
- Both regimes are valid and in scope: **Tx queue non-empty at ACK time** (steady-state burst) and **Tx queue empty at ACK time** (configurable; used to measure queue→TX→ACK delay).
- Frame = **10 ms** / 24 slots; app default generation period **5 ms**, packet limit **50**.
- **Out of scope:** Keep-Alive period; `m_device->Receive` up-stack forwarding (explicitly irrelevant per author).

### Data cycle (as implemented)

1. `ProcessAssociationResponse` → `LaunchApp` (`dect2020ext-mac.cc:2385`).
2. `LaunchApp`: creates `Dect2020ExtApp` (5 ms / 50), `Start()` → `GeneratePacket()` enqueues one dummy DATA_MAC_PDU (`EnqueuePacket`, `:2360`) and re-arms every 5 ms; then `InitiateHarq(data=true)` (`:951`) pops it and runs the RACH path (`InitiateRandomAccess → … → EvaluateLbtChannel → m_phy->Send(data=true) → VerifyHarqAck`).
3. FT `HandleDataPacket` (`:1733`, FT branch) finds the associated PT, builds a DATA_MAC_PDU ACK, schedules `SendHarqAck` at `harqDelay+1` subslots; `SendHarqAck` (`:1070`) transmits if the PHY is available, else drops + logs.
4. PT receives the ACK → `HandleDataPacket` (PT branch) → `HandleHarqAck` (`:1088`) → `process->Reset()`, then `InitiateHarq(next)` if the queue is non-empty, else `MonitorTxQueue()`.
5. No ACK before `VerifyHarqAck` (`:1129`) fires → exponential backoff + retransmit (same as association).

## Verdict summary

The data path correctly **reuses** the association random-access machinery for the uplink (backoff, LBT, fit-check, CW doubling on missing response) and adds a real FT→PT HARQ ACK that is sent as a scheduled response (no LBT) with a graceful busy-PHY drop. However, turning a single-shot association exchange into a **continuous stop-and-wait cycle** exposes a **critical concurrency defect (D1)** and a **stall (D2)** that did not exist in the association case, plus two runtime-crash risks. The remaining differences from the standard are deliberate simplifications consistent with the dummy-data scenario.

| # | Finding | Classification | Severity |
|---|---------|----------------|----------|
| D1 | Stale `VerifyHarqAck` on the reused HARQ process → null-context crash / misattribution | **Critical bug** | Critical |
| D2 | `MonitorTxQueue` re-trigger commented out → cycle stalls when the queue empties | **Bug** | High |
| D3 | FT `NS_FATAL_ERROR` on data from an unregistered PT | **Runtime-crash-risk** (assertion kept by choice) | Low |
| D4 | `responseAbsSubslot` can overflow the frame → scheduler `.at()` out-of-range | **Runtime-crash-risk** (indirectly mitigated; no explicit guard) | Medium |
| D5 | No sequence-number / NDI duplicate detection → duplicate over-count on ACK loss | **Design choice** (known) | — |
| D6/D7 | No NACK/CRC handling; any received DATA PDU = ACK; no downlink-data path | **Design choice** | — |
| D8 | ACK sent without LBT/backoff, graceful `IsAvailable` drop | **Design choice** (correct/consistent) | — |
| M-data | `LaunchApp`'s direct `InitiateHarq` pops `m_txQueue.front()` with no empty-check | **Minor latent-bug** | Low |

---

## Detailed findings

### D1 — Stale `VerifyHarqAck` on the reused HARQ process (critical)

**Symptom:** in the continuous cycle, the next packet is started the instant the ACK is received, while the *previous* packet's response-window timer (`VerifyHarqAck`) is still pending. That stale timer then fires against the freshly-armed next packet.

**Mechanism:** `VerifyHarqAck` events are identified **only by HARQ process number** (always `0`), with no per-transmission token, and a completed/ACKed transmission's pending `VerifyHarqAck` is **never cancelled**.

- `t0`: packet A transmitted; `VerifyHarqAck_A` scheduled at `t0 + responseWindow`.
- The ACK for A arrives **before** `t0 + responseWindow` (guaranteed by the configured response-window sizing) → `HandleHarqAck` → `process->Reset()` (`randomAccessContext = nullptr`, state `FREE`) → queue non-empty → `InitiateHarq(B)`:

```959:964:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
    process->state = Dect2020ExtHarqProcess::WAITING_ACK;
    process->ndi = true; // New data with no previous versions
    process->packet = packet->Copy();
    process->data = data;
```

  `InitiateHarq(B)` sets `state = WAITING_ACK` but does **not** set `randomAccessContext` — that happens only when the scheduled `InitiateRandomAccess(B, newContext=true)` runs (next frame, because the ACK is processed after the RACH window closes).
- `t0 + responseWindow` (still frame N): stale `VerifyHarqAck_A` fires, finds process `0`, sees `state == WAITING_ACK`, and executes:

```1156:1157:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
        uint32_t newCW = std::min(process->randomAccessContext->CW_CURRENT * 2, process->randomAccessContext->CW_MAX);
        process->randomAccessContext->CW_CURRENT = newCW;
```

  At this moment `process->randomAccessContext == nullptr` → **null-pointer dereference → simulation crash.**

**Two failure modes depending on timing:**
- (a) B's context not yet rebuilt (common with once-per-frame RACH) → **null-deref crash**.
- (b) B's `InitiateRandomAccess` already ran → no crash, but the stale verify **misattributes to B**: doubles B's CW and launches a **duplicate** RACH chain (two `m_phy->Send` + two `VerifyHarqAck_B`), breaking the stop-and-wait invariant and corrupting backoff statistics.

This is **new to data**; the association path had exactly one packet, so a stale verify could never collide with a subsequent transmission.

**Classification: Critical bug.**

**Recommended fix (agreed):** collapse cycle progression to a **single decision point**.
- `HandleHarqAck` should only **record** the ACK (free the HARQ process), and nothing else.
- `VerifyHarqAck` becomes the sole serialization point, firing once per transmission at the response-window boundary, and decides:
  - process free (ACK received) **and** queue non-empty → `InitiateHarq(next)`;
  - process free **and** queue empty → `MonitorTxQueue()`;
  - process still `WAITING_ACK` → `InitiateRandomAccess` with exponential backoff (retransmit).

Because the next packet's context and `VerifyHarqAck` are created only *after* the current `VerifyHarqAck` has fired and consumed itself, no two `VerifyHarqAck` events for different packets can coexist, and the null-context window disappears.

**Trade-off (accepted):** the next packet always waits until the response-window boundary even when the ACK arrived early (up to ~`responseWindow` extra latency per packet). This is consistent with strict stop-and-wait and keeps the queue→TX→ACK delay measurement meaningful.

---

### D2 — `MonitorTxQueue` re-trigger is disabled → cycle stalls on an empty queue (bug)

The empty-queue fallback can detect a new packet but cannot start transmitting it:

```1214:1226:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
void
Dect2020ExtMac::MonitorTxQueue()
{
    if (m_txQueue.empty())
    {
        Simulator::Schedule(MilliSeconds(1), &Dect2020ExtMac::MonitorTxQueue, this);
    }
    else
    {
        NS_LOG_INFO(MAC_IDENTITY << ": Detected a new packet in Tx queue");
        // InitiateHARQ();
    }
}
```

When a packet finally arrives, it logs "Detected a new packet" and then does nothing (`InitiateHarq` commented out), so the data cycle **stalls permanently** for any configuration where the queue empties — which is an explicitly intended scenario (delay measurement). Under the D1 fix this is the path `VerifyHarqAck` takes when the queue is empty, so it must work.

**Classification: Bug.** **Fix:** the packet-detected branch must call `InitiateHarq(data=true)`. The **1 ms polling** mechanism is kept intentionally (configurable, not performance-critical) — recorded as a design choice rather than requiring an event-driven enqueue notification.

---

### D3 — FT fatal-errors on data from an unregistered PT (runtime-crash-risk)

```1815:1818:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
    else
    {
        NS_FATAL_ERROR(MAC_IDENTITY << ": PT with longRdId " << targetId << " not found");
    }
```

Usually unreachable in the intended scenario (a PT transmits data only after association, which registers it at the FT; a wrong-FT recipient is filtered earlier by `PacketIsForThisDevice`), but a hard `NS_FATAL_ERROR` aborts the whole simulation on any desync (stray/duplicate/late data PDU, or a future multi-FT-same-channel topology).

**Classification: Runtime-crash-risk (low).** Per author decision, the `NS_FATAL_ERROR` is **kept** as an intentional invariant assertion; documented here as a crash risk (inconsistent with the graceful-drop policy used for beacons/responses).

---

### D4 — Response subslot can overflow the frame (runtime-crash-risk)

```1779:1787:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
        uint32_t responseAbsSubslot = currentAbsSubslot + harqDelay + 1;
        ...
        // It is assumed that RACH resources cannot be configured to enter the next frame ->
        // GetTimeTillSubslotStartNs should be safe to call
        double t = m_macScheduler->GetTimeTillSubslotStartNs(responseAbsSubslot);
```

If `currentAbsSubslot + harqDelay + 1 ≥ subslotsPerFrame`, then `GetSubslotStartTimeOffsetNs` computes `slotNumber ≥ 24` and `m_macScheduler.at(nStart).at(slotNumber)` throws `std::out_of_range` → crash. Guarded only by a comment-level assumption, not a runtime check.

**Mitigation present:** RACH is always configured not to exceed one frame, and `EvaluateLbtChannel`'s `CheckIfRachTxFitsInRemainingResources` only transmits when the (request+response) window fits in the remaining RACH, which keeps the ACK subslot inside the RACH window and therefore inside the frame. So the overflow is **indirectly** prevented — but there is no explicit guard, and the guarantee depends on the fit-check and response-window sizing remaining correct.

**Classification: Runtime-crash-risk (medium).** **Recommended fix:** add a config-time assertion (`raraEndSubslot + maxHarqFeedbackDelay + 1 (+ response TX subslots) < subslotsPerFrame`) or modulo-wrap and handle the cross-frame case explicitly.

---

### D5 — No duplicate detection (design choice)

Every dummy data PDU is built with `SequenceNumber(0)` (`EnqueuePacket`, `:2360`) and `NDI` is always `true`, so when an ACK is lost (FT PHY busy → `SendHarqAck` drops, or ACK collision) and the PT retransmits, the FT cannot recognize the retransmission and processes/ACKs it again. §5.6/HARQ would use the MAC sequence number / NDI to discard duplicates. Net effect: delivered/throughput counts are over-counted on ACK loss (relevant under multi-PT contention).

**Classification: Design choice (well known).** Dummy packets are transmitted without maintaining sequence numbers or new-data indication. (`m_device->Receive` up-stack forwarding is explicitly out of scope.)

---

### D6 / D7 — ACK semantics (design choice)

- **No NACK / no CRC:** `HandleHarqAck` (`:1088`) resets the HARQ process on *reception* of the response, without inspecting feedback format/info or any CRC result — reception always means success (consistent with the no-SINR/no-CRC PHY model). The §5.3.4/§5.5 NACK and "CRC of DATA part fails ⇒ retransmit" paths are not implemented.
- **Any DATA PDU = ACK:** the PT branch of `HandleDataPacket` routes every received DATA_MAC_PDU into `HandleHarqAck`; there is no downlink-data path, so a hypothetical FT→PT data packet would be read as an ACK. Harmless here because the FT only sends ACKs.

**Classification: Design choice.**

---

### D8 — ACK sent without LBT/backoff (design choice, correct)

The uplink data PDU uses the full RACH path; the downlink ACK is sent as a scheduled response with only an availability check:

```1075:1084:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
    // Drop the ACK if the PHY is already busy with another packet
    if (!m_phy->IsAvailable())
    {
        NS_LOG_INFO(MAC_IDENTITY << "Dropping Data Packet ACK due to busy PHY");
        return;
    }

    bool data = true;
    bool random = true;
    m_phy->Send(packet, phyControlFieldType2, data, random);
```

This mirrors `SendAssociationResponse` and matches the standard's model where the random-access **response** is sent by the FT in its own resources, not subject to the requester's RACH contention/LBT. The graceful `IsAvailable` drop is exactly the behavior recommended in the association review's F6.

**Classification: Design choice (correct / consistent with the association response).**

---

### M-data — `LaunchApp` initiates HARQ without an empty-queue check (minor latent-bug)

`LaunchApp` (`:2385`) calls `InitiateHarq(data=true)` directly, and `InitiateHarq` does `m_txQueue.front()` / `pop()` with no empty-check (`:956`). Harmless today because `m_app->Start()` enqueues exactly one packet first, but a misconfiguration such as `packetLimit == 0` (or any future path that launches with an empty queue) would pop an empty queue → undefined behavior / crash. All other `InitiateHarq` callers (`HandleHarqAck`, and `VerifyHarqAck` under the D1 fix) are guarded by a non-empty check.

**Classification: Minor latent-bug.** (Author will decide on a fix — e.g., guard `InitiateHarq` with an empty-queue check, or only launch when a packet exists.)

---

## Inherited findings (the data uplink reuses the identical RACH code path)

These were classified in `Random Access Compliance Findings.md` and apply unchanged to data transmission:

- **assoc-F2 (TODO):** the response-window size is an unenforced configuration assumption. **The D1 fix's correctness directly depends on it** — the ACK must arrive before `VerifyHarqAck` fires, otherwise the device retransmits a packet that was actually delivered. Add the runtime check recommended in F2.
- **assoc-F3 (possible deviation ???):** LBT senses the interval *at* start position `k` and transmits at `k+1`.
- **assoc-F4 (possible deviation, unresolved):** backoff is re-drawn each RACH occurrence instead of one persistent `rachBackOff` carried across frames.
- **assoc-F5 (intentional simplification):** no `dectRachResourceFailure` / `maxRetx`. For data this means a packet that is never ACKed is **retransmitted forever** (CW saturates at `CW_MAX`).
- **assoc-F7 (modeling-choice):** PHY first-wins capture, no SINR — governs how colliding data transmissions resolve (one winner + retries rather than mutual loss).
- **assoc-F8 (intentional simplification):** `NDI` always `true` / fixed redundancy version on (re)transmissions — overlaps D5.

---

## Decision log

- **Deliverable = separate findings report**, no code changes — written beside the association report; classifications agreed item-by-item.
- **Cycle trace confirmed**, and **both regimes in scope** — queue non-empty at ACK time (steady state) *and* queue empty at ACK time (configurable, for delay measurement) — so both re-trigger paths must be correct.
- **D1 = critical bug** — stale `VerifyHarqAck` on the reused HARQ process causes a null-context dereference (or misattribution + duplicate TX); root cause is that the verify is keyed only by process number and never cancelled on ACK.
- **D1 fix = single decision point** — `HandleHarqAck` only frees the process; `VerifyHarqAck` decides next action (next packet / monitor / retransmit). Accepted trade-off: next packet waits to the response-window boundary even on an early ACK.
- **D2 = bug** — `MonitorTxQueue` must call `InitiateHarq` on packet detection; 1 ms polling kept as a configurable design choice.
- **D3 = runtime-crash-risk** — author keeps the `NS_FATAL_ERROR` as an intentional invariant assertion; documented as a crash risk.
- **D4 = runtime-crash-risk** — frame overflow is indirectly mitigated by the RACH fit-check + response-window sizing, but lacks an explicit guard; recommend a config-time assertion or modulo-wrap.
- **D5 = design choice** — dummy packets without sequence numbers / NDI; duplicate over-count on ACK loss is known and accepted; `m_device->Receive` forwarding is out of scope.
- **D6/D7 = design choice** — no NACK/CRC; any received DATA PDU is treated as a positive ACK; no downlink-data path.
- **D8 = design choice (correct)** — ACK is a scheduled, non-contention response (no LBT) with a graceful busy-PHY drop, consistent with the association response and F6.
- **M-data = minor latent-bug** — `LaunchApp`'s unguarded `InitiateHarq` would pop an empty queue under misconfiguration; author will decide on a fix.
- **Inherited findings carry over** — especially assoc-F2 (response-window sizing) which the D1 fix relies on, and assoc-F5 (no abort) which means unacked data retransmits indefinitely.
