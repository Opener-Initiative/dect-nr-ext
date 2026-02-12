#include "ns3/dect2020ext-execution-grid.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/node.h"
#include "ns3/mobility-module.h"
#include "ns3/net-device-container.h"
#include "ns3/isotropic-antenna-model.h"

#include <fstream>
#include <string>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExample");

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

int
main(int argc, char *argv[])
{
    // LogComponentEnable("Dect2020ExtExecutionGrid", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtInterference", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    // LogComponentEnable("SingleModelSpectrumChannel", LOG_LEVEL_LOGIC);

    // Spectrum Channel
    Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();
    channel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());
    channel->AddPropagationLossModel(CreateObject<LogDistancePropagationLossModel>());

    // Channel Manager
    Ptr<Dect2020ExtChannelManager> channelManager = CreateObject<Dect2020ExtChannelManager>();
    channelManager->SetBandNumber(1);
    channelManager->SetSubcarrierScalingFactor(1);
    channelManager->SetFourierTransformScalingFactor(1);
    channelManager->SetSpectrumChannel(channel);
    channelManager->Configure();
    channelManager->InitializeChannels();

    NetDeviceContainer devices;

    // ################
    // ###### FT ######
    // ################

    Ptr<Node> ftNode = CreateObject<Node>();

    // FT PHY
    Ptr<Dect2020ExtPhy> ftPHY = CreateObject<Dect2020ExtPhy>();
    ftPHY->SetChannel(channel);
    channel->AddRx(ftPHY);

    // FT Execution Grid
    Ptr<Dect2020ExtExecutionGrid> ftExecutionGrid = CreateObject<Dect2020ExtExecutionGrid>();
    ftExecutionGrid->Initialize();

    // FT MAC
    Ptr<Dect2020ExtMac> ftMAC = CreateObject<Dect2020ExtMac>();
    ftMAC->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftMAC->SetPhy(ftPHY);
    ftMAC->SetExecutionGrid(ftExecutionGrid);

    ftPHY->SetMac(ftMAC);
    ftExecutionGrid->SetMac(ftMAC);

    // FT Net Device
    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::FT);
    ftNetDevice->SetMac(ftMAC);
    ftNetDevice->SetPhy(ftPHY);
    ftNetDevice->SetNode(ftNode);

    ftNode->AddDevice(ftNetDevice);
    ftPHY->SetDevice(ftNetDevice);

    // FT Antenna
    Ptr<IsotropicAntennaModel> ftAntenna = CreateObject<IsotropicAntennaModel>();
    DoubleValue ftRxGain = ftMAC->GetRxGainFromIndex(ftNetDevice->m_rxGain);
    ftAntenna->SetAttribute("Gain", ftRxGain);
    ftPHY->SetAntenna(ftAntenna);

    // FT Mobility Model
    MobilityHelper ftMobility;
    Ptr<ListPositionAllocator> ftPositionAlloc = CreateObject<ListPositionAllocator>();
    ftPositionAlloc->Add(Vector(0.0, 0.0, 0.0));
    ftMobility.SetPositionAllocator(ftPositionAlloc);
    ftMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    ftMobility.Install(ftNode);
    ftPHY->SetMobility(ftNode->GetObject<MobilityModel>());

    ftMAC->InitializeDevice();

    devices.Add(ftNetDevice);

    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtMac::Start, ftMAC);
    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtPhy::Start, ftPHY);
    Simulator::ScheduleWithContext(ftNode->GetId(), Seconds(0), &Dect2020ExtExecutionGrid::StartExecutionGrid, ftExecutionGrid);

    // #################
    // ###### PTs ######
    // #################

    NodeContainer ptNodes;
    ptNodes.Create(100);
    Ptr<UniformRandomVariable> randomRadius = CreateObject<UniformRandomVariable>();
    randomRadius->SetAttribute("Min", DoubleValue(50.0));
    randomRadius->SetAttribute("Max", DoubleValue(150.0));

    for (uint32_t i = 0; i < ptNodes.GetN(); ++i)
    {
        // PT PHY
        Ptr<Dect2020ExtPhy> ptPHY = CreateObject<Dect2020ExtPhy>();
        ptPHY->SetChannel(channel);

        // PT Execution Grid
        Ptr<Dect2020ExtExecutionGrid> ptExecutionGrid = CreateObject<Dect2020ExtExecutionGrid>();
        ptExecutionGrid->Initialize();

        // PT MAC
        Ptr<Dect2020ExtMac> ptMAC = CreateObject<Dect2020ExtMac>();
        ptMAC->SetPhy(ptPHY);
        ptMAC->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptMAC->SetExecutionGrid(ptExecutionGrid);

        ptPHY->SetMac(ptMAC);
        ptExecutionGrid->SetMac(ptMAC);

        // PT Net Device
        Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
        ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::PT);
        ptNetDevice->SetMac(ptMAC);
        ptNetDevice->SetPhy(ptPHY);
        ptNetDevice->SetNode(ptNodes.Get(i));

        ptNodes.Get(i)->AddDevice(ptNetDevice);
        ptPHY->SetDevice(ptNetDevice);
        channel->AddRx(ptPHY);

        // PT Antenna
        Ptr<IsotropicAntennaModel> ptAntenna = CreateObject<IsotropicAntennaModel>();
        DoubleValue ptRxGain = ptMAC->GetRxGainFromIndex(ptNetDevice->m_rxGain);
        ptAntenna->SetAttribute("Gain", ptRxGain);
        ptPHY->SetAntenna(ptAntenna);

        // PT Mobility Model - CIRCULAR PLACEMENT
        MobilityHelper ptMobility;
        Ptr<ListPositionAllocator> ptPositionAlloc = CreateObject<ListPositionAllocator>();

        // Calculate angle for this PT (evenly distributed around circle)
        double angle = (2.0 * M_PI * i) / ptNodes.GetN(); // radians
        double radius = randomRadius->GetValue();
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);

        ptPositionAlloc->Add(Vector(x, y, 0.0));

        ptMobility.SetPositionAllocator(ptPositionAlloc);
        ptMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

        ptMobility.Install(ptNodes.Get(i));
        ptPHY->SetMobility(ptNodes.Get(i)->GetObject<MobilityModel>());

        ptMAC->InitializeDevice();

        devices.Add(ptNetDevice);

        Simulator::ScheduleWithContext(ptNodes.Get(i)->GetId(), Seconds(0), &Dect2020ExtMac::Start, ptMAC);
        Simulator::ScheduleWithContext(ptNodes.Get(i)->GetId(), Seconds(0), &Dect2020ExtPhy::Start, ptPHY);
        Simulator::ScheduleWithContext(ptNodes.Get(i)->GetId(), Seconds(0), &Dect2020ExtExecutionGrid::StartExecutionGrid, ptExecutionGrid);
    }

    Simulator::Schedule(Seconds(5.9), &CreatePythonConnectionGraph, devices);

    Simulator::Stop(Seconds(6));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}