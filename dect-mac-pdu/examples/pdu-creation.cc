#include "ns3/dect2020-mac-pdu.h"

#include "ns3/core-module.h"
#include "ns3/pointer.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("PduCreationExample");

using PduType = Dect2020MacPdu::PduType;

int
main(int argc, char* argv[])
{
    Dect2020MacPdu pdu;
    pdu.SetPduType(PduType::BROADCAST);
    Ptr<Packet> packet = pdu.CreateMacPdu();

    return 0;
}