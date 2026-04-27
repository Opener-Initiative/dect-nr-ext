#include "dect2020ext-cluster-beacon-message-serialization-test-case.h"

#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-mac.h"

#include "ns3/ptr.h"
#include "ns3/packet.h"

namespace ns3
{

Dect2020ExtClusterBeaconMessageSerializationTestCase::Dect2020ExtClusterBeaconMessageSerializationTestCase()
    : TestCase("Dect2020ExtClusterBeaconMessageSerializationTestCase")
{
}

Dect2020ExtClusterBeaconMessageSerializationTestCase::~Dect2020ExtClusterBeaconMessageSerializationTestCase()
{
}

void
Dect2020ExtClusterBeaconMessageSerializationTestCase::DoSetup()
{
}

void
Dect2020ExtClusterBeaconMessageSerializationTestCase::DoRun()
{
    // 1. Build Network Beacon Message

    Ptr<Packet> packet = Create<Packet>();

    // Cluster Beacon Message IE
    Dect2020ExtClusterBeaconMessageIE clusterBeaconMessageIE;
    clusterBeaconMessageIE.SetSFN(15);
    clusterBeaconMessageIE.SetTxPowerIncluded(0);
    clusterBeaconMessageIE.SetPowerConstraints(0);
    clusterBeaconMessageIE.SetFrameOffsetIncluded(0);
    clusterBeaconMessageIE.SetNextChannelIncluded(0);
    clusterBeaconMessageIE.SetTimeToNextFieldPresent(0);
    clusterBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    clusterBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    clusterBeaconMessageIE.SetCountToTrigger(15);
    clusterBeaconMessageIE.SetRelQuality(1);
    clusterBeaconMessageIE.SetMinQuality(1);

    // Packet size should be 0
    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");

    packet->AddHeader(clusterBeaconMessageIE);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 4, "Should be 8");

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    packet->AddHeader(mxHeaderTypeA);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 5, "Should be 9");

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(24);
    beaconHeader.SetTransmitterAddress(25);

    packet->AddHeader(beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 12, "Should be 16");

    // MAC Header Type
    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    packet->AddHeader(macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 13, "Should be 17");

    // Physical Header Field
    Dect2020ExtPhyControlFieldType1 physicalHeaderField;

    packet->AddHeader(physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 18, "Should be 22");


    // 2. Here transmission happens

    // 3. Extract all headers from the packet according to the MAC layer's functionality

    // Physical Header Field
    Dect2020ExtPhyControlFieldType1 r_physicalHeaderField;
    packet->RemoveHeader(r_physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 13, "Should be 17");

    // MAC Header Type

    Dect2020ExtMacHeaderType r_macHeaderType;
    packet->RemoveHeader(r_macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 12, "Should be 16");

    // Beacon Header
    
    Dect2020ExtBeaconHeader r_beaconHeader;
    packet->RemoveHeader(r_beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 5, "Should be 9");

    // MAC Multiplexing Header

    Dect2020ExtMacMultiplexingHeaderTypeA r_macMxHeader;
    packet->RemoveHeader(r_macMxHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 4, "Should be 8");

    // Cluster Beacon Message IE

    Dect2020ExtClusterBeaconMessageIE r_networkBeaconMessageIE;
    packet->RemoveHeader(r_networkBeaconMessageIE);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");
    std::cout << packet->GetSize();
}

} // namespace ns3