# Design Documentation of the DECT NR+ NS-3 Implementation

# FT Device
---
# Initialization

# Initializing Network

# Spectrum Management Procedure

# Beacon Transmission

After the background scan and operating channel selection (Spectrum Management Procedure), the FT device will start beacon transmissions. The FT will wait till the 21st frame is over and start transmitting beacons from there.
Network Beacon - transmitted in the first subslot
Cluster Beacon - transmitted in the second subslot



# Association Procedure

# Random Access Procedure

#### Random Access Transmission

Initiate the backoff process:
- Extract data from Random Access Resource IE
- Based on RACH resources subslots, STF, and GI - calculate all possible start positions within RACH resources subslots
- Calculate CW_MIN and CW_MAX based on CWmin_sig and CWmax_sig
- Set CW_CURRENT to CW_MIN
- Wait till the start of RACH resources subslots
- Pick a random value between 0 and CW_CURRENT
- Start timer rachBackOff with the random value
- Keep rachBackOff running during RACH resources subslots
- After rachBackOff expires, measure the channel for at least MINIMUM_LBT_PERIOD
- Determine whether the channel is "free" or "possible" for transmission
- If the channel is detected "free" or "possible":
	- If the remaining MAC PDU fits to the remaining RACH resources subslots:
		- Initiate random access transmission
	- Else:
		- Defer random access transmission to the next occurrence of random access resources
- Else channel is detected as "busy"
	- Initiate the backoff process as defined above

NOTE: After transmitting to random access the RD may re-initiate the random access transmission procedure to the same random access resources









