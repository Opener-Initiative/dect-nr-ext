#ifndef SIMULATION_H
#define SIMULATION_H

// Configuration
#include "configuration.h"

// Nodes
#include "ns3/node-container.h"

// Channel
#include "dect-channel.h"

// PHY
#include "dect-phy.h"

#include <string>

// One-to-many WiFi Simulation class
class Simulation : public Configuration
{
    public:
        Simulation();
        ~Simulation();

        ns3::NodeContainer CreateNodes(int quantity);
        DectChannel CreateChannel();
        DectPhy CreatePhy();

};

#endif //SIMULATION_H