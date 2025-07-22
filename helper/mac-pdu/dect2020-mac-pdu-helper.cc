#include "dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-data-mac-pdu-header.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-unicast-header.h"
#include "ns3/dect2020-rd-broadcasting-header.h"

namespace ns3
{

typedef Dect2020MacHeaderType::MacHeaderTypeField MacHeaderTypeField;

Dect2020MacPduHelper::Dect2020MacPduHelper(){}
Dect2020MacPduHelper::~Dect2020MacPduHelper(){}

Ptr<Packet>
Dect2020MacPduHelper::DATAMACPDU(MacSecurityField security,
                                    uint8_t reset,
                                    uint16_t sequenceNumber)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020MacHeaderType macHeaderType;
    Dect2020DataMacPduHeader dataMacPduHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::DATA_MAC_PDU_HEADER);

    dataMacPduHeader.SetReset(reset);
    dataMacPduHeader.SetSequenceNumber(sequenceNumber);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(dataMacPduHeader);

    return packet;
}

Ptr<Packet>
Dect2020MacPduHelper::BEACON(MacSecurityField security,
                                uint32_t networkId,
                                uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020MacHeaderType macHeaderType;
    Dect2020BeaconHeader beaconHeader;

    macHeaderType.SetMacSecurity(security);
    macHeaderType.SetMacHeaderTypeField(MacHeaderTypeField::BEACON_HEADER);

    beaconHeader.SetNetworkId(networkId);
    beaconHeader.SetTransmitterAddress(transmitterAddress);

    packet->AddHeader(macHeaderType);
    packet->AddHeader(beaconHeader);

    return packet;
}

Ptr<Packet>
Dect2020MacPduHelper::UNICAST(MacSecurityField security,
                                uint8_t macSequence,
                                uint8_t sequenceNumber,
                                uint32_t receiverAddress,
                                uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020MacHeaderType macHeaderType;
    Dect2020UnicastHeader unicastHeader;

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
Dect2020MacPduHelper::RDBROADCASTING(MacSecurityField security,
                                        uint8_t reset,
                                        uint16_t sequenceNumber,
                                        uint32_t transmitterAddress)
{
    Ptr<Packet> packet = Create<Packet>();
    Dect2020MacHeaderType macHeaderType;
    Dect2020RDBroadcastingHeader rdBroadcastingHeader;

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