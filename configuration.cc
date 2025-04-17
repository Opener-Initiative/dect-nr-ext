#include "configuration.h"
#include <iostream>

StringMap channelMap
{
    {"PropagationLoss", "ns3::LogDistancePropagationLossModel"},
    {"PropagationDelay", "ns3::ConstantSpeedPropagationDelayModel"}
};

StringMap phyMap
{
    {"ErrorRateModel", "ns3::TableBasedErrorRateModel"}
};

Configuration::Configuration()
{
    Config["Channel"] = channelMap;
    Config["Phy"] = phyMap;
}

Configuration::~Configuration(){}

void
Configuration::PrintConfigMap()
{
    for (const auto& outerPair : Config)
    {
        std::cout << outerPair.first << ":\n";
        for (const auto& innerPair : outerPair.second)
        {
            std::cout << "  " << innerPair.first << " : " << innerPair.second << std::endl;
        }
    }
}