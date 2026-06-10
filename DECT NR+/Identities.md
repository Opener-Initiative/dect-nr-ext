Information is referenced and cited from ETSI TS 103 636-4 V2.1.1
# Network ID
The Network ID has a length of 32 bits, and the first 24 MSB bits are used to identify a DECT-2020 network uniquely from other DECT-2020 networks. The 8 LSB bits of the network are selected locally to minimize collision with other networks.

The network ID is transmitted periodically in beacon messages as plain text enabling other RDs to detect which network the transmitted beacon belongs to.

In the implementation, Network ID occurs in:
- PHY Control Field as `Short Network ID`
- PHY Control Field as `Short Network ID`
- Beacon Header as `Network ID`

# Long Radio Device ID (Long RD ID)
The Long radio device ID has a length of 32 bits, and it identifies a radio device uniquely in a single DECT-2020 network. The coding of the Long RD ID is defined in Table 4.2.3.2-1. An RD obtains the Long RD ID as part of the authentication process over the DECT-2020 NR system or via manual provision or by using other communication channels.

In the implementation, Long RD ID occurs in:
- Beacon Header as `Transmitter Address`
- Unicast Header as `Transmitter Address` and `Receiver Address`
- RD Broadcasting Header as `Transmitter Address`

# Short Radio Device ID (Short RD ID)
The Short radio device ID has a length of 16 bits, and it identifies a radio device locally in the radio neighbourhood in a DECT-2020 NR network. The coding of the Short RD ID is defined in Table 4.2.3.3-1. The Short RD ID is used in the PHY control field as in transmitter or receiver fields to identify the transmitter and the receiver of the packet.

The Short RD ID is exchanged during the association procedure between RDs performing association so that linkage between Short RD ID and Long RD ID is obtained by both associating RD. Each radio device randomly selects a Short RD ID value that it uses in the association.

In the implementation, Short RD ID occurs in:
- PHY Control Field Type 1 as `Transmitter Identity`
- PHY Control Field Type 2 as `Transmitter Identity`