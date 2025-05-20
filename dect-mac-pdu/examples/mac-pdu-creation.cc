#include "ns3/dect2020-mac-header-type.h"

#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-data-mac-pdu-header.h"
#include "ns3/dect2020-rd-broadcasting-header.h"
#include "ns3/dect2020-unicast-header.h"

#include "ns3/core-module.h"
#include "ns3/packet.h"
#include "ns3/pointer.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

int
main(int argc, char* argv[])
{
    Dect2020MacHeaderType macHeaderType;

    // Different MAC Common headers
    Dect2020BeaconHeader beaconHeader;
    Dect2020DataMacPduHeader dataMacPduHeader;
    Dect2020RDBroadcastingHeader rdBroadcastingHeader;
    Dect2020UnicastHeader unicastHeader;

    return 0;
}