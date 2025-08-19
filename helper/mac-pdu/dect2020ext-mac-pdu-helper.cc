#include "dect2020ext-mac-pdu-helper.h"
#include "ns3/dect2020ext-data-mac-pdu-header.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-unicast-header.h"
#include "ns3/dect2020ext-rd-broadcasting-header.h"

namespace ns3
{

typedef Dect2020ExtMacHeaderType::MacHeaderTypeField MacHeaderTypeField;

Dect2020ExtMacPduHelper::Dect2020ExtMacPduHelper(){}
Dect2020ExtMacPduHelper::~Dect2020ExtMacPduHelper(){}

Ptr<Packet>
Dect2020ExtMacPduHelper::DATAMACPDU(MacSecurityField security,
                                    uint8_t reset,
                                    uint16_t sequenceNumber)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020ExtMacHeaderType macHeaderType;
    Dect2020ExtDataMacPduHeader dataMacPduHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::DATA_MAC_PDU_HEADER);

    dataMacPduHeader.SetReset(reset);
    dataMacPduHeader.SetSequenceNumber(sequenceNumber);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(dataMacPduHeader);

    return packet;
}

Ptr<Packet>
Dect2020ExtMacPduHelper::BEACON(MacSecurityField security,
                                uint32_t networkId,
                                uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020ExtMacHeaderType macHeaderType;
    Dect2020ExtBeaconHeader beaconHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::BEACON_HEADER);

    beaconHeader.SetNetworkId(networkId);
    beaconHeader.SetTransmitterAddress(transmitterAddress);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(beaconHeader);

    return packet;
}

Ptr<Packet>
Dect2020ExtMacPduHelper::UNICAST(MacSecurityField security,
                                uint8_t macSequence,
                                uint8_t sequenceNumber,
                                uint32_t receiverAddress,
                                uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020ExtMacHeaderType macHeaderType;
    Dect2020ExtUnicastHeader unicastHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::UNICAST_HEADER);

    unicastHeader.SetMacSequence(macSequence);
    unicastHeader.SetSequenceNumber(sequenceNumber);
    unicastHeader.SetReceiverAddress(receiverAddress);
    unicastHeader.SetTransmitterAddress(transmitterAddress);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(unicastHeader);

    return packet;
}

Ptr<Packet>
Dect2020ExtMacPduHelper::RDBROADCASTING(MacSecurityField security,
                                        uint8_t reset,
                                        uint16_t sequenceNumber,
                                        uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020ExtMacHeaderType macHeaderType;
    Dect2020ExtRDBroadcastingHeader rdBroadcastingHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::RD_BROADCASTING_HEADER);

    rdBroadcastingHeader.SetReset(reset);
    rdBroadcastingHeader.SetSequenceNumber(sequenceNumber);
    rdBroadcastingHeader.SetTransmitterAddress(transmitterAddress);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(rdBroadcastingHeader);

    return packet;
}

} // namespace ns3