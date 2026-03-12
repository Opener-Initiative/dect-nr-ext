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

        static std::map<std::string, int> m_droppedPackets;
        static std::map<std::string, int> m_acceptedPackets;
};

} // namespace ns3

#endif