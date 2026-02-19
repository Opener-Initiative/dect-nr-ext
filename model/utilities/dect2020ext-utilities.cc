#include "dect2020ext-utilities.h"

#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-mac.h"


#include <map>
#include <fstream>

namespace ns3
{

void
CreatePythonConnectionGraph(NetDeviceContainer devices)
{
    int ftCounter = 0;
    int ptCounter = 0;
    std::map<uint64_t, std::string> longRdIdToName;

    std::ofstream topo("./contrib/dect-nr-ext/dect2020ext_topology.py");

    topo << "import networkx as nx\n";
    topo << "import matplotlib.pyplot as plt\n" << "\n";
    topo << "nodes = []\n";
    topo << "edges = []\n" << "\n";

    for (auto device = devices.Begin(); device != devices.End(); device++)
    {
        Ptr<Dect2020ExtNetDevice> dectDevice = DynamicCast<Dect2020ExtNetDevice>(*device);
        Ptr<Dect2020ExtMac> mac = dectDevice->GetMac();
        Ptr<MobilityModel> mobility = dectDevice->GetNode()->GetObject<MobilityModel>();
        Vector position = mobility->GetPosition();
        auto status = mac->GetAssociationStatus();
        std::string statusStr;

        switch (status)
        {
        case 0:
            statusStr = "NOT_ASSOCIATED";
            break;
        case 1:
            statusStr = "ASSOCIATION_PREPARING";
            break;
        case 2:
            statusStr = "WAITING_FOR_SELECTED_FT";
            break;
        case 3:
            statusStr = "ASSOCIATION_PENDING";
            break;
        case 4:
            statusStr = "ASSOCIATED";
            break;
        default:
            statusStr = "UNKNOWN";
            break;
        }

        std::string type = dectDevice->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT ? "FT" : "PT";
        std::string name;
        if (type == "FT")
        {
            name = "FT" + std::to_string(ftCounter);
            ftCounter++;
        }
        else
        {
            name = "PT" + std::to_string(ptCounter);
            ptCounter++;
        }

        longRdIdToName[mac->GetLongRadioDeviceId()] = name;

        topo << "nodes.append(('" << name << "', '" << type << "', '"
                << statusStr << "', " << position.x << ", " << position.y << "))\n";
            
        if (statusStr == "ASSOCIATED")
        {
            uint64_t ftId = mac->m_associatedFTNetDeviceLongRdId;
            std::string ftName = longRdIdToName[ftId];
            topo << "edges.append(('" << ftName << "', '" << name << "', {'power': " << mac->m_selectedFtCandidate.rssiDbm << "}))\n";
        }
    }

    topo << R"(
graph = nx.Graph()

for name, type, status, x, y in nodes:
    graph.add_node(name, type=type, status=status, position=(x, y))

graph.add_edges_from(edges)

position = {name: (x, y) for name, type, status, x, y in nodes}

plt.figure(figsize=(10, 6))
nx.draw(graph, position, with_labels=True, node_size=700, edge_color='red')

edge_labels = nx.get_edge_attributes(graph, 'power')
edge_labels = {k: f'{v:.1f} dBm' for k, v in edge_labels.items()}

nx.draw_networkx_edge_labels(graph, position, edge_labels)

plt.axis('on')
plt.grid(True)
plt.show()
)";

    topo.close();
}

void
InstallCircularMobilityModel(uint32_t index, Ptr<Node> node, Ptr<NodeContainer> nodeContainer, Ptr<UniformRandomVariable> randomRadius)
{        
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAllocator = CreateObject<ListPositionAllocator>();

    // Calculate angle for the node (evenly distributed around circle)
    double angle = (2.0 * M_PI * index) / nodeContainer->GetN(); // radians
    double radius = randomRadius->GetValue();
    double x = radius * std::cos(angle);
    double y = radius * std::sin(angle);        
    
    positionAllocator->Add(Vector(x, y, 0.0));        

    mobility.SetPositionAllocator(positionAllocator);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(node);
}

void
InstallCenterMobilityModel(Ptr<Node> node)
{
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAllocator = CreateObject<ListPositionAllocator>();
    positionAllocator->Add(Vector(0.0, 0.0, 0.0));
    mobility.SetPositionAllocator(positionAllocator);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(node);
}

} // namespace ns3