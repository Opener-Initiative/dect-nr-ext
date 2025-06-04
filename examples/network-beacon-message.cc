#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header.h"
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
    Dect2020MacMultiplexingHeader muxHeader;
    muxHeader.SetType(Dect2020MacMultiplexingHeader::Type::C);
    std::cout << "Type: " << muxHeader.GetType() << std::endl;
    std::cout << bool(muxHeader.GetType() == Dect2020MacMultiplexingHeader::Type::C) << std::endl;
    return 0;
}