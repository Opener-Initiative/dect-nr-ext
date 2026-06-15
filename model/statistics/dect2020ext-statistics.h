#ifndef DECT2020EXT_STATISTICS_H
#define DECT2020EXT_STATISTICS_H

#include <map>
#include <string>

namespace ns3
{

class Dect2020ExtStatistics
{
    public:
        Dect2020ExtStatistics();
        ~Dect2020ExtStatistics();

        static void OutputDroppedPackets();
        static void OutputAcceptedPackets();
        static void OutputAllStatistics();

        // ###### Packets in the PHY layer ######

        // Dropped packets at the PHY layer due to collisions and other factors  
        // Used in Dect2020ExtPhy::StartRx
        static std::map<std::string, int> m_droppedPacketsPhy;

        // Accepted packets at the PHY layer  
        // Used in Dect2020ExtPhy::StartRx
        static std::map<std::string, int> m_acceptedPacketsPhy;

        // Transmitted packets at the PHY layer  
        // Used in Dect2020ExtPhy::Send
        static std::map<std::string, int> m_transmittedPacketsPhy;

        // ###### Packets in the MAC layer ######

        // Dropped Network Beacon Messages by FTs due to collisions with beacons
        // and packets from other FTs and PTs  
        // Used in Dect2020ExtMac::SendNetworkBeaconOnChannel
        static std::map<std::string, int> m_droppedNetworkBeaconsFtMac;

        // Transmitted Network Beacon Messages by FTs  
        // Used in Dect2020ExtMac::SendNetworkBeaconOnChannel
        static std::map<std::string, int> m_transmittedNetworkBeaconsFtMac;

        // Dropped Cluster Beacon Messages due to collisions with beacons
        // and packets from other FTs and PTs  
        // Used in Dect2020ExtMac::SendClusterBeaconOnChannel
        static std::map<std::string, int> m_droppedClusterBeaconsFtMac;

        // Transmitted Cluster Beacon Messages by FTs  
        // Used in Dect2020ExtMac::SendClusterBeaconOnChannel
        static std::map<std::string, int> m_transmittedClusterBeaconsFtMac;

        // Accepted Network Beacon Messages by PTs  
        // Used in Dect2020ExtMac::HandleBeaconPacket
        static std::map<std::string, int> m_acceptedNetworkBeaconsPtMac;

        // Accepted Cluster Beacon Messages by PTs
        // Used in Dect2020ExtMac::HandleBeaconPacket
        static std::map<std::string, int> m_acceptedClusterBeaconsPtMac;

        // Transmitted Association Request Messages by PTs  
        // Used in Dect2020ExtMac::EvaluateLbtChannel
        static std::map<std::string, int> m_transmittedAssociationRequestsPtMac;

        // Accepted Association Request Messges by FTs  
        // Used in Dect2020ExtMac::HandleUnicastPacket
        static std::map<std::string, int> m_acceptedAssociationRequestsFtMac;

        // Dropped Association Response Messages by FTs due to collisions with 
        // packets from other FTs and PTs  
        // Used in Dect2020ExtMac::SendAssociationResponse
        static std::map<std::string, int> m_droppedAssociationResponsesFtMac;

        // Transmitted Association Response Messages by FTs  
        // Used in Dect2020ExtMac::SendAssociationResponse
        static std::map<std::string, int> m_transmittedAssociationResponsesFtMac;

        // Accepted Association Response Messges by PTs  
        // Used in Dect2020ExtMac::VerifyHarqAck
        static std::map<std::string, int> m_acceptedAssociationResponsesPtMac;

        // Transmitted DATA MAC PDU packets by PTs  
        // Used in Dect2020ExtMac::EvaluateLbtChannel
        static std::map<std::string, int> m_transmittedDataPacketsPtMac;

        // Acknowledged DATA MAC PDU packets by FTs  
        // Used in Dect2020ExtMac::VerifyHarqAck
        static std::map<std::string, int> m_acknowledgedDataPacketsPtMac;
};

} // namespace ns3

#endif