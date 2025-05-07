#include "ns3/core-module.h"
#include "ns3/packet.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-unicast-header.h"
#include "ns3/dect2020-route-info.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SerializationDeserializationExample");

int
main(int argc, char* argv[])
{
    Packet packet;
    Dect2020BeaconHeader originalHeader;
    Dect2020BeaconHeader deserializedHeader;
    uint32_t networkId = 0xBCE68F; // 24-bit ID
    uint32_t transmitterAddress = 0xA0120000; // 32-bit address

    originalHeader.SetNetworkId(networkId);
    originalHeader.SetTransmitterAddress(transmitterAddress);

    std::cout << "Original network ID: 0x" 
                << std::hex << originalHeader.GetNetworkId() << "\n";
    std::cout << "Original transmitter address: 0x"
                << std::hex << originalHeader.GetTransmitterAddress() << "\n";

    packet.AddHeader(originalHeader);

    packet.RemoveHeader(deserializedHeader);

    std::cout << "Deserialized network ID: 0x" 
                << std::hex << deserializedHeader.GetNetworkId() << "\n";
    std::cout << "Deserialized transmitter address: 0x"
                << std::hex << deserializedHeader.GetTransmitterAddress() << "\n";
}