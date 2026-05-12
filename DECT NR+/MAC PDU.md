mu = 1, beta = 1
# 1 Spectrum Management Procedure
## 1.1 Network Beacon Message
Transmitter: FT
Receiver: PT
Build functions: `Dect2020ExtMac::BuildNetworkBeacon`, `Dect2020ExtMac::BuildPhysicalHeaderField`
Size: 22 bytes

| Position       | Network Beacon Message Structure | Size    |
| -------------- | :------------------------------: | ------- |
| Index 4: back  |    Network Beacon Message IE     | 8 bytes |
| Index 3        |  MAC Multiplexing Header Type A  | 1 byte  |
| Index 2        |          Beacon Header           | 7 bytes |
| Index 1        |         MAC Header Type          | 1 byte  |
| Index 0: front |     PHY Control Field Type 1     | 5 bytes |
Configuration for optional fields:
- Network Beacon Message IE:
	- Cluster Max TX power field is not included
	- The RD operating in FT mode does not have power constraints (power constraints are not particularly relevant to ns-3)
	- The current cluster channel is the same as the next cluster channel. The current cluster channel field is not included
	- Number of additional Network beacon channels: 0
## 1.2 Cluster Beacon Message
Transmitter: FT
Receiver: PT
Build functions: `Dect2020ExtMac::BuildClusterBeacon`, `Dect2020ExtMac::BuildPhysicalHeaderField`
Size: 23 bytes

| Position       | Cluster Beacon Message Structure | Size    |
| -------------- | :------------------------------: | ------- |
| Index 5: back  |    Cluster Beacon Message IE     | 4 bytes |
| Index 4        |    Random Access Resource IE     | 5 bytes |
| Index 3        |  MAC Multiplexing Header Type A  | 1 byte  |
| Index 2        |          Beacon Header           | 7 bytes |
| Index 1        |         MAC Header Type          | 1 byte  |
| Index 0: front |     PHY Control Field Type 1     | 5 bytes |
Configuration for optional fields:
- Cluster Beacon Message IE:
	- Cluster Max TX power field is not included
	- The RD operating in FT mode does not have power constraints (power constraints are not particularly relevant to ns-3)
	- Frame Offset field is not present
	- The cluster channel for the next cluster beacon period is the same as the current cluster channel. The next cluster channel field is not included
	- The next cluster beacon is transmitted based on Cluster beacon period. The Time to next field is not present
- Random Access Resource IE:
	- Repeat: resource allocation is single allocation, and the repetition and validity fields are not present (since RACH resources always come after the Cluster Beacon)
	- SFN is not included
	- The resource allocation is valid for the channel where the IE is received. The channel field is not present in the IE
	- The random access response is sent on the same channel as the random access message
# 2 Association Procedure
## 2.1 Association Request Message
Transmitter: PT
Receiver: FT
Build function: `Dect2020ExtMac::SendAssociationRequest`
Size: 35 bytes

| Position       | Association Request Message Structure | Size     |
| -------------- | :-----------------------------------: | -------- |
| Index 6: back  |        Association Control IE         | 1 byte   |
| Index 5        |           RD Capability IE            | 7 bytes  |
| Index 4        |    Association Request Message IE     | 5 bytes  |
| Index 3        |    MAC Multiplexing Header Type A     | 1 byte   |
| Index 2        |            Unicast Header             | 10 bytes |
| Index 1        |            MAC Header Type            | 1 byte   |
| Index 0: front |       PHY Control Field Type 2        | 10 bytes |
## 2.2 Association Response Message
Transmitter: FT
Receiver: PT
Build function: `Dect2020ExtMac::SendAssociationResponse`
Size: 30 bytes

| Position       | Association Response Message Structure | Size     |
| -------------- | :------------------------------------: | -------- |
| Index 5: back  |    Association Response Message IE     | 1 byte   |
| Index 4        |            RD Capability IE            | 7 bytes  |
| Index 3        |     MAC Multiplexing Header Type A     | 1 byte   |
| Index 2        |             Unicast Header             | 10 bytes |
| Index 1        |            MAC Header Type             | 1 byte   |
| Index 0: front |        PHY Control Field Type 2        | 10 bytes |
