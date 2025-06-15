#include "dect2020-route-info-test-case.h"
#include "ns3/dect2020-route-info.h"
#include "ns3/packet.h"

namespace ns3 {

Dect2020RouteInfoTestCase::Dect2020RouteInfoTestCase() : TestCase("RouteInfoSerializationTest")
{
}

Dect2020RouteInfoTestCase::~Dect2020RouteInfoTestCase()
{
}

void
Dect2020RouteInfoTestCase::DoSetup()
{
}

void
Dect2020RouteInfoTestCase::DoRun()
{
    Dect2020RouteInfo originalHeader;
    Dect2020RouteInfo deserializedHeader;
    Packet packet;
    uint32_t sinkAddress = 0xFBFED010;
    uint8_t routeCost = 0x1F;
    uint8_t applicationSequenceNumber = 0xFF;

    originalHeader.SetSinkAddress(sinkAddress);
    originalHeader.SetRouteCost(routeCost);
    originalHeader.SetApplicationSequenceNumber(applicationSequenceNumber);

    std::cout << "[ORIGINAL VALUES]" << "\n";
    std::cout << "Original sink address: 0x" << std::hex << originalHeader.GetSinkAddress() << "\n";
    std::cout << "Original route cost: 0x" << std::hex << originalHeader.GetRouteCost() << "\n";
    std::cout << "Original application sequence number: 0x" << std::hex << (int)originalHeader.GetApplicationSequenceNumber() << "\n" << "\n";

    packet.AddHeader(originalHeader);
    packet.RemoveHeader(deserializedHeader);

    std::cout << "[DESERIALIZED VALUES]" << "\n";
    std::cout << "Deserialized sink address: 0x" << std::hex << deserializedHeader.GetSinkAddress() << "\n";
    std::cout << "Deserialized route cost: 0x" << std::hex << deserializedHeader.GetRouteCost() << "\n";
    std::cout << "Deserialized application sequence number: 0x" << std::hex << (int)deserializedHeader.GetApplicationSequenceNumber() << "\n" << "\n";

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetSinkAddress(), 
                         sinkAddress,
                         "Sink address mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetRouteCost(), 
                         routeCost,
                         "Route cost mismatch after serialization/deserialization");

    NS_TEST_ASSERT_MSG_EQ(deserializedHeader.GetApplicationSequenceNumber(), 
                         applicationSequenceNumber,
                         "Application sequence number mismatch after serialization/deserialization");
}

} // namespace ns3