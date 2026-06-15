# Quick Links
- DECT-2020 New Radio (NR):
	- [Part 2: Radio reception and transmission requirements; ETSI TS 103 636-2 V2.1.1](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363602/02.01.01_60/ts_10363602v020101p.pdf)
	- [Part 3: Physical layer; ETSI TS 103 636-3 V2.1.1](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363603/02.01.01_60/ts_10363603v020101p.pdf)
	- [Part 4: MAC layer; ETSI TS 103 636-4 V2.1.1](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf)
	- [Part 5: DLC and Convergence layers; ETSI TS 103 636-5 V2.1.1](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363605/02.01.01_60/ts_10363605v020101p.pdf)
- [ns-3](https://www.nsnam.org/)
# Logging
The default logging setup (logging levels) looks like this:
```c++
LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
LogComponentEnable("Dect2020ExtStatistics", LogLevel(LOG_LEVEL_INFO));
```
Logging levels are usually defined at the start of every simulation script.

Usually, simulation scripts produce an extremely large amount of logs. `splitlogs.py` Python script divides the logs into separate files based on the identity of objects. The identity of objects is constructed from their node's index and operational mode: Fixed Termination point (FT) or Portable Termination point (PT).

Each PHY and MAC object has its own `PHY_IDENTITY` or `MAC_IDENTITY`. This is important for logging since it allows to divide the logs from different models into separate files. The current default way of logging information (for MAC for example) is:
```c++
NS_LOG_INFO(MAC_IDENTITY << "some interesting information");
```
Note, if the information is logged without using a proper identity, the logs will not be separated correctly by the `splitlogs.py` script.
# MAC Initialization with `Dect2020ExtMac::Initialize`
This step is important for `Dect2020ExtMac` since it allows to check if important values have been configured before `Dect2020ExtMac::Start`. The structure of `Dect2020ExtMac::Initialize` is as follows:
- Check all config values (provided by `Dect2020ExtConfig` and simulation file)
- Initialize MAC Scheduler
- Initialize a backoff random variable (used in RACH)
- Generate Network ID if the device is FT
# `Dect2020ExtMac::Start` and `Dect2020ExtPhy::Start`
#### `Dect2020ExtMac::Start`:
- If the device is FT:
	- Check if Random Access Resource has been configured
	- Create a `MAC_IDENTITY`
	- Generate and set long and short radio device IDs
	- Call `Dect2020ExtMac::InitializeNetwork`
- If the device is PT:
	- Check if Association Control IE has been configured
	- Create a `MAC_IDENTITY`
	- Generate and set long and short radio device IDs
	- Call `Dect2020ExtMac::DiscoverNetworks`
# Important Conventions
Channel number (`uint16_t m_operatingChannelNumber`) stands for the absolute channel frequency number. Using channel IDs is avoided in the code base since it confuses what a channel ID and channel number are. Since the standard refers to the absolute channel frequency NUMBERING, channel numbers are used in the code, not channel IDs.
# Spectrum Management Procedure
`Dect2020ExtMac::InitializeNetwork` starts a background scan (`Dect2020ExtMac::BackgroundScan`) immediately (but the delay can also be modified manually). `Dect2020ExtMac::BackgroundScan` extracts band parameters from `Dect2020ExtChannelManager` to get the number of channels inside the operating band. The function schedules RSSI measurements for each channel and creates one `Dect2020ExtBackgroundScanContext` for the whole background scan and one `Dect2020ExtRssi1OnChannelContext` for each channel. The background scan function schedules RSSI measurements for each channel based on 10ms period (10 ms = 1 frame = all subslots to scan for each channel).
#### Operating Channel
The goal of the FT device performing the background scan is to select the operating channel number. Operating channel will be used as the channel for cluster beacon transmissions and (currently) for network beacon transmissions (which is not required by the standard but still implemented for simplification).

PT device doesn't perform any background scans. Therefore, the operating channel number will be defined as the channel which the PT uses to communicate with the FT. This channel is configured via the Cluster Beacon Message IE.
#### `Dect2020ExtMac::ReceiveFromPhy`
Reception of Network and Cluster Beacons is implemented through the `Dect2020ExtMac::ReceiveFromPhy` function. If random access (together with HARQ) have been used to transmit the packet (indicated by the `Dect2020ExtSpectrumSignalParameters::m_random` boolean variable), the PHY Control Field Type 2 header is deserialized. If the packet has not been transmitted with random access (for example a beacon transmission), the PHY Control Field Type 1 header is deserialized from the packet. After that point, packet handling happens based on MAC Header Type 

If the packet is a data packet (indicated by the `Dect2020ExtSpectrumSignalParameters::m_data` boolean variable), the PHY Control Field Type 2 header is deserialized and if the packet is not data, the PHY Control Field Type 1 header is deserialized from the packet. Then, packet handling happens based on MAC Header Type:
- If MAC Header Type is `BEACON_HEADER` and the receiving device is PT (only PT devices analyze beacons):
	1. Create FT Candidate Info (`Dect2020ExtFtCandidateInfo`)
	2. Call `Dect2020ExtMac::HandleBeaconPacket`
- If MAC Header Type is `UNICAST_HEADER` (PT and FT can process unicast packets):
	1. Call `Dect2020ExtMac::HandleUnicastPacket`
- If MAC Header Type is `DATA_MAC_PDU_HEADER` (PT and FT can process data packets):
	1. Call `Dect2020ExtMac::HandleDataPacket`
# Association Procedure
#### Association State
- NOT_ASSOCIATED: the PT device is not associated with any FT device
- ASSOCIATION_PREPARING: the PT device is scanning the network to find the best FT candidate 
- WAITING_FOR_SELECTED_FT: this state is switched upon the best FT candidate selection and can trigger association with the selected FT when the next Cluster Beacon arrives
- ASSOCIATION_PENDING: this state indicates that Association Request Message has been sent to the FT and the PT is waiting for Association Response Message
- ASSOCIATED: indicates that the device is associated with the selected FT device
#### FT Candidate Info
The FT Candidate Info is created or found with `Dect2020ExtMac::FindOrCreateFtCandidate`. FTs are distinguished by Transmitter Identity (short RD ID) inside `Dect2020ExtMac::m_ftCandidates` vector. If the candidate info is created in `Dect2020ExtMac::ReceiveFromPhy`, it is populated with the following data:
1. RSSI value
2. FT PHY Header Field (Type 1)
3. Reception time (`Simulator::Now` in this case)
4. Short RD ID (Transmitter Identity)
# Statistics
The `Dect2020ExtStatistics` model currently supports logging the number of accepted and dropped packets based on **the** `PHY_IDENTITY`.
# HARQ (Stop-and-Wait ARQ) and Random Access Procedure
#### HARQ
In DECT NR+, the procedure responsible for automatic retransmissions if packets are dropped or not correctly decoded is Hybrid Automatic Repeat Request (HARQ). Even though this procedure is referred to as "HARQ" in the code base, the procedure is not fully implemented.
The current "HARQ" implementation only involves Automatic Repeat Request (ARQ) method with 1 HARQ process.
The repeat requests are done upon the timeout (RACH response window exceeds and no ACK has been received).
The RACH response window should always be longer than HARQ feedback delay to give the FT time to respond to the packet.
The processing time for creating the feedback is two subslots or as indicated by the RD in the `HARQ feedback delay` field in RD capability IE.

Feedback delay: n + `HARQ feedback delay` + 1.
#### `Dect2020ExtMac::InitiateHarq`
This function is responsible for triggering HARQ and Random Access. It controls when `Dect2020ExtMac::InitiateRandomAccess` will be called and which packet is dequeued from the Tx queue.

The function acts as follows:
1. Find a free HARQ process (this is designed to expand the implementation in the future). Currently, only 1 HARQ process is available (with ID 0). The function will raise `NS_FATAL_ERROR` if a free HARQ process was not found. There should always be a free process when `Dect2020ExtMac::InitiateHarq` is called. Stop-and-Wait ARQ protocol is used in the context of HARQ during the Random Access procedure.
2. Pop a packet from the Tx queue `Dect2020ExtMac::m_txQueue`
3. Prepare the process and build the PHY Control Field Type 2
4. Get current subslot number and random access resource allocation start and end subslots
5. If the RACH resources are yet to come in the current frame:
	1. Schedule `Dect2020ExtMac::InitiateRandomAccess` at the start of the RACH resources in the current frame
6. If the device is already inside or at the start of the RACH resources:
	1. Execute `Dect2020ExtMac::InitiateRandomAccess` immediately
7. If the device is outside of the RACH resources:
	1. Schedule `Dect2020ExtMac` at the next RACH resources in the next frame
8. At least one of the above conditions has to be met and the simulation will continue with `Dect2020ExtMac::InitiateRandomAccess`
#### `Dect2020ExtMac::InitiateRandomAccess`
This function is designed to purely control the Random Access. It is supposed to be called either inside or at the start of the RACH resources. `Dect2020ExtMac::InitiateHarq` is responsible for correctly triggering `Dect2020ExtMac::InitiateRandomAccess`.

The function directly follows the DECT-2020 NR standard acts as follows:
1. Get the start subslot, the length, and the length type of RACH resources from Random Access Resource IE
2. If RACH length is in subslots:
	1. Determine RACH duration in microseconds by calling `Dect2020ExtMacScheduler::GetTimeTillSubslotEndNs` and passing RACH end subslot (start subslot + length) to the function. This way, if the function is called either at the start or inside of the RACH resources it will get the correct available RACH duration
3. If RACH length is in slots:
	1. Raise `NS_FATAL_ERROR`. RACH resources shouldn't be in slots
4. Calculate all possible start positions withing the available RACH duration
5. Initialize CW_MIN, CW_MAX, and CW_CURRENT
6. Initialize the `Dect2020ExtRandomAccessContext`
7. Call `InitiateBackoff`
#### `Dect2020ExtMac::InitiateBackoff`
This function initiates the backoff and also handles the case where all available positions were exceeded. If this is the case, the procedure start from the begining by scheduling `Dect2020ExtMac::InitiateRandomAccess` again at the next RACH resources in the next frame. It is also important to start from `Dect2020ExtMac::InitiateRandomAccess` to avoid timing issues with starting positions. It is better to just recalculate them once again for the new RACH resources then trying to track down if existing starting positions still apply to the resources. 

The function acts as follows:
1. Get the backoff value from a random number generator (the backoff value will be the position index)
2. Ensure that all available positions are not exceeded
3. Calculate the backoff duration in microseconds by getting the starting position from `randomAccessContext->startPositions[backoffValue]`
4. Schedule LBT with the backoff delay

# Data Transmission
#### General Description
Data transmission is implemented via HARQ operation and Random Access. MAC PDU formats that belong to HARQ and Random Access are used to configure how data transmission will happen.
# MAC PDU Size and Tx Duration Calculation
#### MAC PDU Size Calculation Based on Modulation and Coding Scheme (MCS)
The MAC PDU size calculation in slots and subslots is implemented at the MAC level: `Dect2020ExtMac::CalculateMacPduSizeInSlots` and `Dect2020Ext::CalculateMacPduSizeInSubslots`. Both functions have a single parameter - `uint32_t packetLengthInBytes`. The Transport Block Size (TBS) values for single slot transmission and single spatial stream with maximum turbo code block size of 2048 bits are derived from [ETSI TS 103 636-3 V2.1.1](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363603/02.01.01_60/ts_10363603v020101p.pdf#%5B%7B%22num%22%3A336%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C540%5D) Table C.2-1.
The table is implemented as follows:
```c++
const std::vector<uint16_t> Dect2020ExtPhy::m_singleSlotSingleStreamTransportBlockSizesMu1Beta1
{
    136,  // MCS 0
    296,  // MCS 1
    456,  // MCS 2
    616,  // MCS 3
    936,  // MCS 4
    1256, // MCS 5
    1416, // MCS 6
    1576, // MCS 7
    1896, // MCS 8
    2040, // MCS 9
    2296, // MCS 10
    2552  // MCS 11
};
```

The computation of MAC PDU size in slots and subslots is as follows:
1. Convert size from bytes to bits by multiplying `packetLengthInBytes` by 8.
2. Get MCS Transport Block Size (TBS) by calling `Dect2020ExtPhy::GetMcsTransportBlockSize` and passing alpha, beta, and configured MCS index (at the MAC level) to the function and storing the result in `mcsTransportBlockSizeBitsPerSlot`.
3. If the length is calculated in subslots:
	1. Get subslots per slot from `Dect2020ExtChannelManager` and store the result in `subslotsPerSlot`
	2. Calculate transport block size per subslot by dividing `mcsTransportBlockSizeBitsPerSlot` by `subslotsPerSlot`
	3. Calculate packet MAC PDU length in subslots (with rounding up)  by calling `std::ceil(packetLengthInBits / mcsTransportBlockSizeBitsPerSubslot`
4. If the length is calculated in slots:
	1. Calculate packet length in slots (with rounding up) by calling `std::ceil(packetLengthInBits / mscTransportBlockSizeBitsPerSlot`

**Currently, only MCS index 0 is supported. TBS for MCS 0 is 136.**
#### Tx Duration Calculation
Tx duration is calculated in the `Dect2020ExtPhy::Send` function and inserted into `Dect2020ExtSpectrumSignalParameters`. Those parameters are crucial for transmission over the `Spectrum` channel.
Tx duration in nanoseconds is as follows:
1. If packet length is given in subslots:
	1. TBD
2. If packet length is given in slots:
	1. Multiply packet length (in slots) by 416670 (416.67 nanoseconds per slot)
# MAC PDU
All MAC PDU formats are serialized with host to network byte order (big-endian) and deserialized with network to host byte order (small-endian).
#### Currently implemented MAC PDU formats
- [Physical Header Field](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A404%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C347%5D):
	- Type 1, Header Format: 000
	- Type 2, Header Format: 000
	- Type 2, Header Format: 001
- [MAC Header Type](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A568%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C350%5D)
- MAC Common Headers:
	- [Beacon Header](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A691%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C455%5D)
	- [Data MAC PDU](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A691%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C619%5D)
	- [RD Broadcasting Header](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A763%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C445%5D)
	- [Unicast Header](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A691%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C218%5D)
- [MAC Multiplexing Header](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf)
	- Type A
- Information Elements:
	- [Network Beacon Message IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A993%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C306%5D)
	- [Cluster Beacon Message IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A996%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C207%5D)
	- [Association Request Message IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A1175%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C406%5D)
	- [Association Response Message IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A1273%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C627%5D)
	- [Random Access Resource IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A1713%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C218%5D)
	- [RD Capability IE](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/02.01.01_60/ts_10363604v020101p.pdf#%5B%7B%22num%22%3A1818%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22FitH%22%7D%2C620%5D)
#### MAC PDU Macros
`DECT2020EXT_MAC_PDU_FATAL(message, outputValue)` - outputs a fatal error with the message and the output value (which is wrong in this case)
# Tests
#### Currently Available Tests:
- Physical Header Field:
	- Serialization/Deserialization tests:
		- PHY Control Field: Type 1, Header Format: 000
		- PHY Control Field: Type 2, Header Format: 000
		- PHY Control Field: Type 2, Header Format: 001
- MAC Header Type:
	- Serialization/Deserialization test
- MAC Common Headers:
	- Serialization/Deserialization tests:
		- Beacon Header
		- Data MAC PDU
		- RD Broadcasting Header
		- Unicast Header
- MAC Multiplexing Header:
	- Serialization/Deserialization test:
		- Type A
- Information Elements;
	- Serialization/Deserialization tests:
		- Network Beacon Message IE
		- Cluster Beacon Message IE
		- Association Request Message IE
		- Association Response Message IE
		- Random Access Resource IE
		- RD Capability IE
- Messages
	- Serialization/Deserialization tests:
		- Cluster Beacon Message
		- Network Beacon Message
- MAC PDU size calculation test
#### MAC PDU Tests
All serialization/deserialization tests for MAC Common Headers are implemented with the following principle:
1. Two header objects are initiated - one for the original header and one for the deserialized header
2. One packet object is initiated to simulate adding headers to MAC PDU
3. Values for header fields are generated randomly, for example:
```c++
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<uint32_t> networkIdDist(0, 0xFFFFFF);
std::uniform_int_distribution<uint32_t> transmitterAddressDist(0, 0xFFFFFFFF);

uint32_t networkId = networkIdDist(mt); // 24-bit ID
uint32_t transmitterAddress = transmitterAddressDist(mt); // 32-bit address

originalHeader.SetNetworkId(networkId);
originalHeader.SetTransmitterAddress(transmitterAddress);
```
4. When adding header to the packet, the size of the packet before and after addition is also tested
#### Running Tests
All tests can be executed with the `test.py` ns-3 script:
1. Navigate to the root directory of ns-3
2. To run all `dect2020ext` tests, run this command in the terminal:
```bash
python test.py --suite="dect2020ext*"
```
3. To run a specific `TestSuite`, run this command in the terminal (regex can also be used to specify what to run):
```bash
python test.py --suite="dect2020ext-beacon*"
```

Tips:
1. To be 100% sure that the tested models work correctly, it is probably a good idea to run tests a couple of times. Some tests, for example, MAC Common Header tests, can be tested with random values and therefore should be tested a couple of times to see how the models behave with different random values.
# Future Work
#### Dect2020ExtTypes
