#include "dect2020ext-statistics.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtStatistics");

std::map<std::string, int> Dect2020ExtStatistics::m_droppedPackets;
std::map<std::string, int> Dect2020ExtStatistics::m_acceptedPackets;

Dect2020ExtStatistics::Dect2020ExtStatistics()
{
}

Dect2020ExtStatistics::~Dect2020ExtStatistics()
{
}

void
Dect2020ExtStatistics::OutputDroppedPackets()
{
    for (auto it = m_droppedPackets.begin(); it != m_droppedPackets.end(); it++)
    {
        NS_LOG_INFO(it->first << " dropped " << it->second << " packets");
    }
}

void
Dect2020ExtStatistics::OutputAcceptedPackets()
{    
    for (auto it = m_acceptedPackets.begin(); it != m_acceptedPackets.end(); it++)
    {
        NS_LOG_INFO(it->first << " accepted " << it->second << " packets");
    }
}

} // namespace ns3