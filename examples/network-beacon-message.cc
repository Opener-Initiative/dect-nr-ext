#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/dect2020-network-beacon-message-ie.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("NetworkBeaconMessageExample");

int
main(int argc, char* argv[])
{
    Dect2020MacPduHelper macPduHelper;
    Dect2020MacMultiplexingHeaderTypeC mxHeader;
    Dect2020NetworkBeaconMessageIE networkBeaconMessageIE;

    // Creating MAC PDU (ns3::Packet) with:
    // MAC Header Type and MAC Common Header as Beacon Header
    Ptr<Packet> macPdu = macPduHelper.BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );

    // Setting values for Network Beacon Message IE
    networkBeaconMessageIE.SetTxPowerIncluded(true);
    networkBeaconMessageIE.SetPowerConstraints(false);
    networkBeaconMessageIE.SetCurrentClusterChannelIncluded(true);
    networkBeaconMessageIE.SetNetworkBeaconChannels(2);
    networkBeaconMessageIE.SetNetworkBeaconPeriod(Dect2020NetworkBeaconMessageIE::NETWORK_PERIOD_1000MS);
    networkBeaconMessageIE.SetClusterBeaconPeriod(Dect2020NetworkBeaconMessageIE::CLUSTER_PERIOD_500MS);
    networkBeaconMessageIE.SetNextClusterChannel(1234);
    networkBeaconMessageIE.SetTimeToNext(50000); // 50ms in microseconds
    networkBeaconMessageIE.SetClustersMaxTxPower(20); // 20 dBm
    networkBeaconMessageIE.SetCurrentClusterChannel(5678);
    
    // Set additional network beacon channels
    uint16_t additionalChannels[3] = {100, 200, 300};
    networkBeaconMessageIE.SetAdditionalNetworkBeaconChannels(additionalChannels);

    // Setting up the multiplexing header
    mxHeader.SetMacExtension(0x00); // 00 = No length is included in the IE Header. The IE type defines the length of the IE payload.
    mxHeader.SetIEType(Dect2020MacMultiplexingHeader::ExtendedIEType::NetworkBeaconMessage);
    mxHeader.AddPayload(&networkBeaconMessageIE, networkBeaconMessageIE.GetSerializedSize());

    // Inserting the multiplexing header together with the payload into the MAC PDU
    macPdu->AddHeader(mxHeader); 

    return 0;
}