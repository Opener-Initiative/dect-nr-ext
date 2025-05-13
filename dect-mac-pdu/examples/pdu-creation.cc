#include "ns3/dect2020-rd-broadcasting-header.h"
#include "ns3/core-module.h"
#include "ns3/packet.h"
#include "ns3/pointer.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("PduCreationExample");


int
main(int argc, char* argv[])
{
    Dect2020RdBroadcastingHeader header;
    Dect2020RdBroadcastingHeader des;
    Packet packet;
    header.SetReset(0x1);
    header.SetSequenceNumber(0xF0F);
    header.SetTransmitterAddress(0xF0F0F0FF);
    header.Print(std::cout);
    packet.AddHeader(header);
    packet.RemoveHeader(des);
    des.Print(std::cout);

    return 0;
}