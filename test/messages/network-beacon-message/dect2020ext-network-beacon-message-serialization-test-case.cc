#include "dect2020ext-network-beacon-message-serialization-test-case.h"

#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"

#include "ns3/ptr.h"
#include "ns3/packet.h"

namespace ns3
{

Dect2020ExtNetworkBeaconMessageSerializationTestCase::Dect2020ExtNetworkBeaconMessageSerializationTestCase()
    : TestCase("Dect2020ExtNetworkBeaconMessageSerializationTestCase")
{
}

Dect2020ExtNetworkBeaconMessageSerializationTestCase::~Dect2020ExtNetworkBeaconMessageSerializationTestCase()
{
}

void
Dect2020ExtNetworkBeaconMessageSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtNetworkBeaconMessageSerializationTestCase::DoRun()
{
    // 1. Build Network Beacon Message

    Ptr<Packet> packet = Create<Packet>();

    Dect2020ExtNetworkBeaconMessageIe networkBeaconMessageIe;
    networkBeaconMessageIe.SetTxPowerIncluded(false);
    networkBeaconMessageIe.SetPowerConstraints(false);
    networkBeaconMessageIe.SetCurrentClusterChannelIncluded(false);
    networkBeaconMessageIe.SetNetworkBeaconChannels(0);
    networkBeaconMessageIe.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS);
    networkBeaconMessageIe.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS);
    networkBeaconMessageIe.SetNextClusterChannel(1657);
    networkBeaconMessageIe.SetTimeToNext(0);

    // Packet size should be 0
    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");

    packet->AddHeader(networkBeaconMessageIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 8, "Should be 8");
    NS_TEST_ASSERT_MSG_EQ(networkBeaconMessageIe.GetSerializedSize(), 8, "Should be 8");

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(false); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    packet->AddHeader(mxHeaderTypeA);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 9, "Should be 9");
    NS_TEST_ASSERT_MSG_EQ(mxHeaderTypeA.GetSerializedSize(), 1, "Should be 1");

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(0xF);
    beaconHeader.SetTransmitterAddress(0xF);

    packet->AddHeader(beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 16, "Should be 16");
    NS_TEST_ASSERT_MSG_EQ(beaconHeader.GetSerializedSize(), 7, "Should be 7");

    // MAC Header Type
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    packet->AddHeader(macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 17, "Should be 17");
    NS_TEST_ASSERT_MSG_EQ(macHeaderType.GetSerializedSize(), 1, "Should be 1");

    // Physical Header Field
    Dect2020ExtPhyControlFieldType1 physicalHeaderField;

    packet->AddHeader(physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 22, "Should be 22");
    NS_TEST_ASSERT_MSG_EQ(physicalHeaderField.GetSerializedSize(), 5, "Should be 5");

    std::cout << "The whole size of the message is " << packet->GetSize() << " bytes" << "\n";

    // 2. Here transmission happens

    // 3. Extract all headers from the packet according to the MAC layer's functionality

    // Physical Header Field
    Dect2020ExtPhyControlFieldType1 r_physicalHeaderField;
    packet->RemoveHeader(r_physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 17, "Should be 17");

    // MAC Header Type

    Dect2020ExtMacHeaderType r_macHeaderType;
    packet->RemoveHeader(r_macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 16, "Should be 16");

    // Beacon Header
    
    Dect2020ExtBeaconHeader r_beaconHeader;
    packet->RemoveHeader(r_beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 9, "Should be 9");

    // MAC Multiplexing Header

    Dect2020ExtMacMultiplexingHeaderTypeA r_macMxHeader;
    packet->RemoveHeader(r_macMxHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 8, "Should be 8");

    // Network Beacon Message IE

    Dect2020ExtNetworkBeaconMessageIe r_networkBeaconMessageIe;
    packet->RemoveHeader(r_networkBeaconMessageIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");
}

} // namespace ns3