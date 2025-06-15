#include "serialization-test-case.h"
#include "ns3/log.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE("Dect2020MacMultiplexingHeaderTypeCSerializationTestCase");

namespace ns3
{

typedef Dect2020MacMultiplexingHeaderTypeC::ExtendedIEType ExtendedIEType;

Dect2020MacMultiplexingHeaderTypeCSerializationTestCase::Dect2020MacMultiplexingHeaderTypeCSerializationTestCase()
    : TestCase("Dect2020MacMultiplexingHeaderTypeCSerializationTestCase")
{
}

void
Dect2020MacMultiplexingHeaderTypeCSerializationTestCase::DoSetup()
{
}

void
Dect2020MacMultiplexingHeaderTypeCSerializationTestCase::DoRun()
{
	Packet packet;
	Dect2020MacMultiplexingHeaderTypeC originalMxHeader;
	Dect2020MacMultiplexingHeaderTypeC deserializedMxHeader;
	Dect2020BeaconHeader* payload = new Dect2020BeaconHeader();

	// Set up the beacon header (payload)
	payload->SetNetworkId(0xFFFFFF);
	payload->SetTransmitterAddress(0xFFFFFFFF);

	// Set up the MAC Mx Header
	originalMxHeader.SetType(Dect2020MacMultiplexingHeaderTypeC::Type::C);
	originalMxHeader.SetMacExtension(0x00);
	originalMxHeader.SetIEType(ExtendedIEType::ClusterBeaconMessage);
	originalMxHeader.AddPayload(payload, payload->GetSerializedSize());

	// Serialize the header
	packet.AddHeader(originalMxHeader);

	// Deserialize the header
	packet.RemoveHeader(deserializedMxHeader);

	// Check if all values of the MAC Mx Header are the same
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetType(), originalMxHeader.GetType(), "Type mismatch");
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetMacExtension(), originalMxHeader.GetMacExtension(), "MacExtension mismatch");
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetIEType(), originalMxHeader.GetIEType(), "IEType mismatch");
}

} // namespace ns3
