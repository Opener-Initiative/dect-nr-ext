#include "ns3/node.h"
#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-net-device.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    Ptr<Node> node = CreateObject<Node>();
    Ptr<Dect2020Channel> channel = CreateObject<Dect2020Channel>();
    Ptr<Dect2020NetDevice> device = CreateObject<Dect2020NetDevice>();

    device->SetChannel(channel->GetWrappedChannel());
    node->AddDevice(device);

    return 0;
}