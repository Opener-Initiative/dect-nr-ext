#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/dect2020-beacon-header.h"
#include "ns3/dect2020-beacon-message.h"
#include "ns3/dect2020-mac-header-type.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/dect2020-net-device.h"
#include "ns3/dect2020-mac.h"
#include "ns3/dect2020-phy.h"
#include "ns3/dect2020-spectrum-signal-parameters.h"
#include "ns3/dect2020-physical-header-field.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;
using TermPointType = Dect2020NetDevice::TerminationPointType;

NS_LOG_COMPONENT_DEFINE("Dect2020Simulation");

// Empfangsfunktion definieren
bool
ReceivePacket(Ptr<NetDevice> device,
              Ptr<const Packet> packet,
              uint16_t protocol,
              const Address& sender)
{
    NS_LOG_UNCOND("Gerät 2 hat ein Paket empfangen von " << Mac48Address::ConvertFrom(sender));
    return true; // Paket wurde erfolgreich empfangen
}

int
main(int argc, char* argv[])
{
    // Enable packet metadata at the beginning
    PacketMetadata::Enable();
    
    Simulator::Stop(Seconds(10)); // Increased simulation time

    NS_LOG_INFO(Simulator::Now().GetMilliSeconds());

    // Protokollierung aktivieren
    /*
    LogComponentEnable("Dect2020NetDevice", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020Mac", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020Phy", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020Channel", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020Simulation", LOG_LEVEL_INFO);
    LogComponentEnable("Dect2020SpectrumSignalParameters", LOG_LEVEL_INFO);
    */

    // Erstellen der Knoten
    NodeContainer nodes;
    nodes.Create(2);
    
    // Add mobility models to nodes
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);
    
    // Set positions for the nodes
    nodes.Get(0)->GetObject<MobilityModel>()->SetPosition(Vector(0.0, 0.0, 0.0));
    nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(Vector(10.0, 0.0, 0.0));

    // Erstellen des Kanals
    Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();

    // Erstellen und Konfigurieren der Geräte
    NetDeviceContainer devices;
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        // Erstellen des NetDevice
        Ptr<Dect2020NetDevice> dev = CreateObject<Dect2020NetDevice>();

        if (i == 0)
        {
            TermPointType FT = TermPointType::FT;
            dev->SetTerminationPointType(FT); // Erstes Gerät ist FT
        }
        else
        {
            TermPointType PT = TermPointType::PT;
            dev->SetTerminationPointType(PT);
        }

        // Erstellen der MAC- und PHY-Schichten
        Ptr<Dect2020Mac> mac = CreateObject<Dect2020Mac>();
        Ptr<Dect2020Phy> phy = CreateObject<Dect2020Phy>();

        // Verbinden der Komponenten
        dev->SetMac(mac);
        dev->SetPhy(phy);
        mac->SetNetDevice(dev);
        mac->SetPhy(phy);
        phy->SetMac(mac);
        phy->SetDevice(dev);
        phy->SetChannel(channel);

        // mac->Start();

        // Hinzufügen des Geräts zum Knoten
        nodes.Get(i)->AddDevice(dev);

        // Setzen der MAC-Adresse
        dev->SetAddress(Mac48Address::Allocate());

        // Manuelles Setzen des Link-Status
        dev->SetLinkUp();

        devices.Add(dev);
        mac->Start();
    }

    // Ptr<Dect2020NetDevice> ft = DynamicCast<Dect2020NetDevice>(devices.Get(0));
    // ft->GetMac()->Start();

    // Setzen des Empfangs-Callbacks für Gerät 2
    // devices.Get(1)->SetReceiveCallback(MakeCallback(&ReceivePacket));

    // After creating all nodes and devices, add this:
    AnimationInterface anim("dect2020-animation.xml"); // Create animation interface
    anim.SetMobilityPollInterval(Seconds(0.1)); // Set update interval
    anim.EnablePacketMetadata(true); // Enable packet metadata for better visualization

    // Simulation ausführen
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}