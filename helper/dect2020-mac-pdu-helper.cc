#include "dect2020-mac-pdu-helper.h"

namespace ns3
{

Dect2020MacPduHelper::Dect2020MacPduHelper(){}
Dect2020MacPduHelper::~Dect2020MacPduHelper(){}

Ptr<Packet>
Dect2020MacPduHelper::DATAMACPDU()
{
    Ptr<Packet> packet = Create<Packet>();
    return packet;
}

Ptr<Packet>
Dect2020MacPduHelper::BEACON()
{
    Ptr<Packet> packet = Create<Packet>();
    return packet;
}

Ptr<Packet>
Dect2020MacPduHelper::UNICAST()
{
    Ptr<Packet> packet = Create<Packet>();
    return packet;
}

Ptr<Packet>
Dect2020MacPduHelper::RDBROADCASTING()
{
    Ptr<Packet> packet = Create<Packet>();
    return packet;
}

} // namespace ns3