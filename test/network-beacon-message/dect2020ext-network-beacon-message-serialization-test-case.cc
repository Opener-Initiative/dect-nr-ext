#include "dect2020ext-network-beacon-message-serialization-test-case.h"

#include "ns3/dect2020ext-network-beacon-message-ie.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-mac.h"

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

    // Network Beacon Message IE
    Dect2020ExtNetworkBeaconMessageIE networkBeaconMessageIE;
    networkBeaconMessageIE.SetTxPowerIncluded(false);
    networkBeaconMessageIE.SetPowerConstraints(false);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(false);
    networkBeaconMessageIE.SetNetworkBeaconChannels(0);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    networkBeaconMessageIE.SetNextClusterChannel(1657);
    networkBeaconMessageIE.SetTimeToNext(0);

    // Packet size should be 0
    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");

    packet->AddHeader(networkBeaconMessageIE);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 8, "Should be 8");

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    packet->AddHeader(mxHeaderTypeA);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 9, "Should be 9");

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(24);
    beaconHeader.SetTransmitterAddress(25);

    packet->AddHeader(beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 16, "Should be 16");

    // MAC Header Type
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    packet->AddHeader(macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 17, "Should be 17");

    // Physical Header Field
    Dect2020ExtPhyControlFieldType1 physicalHeaderField;

    packet->AddHeader(physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 22, "Should be 22");

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

    Dect2020ExtNetworkBeaconMessageIE r_networkBeaconMessageIE;
    packet->RemoveHeader(r_networkBeaconMessageIE);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");
}

} // namespace ns3