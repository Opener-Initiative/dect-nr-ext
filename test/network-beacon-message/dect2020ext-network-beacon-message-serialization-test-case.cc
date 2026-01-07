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
    // 1. Build a Network Beacon Message
    Ptr<Packet> networkBeacon = Create<Packet>();

    // Network Beacon Message IE
    Dect2020ExtNetworkBeaconMessageIE networkBeaconMessageIE;
    networkBeaconMessageIE.SetTxPowerIncluded(1);
    networkBeaconMessageIE.SetPowerConstraints(0);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(0);
    networkBeaconMessageIE.SetNetworkBeaconChannels(0);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::NETWORK_BEACON_PERIOD_50MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020ExtNetworkBeaconMessageIE::CLUSTER_BEACON_PERIOD_100MS);
    networkBeaconMessageIE.SetNextClusterChannel(1657);

    // Time timeToNext = m_nextNetworkBeaconTime - Simulator::Now();
    // networkBeaconMessageIE.SetTimeToNext(timeToNext.GetMilliSeconds());

    networkBeacon->AddHeader(networkBeaconMessageIE);

    Dect2020ExtMacMultiplexingHeaderTypeA mxHeaderTypeA; // Short SDU, no payload
    mxHeaderTypeA.SetMacExtensionFieldEncoding(0); // No length field included in the IE Header
    mxHeaderTypeA.SetLengthField(0); // No length field included in the IE Header
    mxHeaderTypeA.SetIeTypeFieldEncoding(Dect2020ExtMacMultiplexingHeaderTypeA::Dect2020ExtIETypeFieldEncoding::NETWORK_BEACON_MESSAGE);

    networkBeacon->AddHeader(mxHeaderTypeA);

    Dect2020ExtBeaconHeader beaconHeader;
    beaconHeader.SetNetworkId(24);
    beaconHeader.SetTransmitterAddress(25);

    networkBeacon->AddHeader(beaconHeader);

    Dect2020ExtMacHeaderType macHeaderType;
    macHeaderType.SetMacHeaderTypeField(Dect2020ExtMacHeaderType::MacHeaderTypeField::BEACON_HEADER);

    networkBeacon->AddHeader(macHeaderType);
}

} // namespace ns3