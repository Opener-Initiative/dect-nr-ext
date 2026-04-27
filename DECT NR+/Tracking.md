# Tasks
- Add ranges to all modules in MAC PDU:
	- Physical Header Field
	- MAC Header Type
	- MAC Common Headers
	- IEs
- Add comments to every MAC PDU module (with names)
- Check MAC Common Headers, MAC Header Type, Physical Header Field, and Information Elements
- MAC Common Header tests - make test values random (use mt19937 predefined random number generator)

# Done

Write tests for:
- MAC PDU: All MAC PDUs (MAC Header Type, Information Elements, MAC Messages)
- MAC: MAC PDU size calculation in slots and subslots
- PHY: MAC PDU duration in nanoseconds (based on its size in slots or subslots)
Explain HARQ in diagram
Rename HARQ to ARQ or stop and wait

# Suggestions
1. Update the Statistics module - include delay: timing of RACH, channel occupancy
2. ARQ Protocols:
	1. Stop and Wait
	2. GobackN
	3. Selective Repeat
3. ARQ protocols on Layer 2 DLC (for flow control and error control)
4. Flow Control is to adapt the Tx transmission to the Rx speed
5. HARQ = ARQ (L2) + FEC (Turbo Coding on PHY)

# Issues to be resolved

# Changes to the implementation documentation
- Implemented PHY Tx state machine -> update diagram 8

# Questions
1. Does network byte order matter in MAC PDUs? Should host to network byte orders be used?