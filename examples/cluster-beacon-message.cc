#include "ns3/dect2020-mac-pdu-helper.h"
#include "ns3/dect2020-mac-multiplexing-header-type-c.h"
#include "ns3/dect2020-cluster-beacon-message-ie.h"

#include "ns3/pointer.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ClusterBeaconMessageExample");

int
main(int argc, char* argv[])
{
    Dect2020MacPduHelper macPduHelper;
    Dect2020MacMultiplexingHeaderTypeC mxHeader;
    Dect2020ClusterBeaconMessageIE clusterBeaconMessageIE;

    // Creating MAC PDU (ns3::Packet) with:
    // MAC Header Type and MAC Common Header as Beacon Header
    Ptr<Packet> macPdu = macPduHelper.BEACON(
        MacSecurityField::MAC_SECURITY_NOT_USED,
        0xFFFFFFFF,
        0xFFFFFFFF
    );

    // Setting values for Cluster Beacon Message IE
    clusterBeaconMessageIE.SetSFN(42);                    // System Frame Number
    clusterBeaconMessageIE.SetPowerConst(1);              // Power constant flag
    clusterBeaconMessageIE.SetFO(0);                      // Frame Offset flag
    clusterBeaconMessageIE.SetNextChannel(1);             // Next channel flag
    clusterBeaconMessageIE.SetTimeToNextBit(1);           // 1-bit time to next
    clusterBeaconMessageIE.SetNetworkBeaconPeriod(8);     // Network beacon period (4 bits)
    clusterBeaconMessageIE.SetClusterBeaconPeriod(4);     // Cluster beacon period (4 bits)
    clusterBeaconMessageIE.SetCountToTrigger(3);          // Count to trigger (4 bits)
    clusterBeaconMessageIE.SetRelQuality(2);              // Relative quality (2 bits)
    clusterBeaconMessageIE.SetMinQuality(1);              // Minimum quality (2 bits)
    clusterBeaconMessageIE.SetClusterMaxTXPower(12);      // Cluster max TX power (4 bits)
    clusterBeaconMessageIE.SetFrameOffset(128);           // Frame offset (1 byte)
    clusterBeaconMessageIE.SetNextClusterChannel(2047);   // Next cluster channel (13 bits)
    clusterBeaconMessageIE.SetTimeToNext(1000000);        // 4-byte time to next

    // Setting up the multiplexing header
    mxHeader.SetMacExtension(0x00); // 00 = No length is included in the IE Header. The IE type defines the length of the IE payload.
    mxHeader.SetIEType(Dect2020MacMultiplexingHeader::ExtendedIEType::ClusterBeaconMessage);
    mxHeader.AddPayload(&clusterBeaconMessageIE, clusterBeaconMessageIE.GetSerializedSize());

    // Inserting the multiplexing header together with the payload into the MAC PDU
    macPdu->AddHeader(mxHeader); 

    return 0;
}