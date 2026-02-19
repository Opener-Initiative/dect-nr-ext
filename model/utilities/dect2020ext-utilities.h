#include "ns3/net-device-container.h"
#include "ns3/mobility-module.h"
#include "ns3/node-container.h"
#include "ns3/double.h"

namespace ns3
{

void CreatePythonConnectionGraph(NetDeviceContainer devices);
void InstallCircularMobilityModel(uint32_t index, Ptr<Node> node, Ptr<NodeContainer> nodeContainer, Ptr<UniformRandomVariable> randomRadius);
void InstallCenterMobilityModel(Ptr<Node> node);

} // namespace ns3