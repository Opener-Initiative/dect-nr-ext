#include "simulation.h"
#include <iostream>

Simulation::Simulation(){}

Simulation::~Simulation(){}

ns3::NodeContainer
Simulation::CreateNodes(int quantity)
{
    ns3::NodeContainer container;
    container.Create(quantity);
    return container;
}

DectChannel
Simulation::CreateChannel()
{
    DectChannel channel;
    channel.SetPropagationDelay(Config["Channel"]["PropagationDelay"]);
    channel.AddPropagationLoss(Config["Channel"]["PropagationLoss"]);
    return channel;
}

DectPhy
Simulation::CreatePhy()
{
    DectPhy phy;
    phy.SetErrorRateModel(Config["Phy"]["ErrorRateModel"]);
    return phy;
}