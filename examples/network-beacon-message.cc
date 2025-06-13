#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020-network-beacon-message-ie.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("NetworkBeaconMessageExample");

int
main(int argc, char* argv[])
{
    Ptr<Packet> pdu = Dect2020MacPduHelper::BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );

    Dect2020MacMultiplexingHeaderTypeB mxHeader;
    mxHeader.SetType(Dect2020MacMultiplexingHeader::Type::A);
    mxHeader.SetMacExtension(0x00);
    mxHeader.SetLength(0x00);
    mxHeader.SetIEType(0x00);
    mxHeader.InsertSDU(0x00);
}