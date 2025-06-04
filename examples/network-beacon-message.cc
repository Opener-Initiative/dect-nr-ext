#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-network-beacon-message-ie.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("NetworkBeaconMessageExample");

Ptr<Packet>
CreateBeaconPdu()
{
    Ptr<Packet> pdu = Dect2020MacPduHelper::BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );
    return pdu;
}

int
main(int argc, char* argv[])
{
    Ptr<Packet> pdu =CreateBeaconPdu();
    return 0;
}