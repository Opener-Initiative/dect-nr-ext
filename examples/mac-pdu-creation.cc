#include "ns3/dect2020-mac-pdu-helper.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MacPduCreationExample");

// Create example MAC PDU with MAC header Type and Common header as DATA MAC PDU
Ptr<Packet>
CreateDataMacPdu()
{
    Ptr<Packet> pdu = Dect2020MacPduHelper::DATAMACPDU(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        1,
        1
    );
    return pdu;
}

int
main(int argc, char* argv[])
{
    Ptr<Packet> dataMacPdu = CreateDataMacPdu();

    return 0;
}