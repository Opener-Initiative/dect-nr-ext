#include "dect2020ext-mac-pdu-size-calculation-test-case.h"

#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-config.h"

#include <random>

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

    std::random_device rd;
    std::mt19937 mt(rd());
    std::bernoulli_distribution packetLengthTypeDist(0.5);
    std::uniform_int_distribution<unsigned int> packetLengthInBytesDist(22, 35); // range of sizes for implemented MAC messages

    bool packetLengthType = packetLengthTypeDist(mt);
    auto packetLengthInBytes = static_cast<uint32_t>(packetLengthInBytesDist(mt));
    uint32_t size;

    if (packetLengthType)
    {
        size = mac->CalculateMacPduSizeInSlots(packetLengthInBytes);
        std::cout << "Size in slots: " << size;
    }
    else
    {
        size = mac->CalculateMacPduSizeInSubslots(packetLengthInBytes);
        std::cout  << "Size in subslots: " << size;
    }
}

} // namespace ns3
