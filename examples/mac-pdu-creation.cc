#include "ns3/dect2020-mac-multiplexing-header.h"
#include "ns3/dect2020-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020-mac-multiplexing-header-type-b.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    Dect2020MacMultiplexingHeaderTypeA headerA;
    headerA.SetIEType(Dect2020MacMultiplexingHeader::NoPayloadIEType::KeepAliveIE);
    std::cout << "headerA: " << headerA.GetIEType() << std::endl;
}