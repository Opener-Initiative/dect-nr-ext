#include "dect2020ext-statistics.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtStatistics");

// PHY
std::map<std::string, int> Dect2020ExtStatistics::m_droppedPacketsPhy;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedPacketsPhy;
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedPacketsPhy;

// MAC - Network Beacons
std::map<std::string, int> Dect2020ExtStatistics::m_droppedNetworkBeaconsFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedNetworkBeaconsFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedNetworkBeaconsPtMac;

// MAC - Cluster Beacons
std::map<std::string, int> Dect2020ExtStatistics::m_droppedClusterBeaconsFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedClusterBeaconsFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedClusterBeaconsPtMac;

// MAC - Association
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedAssociationRequestsPtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedAssociationRequestsFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_droppedAssociationResponsesFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedAssociationResponsesFtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedAssociationResponsesPtMac;

// MAC - Data
std::map<std::string, int> Dect2020ExtStatistics::m_transmittedDataPacketsPtMac;
std::map<std::string, int> Dect2020ExtStatistics::m_acknowledgedDataPacketsPtMac;

Dect2020ExtStatistics::Dect2020ExtStatistics()
{
}

Dect2020ExtStatistics::~Dect2020ExtStatistics()
{
}

void
Dect2020ExtStatistics::OutputDroppedPackets()
{
    NS_LOG_INFO("--- Dropped packets (PHY) ---");
    for (auto& [key, val] : m_droppedPacketsPhy)
        NS_LOG_INFO(key << " dropped " << val << " packets (PHY)");

    NS_LOG_INFO("--- Dropped packets (MAC) ---");
    for (auto& [key, val] : m_droppedNetworkBeaconsFtMac)
        NS_LOG_INFO(key << " dropped " << val << " network beacons (FT MAC)");
    for (auto& [key, val] : m_droppedClusterBeaconsFtMac)
        NS_LOG_INFO(key << " dropped " << val << " cluster beacons (FT MAC)");
    for (auto& [key, val] : m_droppedAssociationResponsesFtMac)
        NS_LOG_INFO(key << " dropped " << val << " association responses (FT MAC)");
}

void
Dect2020ExtStatistics::OutputAcceptedPackets()
{
    NS_LOG_INFO("--- Accepted packets (PHY) ---");
    for (auto& [key, val] : m_acceptedPacketsPhy)
        NS_LOG_INFO(key << " accepted " << val << " packets (PHY)");

    NS_LOG_INFO("--- Accepted packets (MAC) ---");
    for (auto& [key, val] : m_acceptedNetworkBeaconsPtMac)
        NS_LOG_INFO(key << " accepted " << val << " network beacons (PT MAC)");
    for (auto& [key, val] : m_acceptedClusterBeaconsPtMac)
        NS_LOG_INFO(key << " accepted " << val << " cluster beacons (PT MAC)");
    for (auto& [key, val] : m_acceptedAssociationRequestsFtMac)
        NS_LOG_INFO(key << " accepted " << val << " association requests (FT MAC)");
}

void
Dect2020ExtStatistics::OutputAllStatistics()
{
    NS_LOG_INFO("========== PHY Layer Statistics ==========");
    for (auto& [key, val] : m_transmittedPacketsPhy)
        NS_LOG_INFO(key << " transmitted " << val << " packets (PHY)");
    for (auto& [key, val] : m_droppedPacketsPhy)
        NS_LOG_INFO(key << " dropped " << val << " packets (PHY)");
    for (auto& [key, val] : m_acceptedPacketsPhy)
        NS_LOG_INFO(key << " accepted " << val << " packets (PHY)");

    NS_LOG_INFO("========== MAC Layer Statistics ==========");

    NS_LOG_INFO("--- Network Beacons ---");
    for (auto& [key, val] : m_transmittedNetworkBeaconsFtMac)
        NS_LOG_INFO(key << " transmitted " << val << " network beacons (FT MAC)");
    for (auto& [key, val] : m_droppedNetworkBeaconsFtMac)
        NS_LOG_INFO(key << " dropped " << val << " network beacons (FT MAC)");
    for (auto& [key, val] : m_acceptedNetworkBeaconsPtMac)
        NS_LOG_INFO(key << " accepted " << val << " network beacons (PT MAC)");

    NS_LOG_INFO("--- Cluster Beacons ---");
    for (auto& [key, val] : m_transmittedClusterBeaconsFtMac)
        NS_LOG_INFO(key << " transmitted " << val << " cluster beacons (FT MAC)");
    for (auto& [key, val] : m_droppedClusterBeaconsFtMac)
        NS_LOG_INFO(key << " dropped " << val << " cluster beacons (FT MAC)");
    for (auto& [key, val] : m_acceptedClusterBeaconsPtMac)
        NS_LOG_INFO(key << " accepted " << val << " cluster beacons (PT MAC)");

    NS_LOG_INFO("--- Association ---");
    for (auto& [key, val] : m_transmittedAssociationRequestsPtMac)
        NS_LOG_INFO(key << " transmitted " << val << " association requests (PT MAC)");
    for (auto& [key, val] : m_acceptedAssociationRequestsFtMac)
        NS_LOG_INFO(key << " accepted " << val << " association requests (FT MAC)");
    for (auto& [key, val] : m_transmittedAssociationResponsesFtMac)
        NS_LOG_INFO(key << " transmitted " << val << " association responses (FT MAC)");
    for (auto& [key, val] : m_droppedAssociationResponsesFtMac)
        NS_LOG_INFO(key << " dropped " << val << " association responses (FT MAC)");
    for (auto& [key, val] : m_acceptedAssociationResponsesPtMac)
        NS_LOG_INFO(key << " accepted " << val << " association responses (PT MAC)");

    NS_LOG_INFO("--- Data ---");
    for (auto& [key, val] : m_transmittedDataPacketsPtMac)
        NS_LOG_INFO(key << " transmitted " << val << " data packets (but not necessairly acknowledged) (PT MAC)");
    for (auto& [key, val] : m_acknowledgedDataPacketsPtMac)
        NS_LOG_INFO(key << " " << val << " Data packets were acknowledged (PT MAC)");
}

} // namespace ns3