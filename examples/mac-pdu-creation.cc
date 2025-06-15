#include "ns3/dect2020-network-beacon-message-ie.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    Header* header = new Dect2020NetworkBeaconMessageIE;
    header->Print(std::cout);
}