#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header-type-b.h"
#include "ns3/dect2020-network-beacon-message-ie.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    Ptr<Packet> pdu = Dect2020MacPduHelper::BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );

    Dect2020MacMultiplexingHeaderTypeB mxHeader;
    mxHeader.SetType(Dect2020MacMultiplexingHeader::Type::B);
    mxHeader.SetMacExtension(0x00);
    mxHeader.SetLength(0x00);
    mxHeader.SetIEType(0x00);
    mxHeader.InsertSDU(0x00);

    // mxHeader.Serialize()
    pdu -> AddHeader(mxHeader);
}