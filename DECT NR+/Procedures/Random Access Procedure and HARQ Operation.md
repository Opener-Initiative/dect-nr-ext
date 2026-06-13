Information is referenced and cited from ETSI TS 103 636-4 V2.1.1
# Random Access Transmission
#### `Dect2020ExtMac::InitiateHarq`
This function is responsible for triggering the HARQ Operation and the Random Access Procedure. This function is always used by PT devices. It controls when `Dect2020ExtMac::InitiateRandomAccess` is called and how the packet is dequeued from the front of the TX queue `Dect2020ExtMac::m_txQueue`. The dect-nr-ext implementation is designed to transmit packets using the Random Access Procedure by first putting them into the TX queue, initiating the HARQ Operation, and then initiating the Random Access Procedure. The result of this function is correctly scheduling `Dect2020ExtMac::InitiateRandomAccess` with a prepared HARQ process for the transmission. `Dect2020ExtMac::InitiateRandomAccess` must be always called either inside or at the start of the RACH resources. The RACH resources are defined as RACH start subslot + RACH length so that:
1. If RACH start subslot is 10, it is possible to get the time in nanoseconds till the start of the RACH resources in the current frame by calling `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs` which will return the time in nanoseconds till subslot 10 start
2. If RACH start subslot is 10 and RACH length is 30, it is possible to calculate the total RACH resource duration by calling `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs` with RACH start subslot + RACH length as a parameter. RACH start subslot + RACH length = 40, but the actual subslots included in the RACH resources are from the subslot 10 till the subslot 39. This way, it is safe to call `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs` to get the full duration of the RACH resources.

The function does the following actions:
1. Call `Dect2020ExtMac::FindFreeHarqProcess` in order to find a free HARQ process. Currently, only 1 HARQ process is available (with process number 0). The function will raise `NS_FATAL_ERROR` if a free HARQ process was not found. There must always be a free HARQ process when `Dect2020ExtMac::InitiateHarq` is called. Stop-and-Wait ARQ protocol is used in the context of the HARQ Operation during the Random Access Procedure.
2. Pop a packet from the TX queue
3. Prepare the process:
	1. Set NDI (New Data Indication) to `true` (new data with no previous versions)
	2. Assign a TX packet to the process (to keep track of which packet is currently transmitted and which process it belongs to)
	3. Assign the boolean `data` value to the process to indicate whether this packet is a data packet and not Association Request Message
4. Build the PHY Control Field Type 2
5. Conduct sanity check to assert that the RACH length is defined in subslots
6. Correctly schedule `Dect2020ExtMac::InitiateRandomAccess` with a new context to fulfill those conditions:
	1. If the RACH resources are yet to come in the current frame -> schedule `Dect2020ExtMac::InitiateRandomAccess` at the start of the RACH resources in the current frame
	2. If the device is already inside or at the start of the RACH resources -> execute `Dect2020ExtMac::InitiateRandomAccess` immediately
	3. If the device is outside of the RACH resources -> schedule `Dect2020Ext::InitiateRandomAccess` at the next RACH resources in the next frame

#### `Dect2020ExtMac::InitiateRandomAccess`
This function is designed to purely control the Random Access. It is supposed to be called either inside or at the start of the RACH resources. `Dect2020ExtMac::InitiateHarq` is responsible for correctly triggering `Dect2020ExtMac::InitiateRandomAccess`.

The function directly follows the DECT-2020 NR standard and does the following actions:
1. Get the duration of the RACH resources to calculate all possible starting positions. Using `Dect2020ExtMacScheduler::GetTimeTillSubslotStartNs` with RACH start subslot + RACH length as a parameter allows to correctly get the remaining RACH duration if the device is at the start or inside the RACH resources.
2. Calculate all possible starting positions (`START_POSITION_INTERVAL_US` = `STF_US` + `GI_US`)
3. If a new Random Access Procedure context is created:
	1. Configure CW_MIN, CW_MAX as defined in the DECT-2020 NR standard
	2. Set CW_CURRENT to CW_MIN. Note, a new context indicates that the Random Access Procedure is being conducted for a new packet. This means that CW_MIN, CW_MAX, and CW_CURRENT will be reset and recalculated for each new packet (each new packet doesn't mean re-transmissions due to collisions or no ACK, it means with each new packet from the TX queue)
	3. Assign the new context to the corresponding HARQ process
4. If existing Random Access Procedure context is used:
	1. Update the starting positions (since they are recalculated each time a re-transmission happens)
5. Initiate backoff

#### `Dect2020ExtMac::Dect2020ExtInitiateBackoff`
This function initiates the backoff and also handles the case where all available positions were exceeded. If this is the case, the procedure starts from the begining by scheduling `Dect2020ExtMac::InitiateRandomAccess` again at the next RACH resources in the next frame. It is also important to start from `Dect2020ExtMac::InitiateRandomAccess` to avoid timing issues with starting positions. It is better to just recalculate them once again for the new RACH resources then trying to track down if existing starting positions still apply to the resources. The same context is passed to `Dect2020ExtMac::InitiateRandomAccess`. Also, the function does a sanity check to assert that the packet fits into the MAC Random Access TX Time (the implementation should be configured correctly).

The function does the following actions:
1. Get a random backoff value uniformly distributed between 0 and CW_CURRENT (the backoff value will be the position index)
2. Handle the case where available positions are exceeded by rescheduling `Dect2020ExtInitiateRandomAccess` at the start of the RACH resources in the next frame
3. Calculate the backoff duration in microseconds by getting the starting position from `randomAccessContext->startPositions[backoffValue]`
4. Schedule LBT (Listen Before Talk) with the backoff delay

#### `Dect2020ExtMac::PerformLbt`
The function performs the LBT protocol by sensing the energy on the channel 5 times within one `START_POSITION_INTERVAL_US`. The sampling points are defined as:
1. `0.0`: Start of STF
2. `STF_US / 2.0`: Middle of STF
3. `STF_US`: Start of GI
4. `STF_US + (GI_US / 2.0)`: Middle of GI
5. `STF_US + GI_US`: End of GI

The function loops through the sampling points and uses them as delays to schedule `Dect2020ExtMac::SampleLbtRssi`.

#### `Dect2020ExtMac::SampleLbtRssi`
The function does the following actions:
1. Get the current channel number and calculate the channel index
2. Sample the channel power from the PHY interference model (`Dect2020ExtPhy::m_interference`) and convert it from Watt to dBm
3. Store the sample in the Random Access Procedure context
4. Call `Dect2020ExtMac::EvaluateLbtChannel` if all samples are collected

#### `Dect2020ExtMac::EvaluateLbtChannel`
The function does the following actions:
1. Conducts a sanity check to assert that the device is currently not transmitting another packet (this happened in the past due to implementation errors)
2. Check if the RACH TX fits into the remaining RACH resources. RACH TX is defined as the response window (the worst case scenario which is the response window). `Dect2020ExtMac::EvaluateLbtChennel` calls `Dect2020ExtMac::CheckIfRachTxFitsInRemainingResources` to check if the duration of the response window (response window * `subslotDurationNs`) fits into the remaining RACH duration defined as `m_macScheduler->GetTimeTillSubslotStartNs(startSubslot + length)`. The response window is defined as `(Max RACH Tx length * 2) + 3 (HARQ feedback) + 1 (additional subslot to cover propagation delay)`. The HARQ feedback must always be 3 (HARQ feedback from RD Capability IE must always be 2)
3. If the RACH TX fits into the remaining RACH resources, the function determines the channel state based on RSSI thresholds
	1. If the channel is free or possible and the PHY has not started receiving in the meantime (race condition: PHY may enter RX between the LBT samples and `Dect2020ExtMac::EvaluateLbtChannel`):
		1. Transmit the packet immediately with the boolean `ranom` option as `true` (indicating that this transmission is conducted with the Random Access Procedure)
		2. Schedule `Dect2020ExtMac::VerifyAck` with the delay defined as the response window * `subslotDurationNs`
	2. If the channel is busy or the PHY entered RX right at the time when this function is called -> re-initiate the Random Access Procedure:
		1. Re-initiate the Random Access Procedure with the same context
4. If the RACH TX doesn't fit into the remaining RACH resources -> defer the TX till the next RACH resources in the next frame with the same context

#### `Dect2020ExtMac::VerifyHarq`
The function does the following actions:
1. Find a HARQ process with the process number from the given PHY Control Field Type 2
2. Check if the packet has been acknowledged by asserting that the process state is `WAITING_ACK`
3. If the process state is `WAITING_ACK`:
	1. Apply exponential backoff by multiplying `CW_CURRENT` by 2, but capping at CW_MAX
	2. Determine if the device is behind, at the start, inside, or already outside of the RACH resources (like in `Dect2020ExtMac::InitiateHarq`)
	3. Schedule the appropriate execution of `Dect2020ExtMac::InitiateRandomAccess`
4. If the process state is not `WAITING_ACK`:
	1. Simply do nothing, the execution continues without re-transmissions

#### Random Access Response (ACK)
The ACK is sent with the delay of `rdCapabilityIe.harqDelay + 1`. The HARQ delay value from the RD Capability IE must always be 2 to fulfill the default HARQ feedback duration of 3 subslots as specified in the DECT-2020 NR standard.