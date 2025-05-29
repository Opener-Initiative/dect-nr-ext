#include "ns3/dect2020-mac-pdu-helper.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    // Create DATA MAC PDU
    Ptr<Packet> unicastPdu = Dect2020MacPduHelper::UNICAST(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        1,
        1,
        0xFFF,
        0xFFF
    );
    return 0;
}