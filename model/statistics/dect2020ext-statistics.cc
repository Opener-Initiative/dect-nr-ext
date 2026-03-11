#include "dect2020ext-statistics.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtStatistics");

std::map<std::string, int> Dect2020ExtStatistics::m_droppedPackets;

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

} // namespace ns3