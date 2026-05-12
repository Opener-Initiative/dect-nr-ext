#include "dect2020ext-cluster-beacon-message-serialization-test-case.h"

#include "ns3/dect2020ext-cluster-beacon-message-ie.h"
#include "ns3/dect2020ext-random-access-resource-ie.h"
#include "ns3/dect2020ext-mac-multiplexing-header-type-a.h"
#include "ns3/dect2020ext-beacon-header.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"

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

    Dect2020ExtClusterBeaconMessageIe clusterBeaconMessageIe;
    clusterBeaconMessageIe.SetSFN(15);
    clusterBeaconMessageIe.SetTxPowerIncluded(false);
    clusterBeaconMessageIe.SetPowerConstraints(false);
    clusterBeaconMessageIe.SetFrameOffsetIncluded(false);
    clusterBeaconMessageIe.SetNextChannelIncluded(false);
    clusterBeaconMessageIe.SetTimeToNextFieldPresent(false);
    clusterBeaconMessageIe.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::NETWORK_BEACON_PERIOD_50MS);
    clusterBeaconMessageIe.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIe::CLUSTER_BEACON_PERIOD_100MS);
    clusterBeaconMessageIe.SetCountToTrigger(15);
    clusterBeaconMessageIe.SetRelQuality(1);
    clusterBeaconMessageIe.SetMinQuality(1);

    // Packet size should be 0
    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");

    packet->AddHeader(clusterBeaconMessageIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 4, "Should be 4");
    NS_TEST_ASSERT_MSG_EQ(clusterBeaconMessageIe.GetSerializedSize(), 4, "Should be 4");

    Dect2020ExtRandomAccessResourceIe randomAccessResourceIe;
    randomAccessResourceIe.SetRepeat(0);
    randomAccessResourceIe.SetSystemFrameNumberFieldIncluded(false);
    randomAccessResourceIe.SetChannelFieldIncluded(false);
    randomAccessResourceIe.SetSeparateChannelFieldIncluded(false);
    randomAccessResourceIe.SetStartSubslot(10);
    randomAccessResourceIe.SetLengthType(false); // length in subslots
    randomAccessResourceIe.SetRaraLength(50);
    randomAccessResourceIe.SetMaxRachLengthType(false); // length in subslots
    randomAccessResourceIe.SetMaxRachLength(4); // Max 4 subslots (2 slots) / transmission
    randomAccessResourceIe.SetCwMinSig(1);
    randomAccessResourceIe.SetDectDelay(false);
    randomAccessResourceIe.SetResponseWindow(5); // response window length in subslots -> value + 1 subslot
    randomAccessResourceIe.SetCwMaxSig(5);

    packet->AddHeader(randomAccessResourceIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 9, "Should be 9");
    NS_TEST_ASSERT_MSG_EQ(randomAccessResourceIe.GetSerializedSize(), 5, "Should be 5");

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(false); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::IeTypeFieldEncoding::CLUSTER_BEACON_MESSAGE);

    packet->AddHeader(mxHeaderTypeA);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 10, "Should be 10");
    NS_TEST_ASSERT_MSG_EQ(mxHeaderTypeA.GetSerializedSize(), 1, "Should be 1");

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(24);
    beaconHeader.SetTransmitterAddress(25);

    packet->AddHeader(beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 17, "Should be 17");
    NS_TEST_ASSERT_MSG_EQ(beaconHeader.GetSerializedSize(), 7, "Should be 7");

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    packet->AddHeader(macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 18, "Should be 18");
    NS_TEST_ASSERT_MSG_EQ(macHeaderType.GetSerializedSize(), 1, "Should be 1");

    Dect2020ExtPhyControlFieldType1 physicalHeaderField;

    packet->AddHeader(physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 23, "Should be 23");
    NS_TEST_ASSERT_MSG_EQ(physicalHeaderField.GetSerializedSize(), 5, "Should be 5");

    std::cout << "The whole size of the message is " << packet->GetSize() << " bytes" << "\n";

    // 2. Here transmission happens

    // 3. Extract all headers from the packet according to the MAC layer's functionality

    Dect2020ExtPhyControlFieldType1 r_physicalHeaderField;
    packet->RemoveHeader(r_physicalHeaderField);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 18, "Should be 18");

    Dect2020ExtMacHeaderType r_macHeaderType;
    packet->RemoveHeader(r_macHeaderType);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 17, "Should be 17");
    
    Dect2020ExtBeaconHeader r_beaconHeader;
    packet->RemoveHeader(r_beaconHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 10, "Should be 10");

    Dect2020ExtMacMultiplexingHeaderTypeA r_macMxHeader;
    packet->RemoveHeader(r_macMxHeader);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 9, "Should be 9");

    Dect2020ExtRandomAccessResourceIe r_randomAccessResourceIe;
    packet->RemoveHeader(r_randomAccessResourceIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 4, "Should be 4");

    Dect2020ExtClusterBeaconMessageIe r_networkBeaconMessageIe;
    packet->RemoveHeader(r_networkBeaconMessageIe);

    NS_TEST_ASSERT_MSG_EQ(packet->GetSize(), 0, "Should be 0");
}

} // namespace ns3