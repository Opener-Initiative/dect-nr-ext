#include "dect2020ext-mac-pdu-size-calculation-test-case.h"

#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-config.h"

namespace ns3
{

Dect2020ExtMacPduSizeCalculationTestCase::Dect2020ExtMacPduSizeCalculationTestCase()
    : TestCase("Dect2020ExtMacPduSizeCalculationTestCase")
{
}

Dect2020ExtMacPduSizeCalculationTestCase::~Dect2020ExtMacPduSizeCalculationTestCase()
{
}

void
Dect2020ExtMacPduSizeCalculationTestCase::DoSetup()
{
}

void
Dect2020ExtMacPduSizeCalculationTestCase::DoRun()
{
    Dect2020ExtConfig config;

    Ptr<Dect2020ExtPhy> phy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(phy);

    Ptr<Dect2020ExtMac> mac = CreateObject<Dect2020ExtMac>();
    config.ApplyToMac(mac);

    mac->SetPhy(phy);
    phy->SetMac(mac);

    auto printResult = [](const std::string& name,
                          uint32_t sizeBytes,
                          uint32_t expectedSlots,
                          uint32_t calculatedSlots,
                          uint32_t expectedSubslots,
                          uint32_t calculatedSubslots)
    {
        std::cout << "----------------------------------------\n"
                  << name << "\n"
                  << "  Size (bytes)       : " << sizeBytes << "\n"
                  << "  Slots    - expected: " << expectedSlots
                  <<            "  calculated: " << calculatedSlots << "\n"
                  << "  Subslots - expected: " << expectedSubslots
                  <<            "  calculated: " << calculatedSubslots << "\n";
    };

    // Network Beacon Message
    uint32_t networkBeaconMessageSize = 22; // bytes
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSlots(networkBeaconMessageSize), 2, "Error: 22 bytes should occupy 2 slots, got " << mac->CalculateMacPduSizeInSlots(networkBeaconMessageSize));
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSubslots(networkBeaconMessageSize), 3, "Error: 22 bytes should occupy 3 subslots, got " << mac->CalculateMacPduSizeInSubslots(networkBeaconMessageSize));
    printResult("Network Beacon Message",
                networkBeaconMessageSize, 2,
                mac->CalculateMacPduSizeInSlots(networkBeaconMessageSize),
                3,
                mac->CalculateMacPduSizeInSubslots(networkBeaconMessageSize));

    // Cluster Beacon Message
    uint32_t clusterBeaconMessageSize = 23; // bytes
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSlots(clusterBeaconMessageSize), 2, "Error: 23 bytes should occupy 2 slots, got " << mac->CalculateMacPduSizeInSlots(clusterBeaconMessageSize));
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSubslots(clusterBeaconMessageSize), 3, "Error: 23 bytes should occupy 3 subslots, got " << mac->CalculateMacPduSizeInSubslots(clusterBeaconMessageSize));
    printResult("Cluster Beacon Message",
                clusterBeaconMessageSize, 2,
                mac->CalculateMacPduSizeInSlots(clusterBeaconMessageSize),
                3,
                mac->CalculateMacPduSizeInSubslots(clusterBeaconMessageSize));

    // Association Request Message
    uint32_t associationRequestMessage = 35; // bytes
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSlots(associationRequestMessage), 3, "Error: 35 bytes should occupy 3 slots, got " << mac->CalculateMacPduSizeInSlots(associationRequestMessage));
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSubslots(associationRequestMessage), 5, "Error: 35 bytes should occupy 5 subslots, got " << mac->CalculateMacPduSizeInSubslots(associationRequestMessage));
    printResult("Association Request Message",
                associationRequestMessage, 3,
                mac->CalculateMacPduSizeInSlots(associationRequestMessage),
                5,
                mac->CalculateMacPduSizeInSubslots(associationRequestMessage));

    // Association Response Message
    uint32_t associationResponseMessage = 30; // bytes
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSlots(associationResponseMessage), 2, "Error: 30 bytes should occupy 2 slots, got " << mac->CalculateMacPduSizeInSlots(associationResponseMessage));
    NS_TEST_ASSERT_MSG_EQ(mac->CalculateMacPduSizeInSubslots(associationResponseMessage), 4, "Error: 30 bytes should occupy 4 subslots, got " << mac->CalculateMacPduSizeInSubslots(associationResponseMessage));
    printResult("Association Response Message",
                associationResponseMessage, 2,
                mac->CalculateMacPduSizeInSlots(associationResponseMessage),
                4,
                mac->CalculateMacPduSizeInSubslots(associationResponseMessage));

    std::cout << "----------------------------------------\n";
}

} // namespace ns3
