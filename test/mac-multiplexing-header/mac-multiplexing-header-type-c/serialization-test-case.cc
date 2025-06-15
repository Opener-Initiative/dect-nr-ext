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
	Dect2020BeaconHeader* originalPayload = new Dect2020BeaconHeader();
	Dect2020BeaconHeader* deserializedPayload = new Dect2020BeaconHeader();

	// Set up the beacon header (payload)
	originalPayload->SetNetworkId(0xF0F0FF);
	originalPayload->SetTransmitterAddress(0xF0F0FFFF);

	// Set up the original MAC Mx Header
	originalMxHeader.SetType(Dect2020MacMultiplexingHeaderTypeC::Type::C);
	originalMxHeader.SetMacExtension(0x00);
	originalMxHeader.SetIEType(ExtendedIEType::ClusterBeaconMessage);
	originalMxHeader.AddPayload(originalPayload, originalPayload->GetSerializedSize());

	// Set up the deserialized MAC Mx Header
	deserializedMxHeader.SetType(Dect2020MacMultiplexingHeaderTypeC::Type::C);
	deserializedMxHeader.AddPayload(deserializedPayload, deserializedPayload->GetSerializedSize());

	// Serialize the header
	packet.AddHeader(originalMxHeader);

	// Deserialize the header
	packet.RemoveHeader(deserializedMxHeader);

	// Check if all values of the MAC Mx Header are the same
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetMacExtension(), originalMxHeader.GetMacExtension(), "MacExtension mismatch");
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetIEType(), originalMxHeader.GetIEType(), "IEType mismatch");

	// Check if the payload is the same
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetPayloadAs<Dect2020BeaconHeader>()->GetNetworkId(), originalPayload->GetNetworkId(), "NetworkId mismatch");
	NS_TEST_ASSERT_MSG_EQ(deserializedMxHeader.GetPayloadAs<Dect2020BeaconHeader>()->GetTransmitterAddress(), originalPayload->GetTransmitterAddress(), "TransmitterAddress mismatch");
}

} // namespace ns3
