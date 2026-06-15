# `data` Option Trace + Association→Data Handoff — Findings

**Reference standard:** ETSI TS 103 636-4 V2.1.1 (2024-10), §5.3 (Random Access) + §5.5 (HARQ).
**Companion documents:** `Random Access Compliance Findings.md` (association), `Data Transmission Compliance Findings.md` (data cycle, incl. **D1**).
**Module under review:** `contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc` (+ `model/phy/dect2020ext-phy.cc`).
**Purpose:** trace the `data` boolean through the association, random-access, and data-transmission paths; verify it is used correctly; decide whether `m_phy->Send` needs it; and capture a follow-on bug discovered during the trace (the association→data handoff).
**Deliverable:** findings only.

## What the `data` option means

`data == false` → the packet is an **Association Request** (control); `data == true` → the packet is a **data MAC PDU**. It is set from the parameter of `InitiateHarq(bool data)`.

## Trace of the `data` flag

| Stage | Behavior |
|---|---|
| **Origins** (`InitiateHarq(bool data)`) | `SendAssociationRequest → InitiateHarq(false)`; `LaunchApp → InitiateHarq(true)`; `VerifyHarqAck → InitiateHarq(data)` (data-cycle advance); `MonitorTxQueue → InitiateHarq(true)` |
| `InitiateHarq` | stores it: `process->data = data` (`:964`) |
| `InitiateRandomAccess` | has **no** `data` parameter and does **not** set `randomAccessContext->data` |
| `EvaluateLbtChannel` | reads `randomAccessContext->process->data` for `m_phy->Send(..., process->data, random)` (`:1471`) and forwards it to `VerifyHarqAck(..., process->data)` (`:1477`) |
| `VerifyHarqAck(..., bool data)` | the **only** place `data` changes behavior: after the retransmit block, `if (data)` advances the data cycle; `data==false` (association) takes no advance |
| `m_phy->Send → params->m_data → EndRx → callback → ReceiveFromPhy(..., bool data, ...)` | `ReceiveFromPhy` **never reads `data`**; it routes purely by `macHeaderType` (BEACON / UNICAST / DATA_MAC_PDU) and only uses `random` (Type 1 vs Type 2 PHY control field) |

**Source of truth:** `process->data` (set in `InitiateHarq`). `EvaluateLbtChannel` reads it correctly for both the transmission and `VerifyHarqAck`. Association → `false`, data → `true`. The conceptual design of `VerifyHarqAck`'s `if (data)` advance gate is correct (association is one-shot; data cycles).

## Findings

### F-DATA-1 (was "I1") — Dead `data` / `random` fields in `Dect2020ExtRandomAccessContext` - done

`InitiateRandomAccess` never sets `randomAccessContext->data` (it has no `data` parameter), and nothing ever reads `randomAccessContext->data` or `::random`. The real flag travels via `process->data`. The struct fields are uninitialized and dead — a latent trap if anyone ever reads them.

**Classification: cleanup / latent-trap.**
**Decision: remove** `data` and `random` from `Dect2020ExtRandomAccessContext`.

### F-DATA-2 (was "I2") — `data` is unused on the PHY/RX path → `m_phy->Send` does not need it - done

The `data` argument is carried `Send → params->m_data → EndRx → receive callback → ReceiveFromPhy` and then never used; packet type is determined by the MAC header type, and only `random` is needed by the PHY/receiver.

**Classification: dead parameter on the PHY interface.**
**Decision: remove** the `data` argument from `m_phy->Send`, `Dect2020ExtSpectrumSignalParameters::m_data`, `Dect2020ExtPhy::EndRx`, the receive callback signature, and `ReceiveFromPhy`. Keep `random`.

> **Precision (must not over-remove):** This removal is limited to the **PHY-carried** `data`. **Keep** `process->data` and the `VerifyHarqAck(..., bool data)` parameter — that is the meaningful flag, and the F-DATA-3 fix depends on it to distinguish an Association Request from a data packet.

### F-DATA-3 (new bug; same class as D1) — The association→data handoff reintroduces the D1 race - done

`ProcessAssociationResponse` starts the next transmission on ACK *reception*:

```2111:2132:contrib/dect-nr-ext/model/mac/dect2020ext-mac.cc
    if (ftInfo->associationResponseMessageIe.GetAssociationAccepted())
    {
        Dect2020ExtHarqProcess* process = FindHarqProcess(ftInfo->phyControlFieldType2.GetDfHarqProcessNumber());

        // Reset the HARQ process
        process->Reset();
        ...
        if (m_proceedToDataTransmission)
        {
            // Starting app
            LaunchApp();
            StartKeepAlivePeriod();
        }
    }
```

The Association Request was sent via `InitiateHarq(false)`, so a `VerifyHarqAck(..., data=false)` is **pending** at `t0 + responseWindow`. Because the response window is deliberately oversized, the Association Response (the ACK) is always processed first → `ProcessAssociationResponse` → `process->Reset()` → `LaunchApp() → InitiateHarq(true)`, which re-arms the **single** HARQ process to `WAITING_ACK` for the first data packet (its context not yet rebuilt). Then the stale `VerifyHarqAck(data=false)` fires, sees `state == WAITING_ACK`, and enters the **retransmit block** → dereferences `process->randomAccessContext->CW_CURRENT`:
- context not yet rebuilt → **null-pointer dereference → crash**;
- context already rebuilt → **misattribution**: retransmits the first data packet and doubles its CW.

This is exactly the **D1** mechanism (`ProcessAssociationResponse` here plays the role the old buggy `HandleHarqAck` played), occurring at the association→first-data-packet boundary. The `data=false` flag does **not** protect it, because the retransmit block runs before the `if (data)` gate.

**Classification: critical bug (same class as D1).**

#### Agreed fix (single decision point — the D1 pattern)

`ProcessAssociationResponse` must only **record** the result and launch nothing; `VerifyHarqAck` becomes the single point that starts data transmission once the association request is acknowledged.

**Crux (make-or-break):** `LaunchApp()` and `StartKeepAlivePeriod()` must be **moved out** of `ProcessAssociationResponse` and **into** `VerifyHarqAck`. If they are *added* to `VerifyHarqAck` but *left* in `ProcessAssociationResponse`, the app launches twice → the second `InitiateHarq` finds the single process still `WAITING_ACK` → `"No free HARQ process"` crash, and the race is not fixed.

```cpp
// ProcessAssociationResponse: record only — NO LaunchApp / StartKeepAlivePeriod
process->Reset();
ChangeAssociationStatus(ASSOCIATED);
m_associatedFtNetDeviceLongRdId = ftInfo->unicastHeader.GetTransmitterAddress();

// VerifyHarqAck, AFTER the WAITING_ACK retransmit block (so state == FREE == acknowledged):
if (data) {                                  // data packet acknowledged -> advance the cycle
    if (!m_txQueue.empty()) InitiateHarq(true);
    else                    MonitorTxQueue();
} else {                                      // Association Request acknowledged
    if (!m_associationRequestAccepted) {
        m_associationRequestAccepted = true;
        if (m_proceedToDataTransmission) { LaunchApp(); StartKeepAlivePeriod(); }
    }
}
```

Notes:
- The new boolean `m_associationRequestAccepted` is a good **idempotent-launch** guard, though strictly redundant: reaching the post-retransmit code already implies `state == FREE` (acknowledged), and `data == false` already implies "Association Request," so a single launch is guaranteed by the single-decision-point structure. Keep it as a safety net; the **essential** change is relocating `LaunchApp`.
- Behavioral effect (consistent with the D1 trade-off): data transmission now starts at `t0 + responseWindow` (when the association `VerifyHarqAck` fires) instead of the instant the Association Response is received — a small, acceptable delay.
- `LaunchApp` / `StartKeepAlivePeriod` currently have a single caller (`ProcessAssociationResponse`, lines 2130–2131), so the move is self-contained.

### F-DATA-4 (was "I4") — `SendHarqAck` sends the ACK with `data=true` - done

The ACK is HARQ feedback, not a data PDU, yet it is sent with `data=true` (`:1109–:1111`). Harmless because `data` is unused on RX, and **moot once F-DATA-2 removes the PHY `data` argument**.

**Classification: cosmetic.**
**Decision: fix** (subsumed by F-DATA-2's removal of the PHY `data` argument).

## Answer to "does `m_phy->Send` need the `data` option?"

**No.** It is dead on the PHY/RX path (receiver routes by MAC header type; only `random` is needed). Remove it per F-DATA-2 — while keeping the MAC-side `process->data` / `VerifyHarqAck(bool data)`.

## Decision log

- **Trace conclusion** — the effective `data` flag is `process->data` (set in `InitiateHarq`); `EvaluateLbtChannel` uses it correctly; its only behavioral use is `VerifyHarqAck`'s advance gate.
- **F-DATA-1 = remove dead fields** — `Dect2020ExtRandomAccessContext::data`/`::random` are never set or read.
- **F-DATA-2 = remove PHY-carried `data`** — unused on RX; `m_phy->Send`/`m_data`/`EndRx`/callback/`ReceiveFromPhy` drop it; `random` stays.
- **Precision** — do **not** remove `process->data` or `VerifyHarqAck(bool data)`; the handoff fix relies on them.
- **F-DATA-3 = critical bug (same class as D1)** — `ProcessAssociationResponse` launching the app on ACK reception collides with the association request's pending `VerifyHarqAck` (null-context crash / misattribution), deterministic due to the oversized response window.
- **F-DATA-3 fix** — move `LaunchApp`/`StartKeepAlivePeriod` out of `ProcessAssociationResponse` into `VerifyHarqAck`'s "association acknowledged" branch; add `m_associationRequestAccepted` as an idempotent-launch guard; the relocation is the essential part.
- **F-DATA-4 = cosmetic** — `SendHarqAck` `data=true` is moot once F-DATA-2 is applied.
- **`m_phy->Send` verdict** — does not need the `data` option; remove it.
