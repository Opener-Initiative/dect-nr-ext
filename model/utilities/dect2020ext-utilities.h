#include "ns3/net-device-container.h"
#include "ns3/mobility-module.h"
#include "ns3/node-container.h"
#include "ns3/double.h"

namespace ns3
{

void CreatePythonConnectionGraph(NetDeviceContainer devices);
void InstallCircularMobilityModel(uint32_t index, uint32_t totalNodes, Ptr<Node> node, Ptr<UniformRandomVariable> randomRadius, Vector center = Vector(0.0, 0.0, 0.0));
void InstallCenterMobilityModel(Ptr<Node> node);
void InstallConstantMobilityModel(Ptr<Node> node, Vector position);
void OutputSelectedOperatingChannels(NetDeviceContainer devices);

} // namespace ns3