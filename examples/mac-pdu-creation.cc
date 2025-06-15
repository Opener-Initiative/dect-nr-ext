#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    Dect2020BeaconHeader* beacon = new Dect2020BeaconHeader();
    Dect2020BeaconHeader* deserializedBeacon = new Dect2020BeaconHeader();
    Dect2020MacMultiplexingHeaderTypeC mxHeader;
    Dect2020MacMultiplexingHeaderTypeC deserializedMxHeader;

    beacon->SetNetworkId(0xFFFFFF);
    beacon->SetTransmitterAddress(0xFFFFFFFF);

    mxHeader.SetType(Dect2020MacMultiplexingHeaderTypeC::Type::C);
    mxHeader.SetMacExtension(0x00);
    mxHeader.SetIEType(Dect2020MacMultiplexingHeaderTypeC::ExtendedIEType::NetworkBeaconMessage);
    mxHeader.AddPayload(beacon, beacon->GetSerializedSize());

    deserializedMxHeader.AddPayload(deserializedBeacon, deserializedBeacon->GetSerializedSize());

    Packet packet;
    packet.AddHeader(mxHeader);
    packet.RemoveHeader(deserializedMxHeader);
}