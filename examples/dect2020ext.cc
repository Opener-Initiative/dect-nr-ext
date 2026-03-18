#include "ns3/dect2020ext-mac-scheduler.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-config.h"
#include "ns3/dect2020ext-utilities.h"
#include "ns3/dect2020ext-statistics.h"

#include "ns3/simulator.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/node.h"
#include "ns3/mobility-module.h"
#include "ns3/net-device-container.h"
#include "ns3/isotropic-antenna-model.h"

#include <string>

using namespace ns3;

using DlDataReception = Dect2020ExtAssociationControlIE::DlDataReception;
using ULPeriod = Dect2020ExtAssociationControlIE::ULPeriod;

NS_LOG_COMPONENT_DEFINE("Dect2020ExtExample");

int
main(int argc, char *argv[])
{
    LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    LogComponentEnable("Dect2020ExtStatistics", LogLevel(LOG_LEVEL_INFO));

    // LogComponentEnable("Dect2020ExtMacScheduler", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtMac", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtPhy", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC | LOG_PREFIX_NODE));
    // LogComponentEnable("Dect2020ExtInterference", LogLevel(LOG_LEVEL_INFO | LOG_PREFIX_TIME | LOG_PREFIX_FUNC));
    // LogComponentEnable("SingleModelSpectrumChannel", LOG_LEVEL_LOGIC);

    Dect2020ExtConfig config;
    Dect2020ExtAssociationControlIE associationControlIE;
    associationControlIE.SetCbM(1); // maintain Cluster Beacon reception
    associationControlIE.SetDlDataReception(DlDataReception::MS_0); // maintaining Cluster Beacon reception
    associationControlIE.SetUlPeriod(ULPeriod::S_10); // association maintenance period 10 seconds

    // ################
    // ###### FT ######
    // ################

    Ptr<Dect2020ExtPhy> ftPhy = CreateObject<Dect2020ExtPhy>();
    config.ApplyToPhy(ftPhy);

    Dect2020ExtRandomAccessResourceIE ftRandomAccessResources;

    Ptr<Dect2020ExtMac> ftMac = CreateObject<Dect2020ExtMac>();
    ftMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftMac->SetPhy(ftPhy);
    ftMac->ConfigureRandomAccessResources(ftRandomAccessResources);
    config.ApplyToMac(ftMac);

    ftPhy->SetMac(ftMac);

    Ptr<Node> ftNode = CreateObject<Node>();

    Ptr<Dect2020ExtNetDevice> ftNetDevice = CreateObject<Dect2020ExtNetDevice>();
    ftNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT);
    ftNetDevice->SetPhy(ftPhy);
    ftNetDevice->SetMac(ftMac);
    ftNetDevice->SetNode(ftNode);

    ftNode->AddDevice(ftNetDevice);

    InstallCenterMobilityModel(ftNode);
    ftPhy->SetMobility(ftNode->GetObject<MobilityModel>());

    // ################
    // ###### PT ######
    // ################

    int ptNumber = 1;

    for (int i = 0; i < ptNumber; i++)
    {
        Ptr<Dect2020ExtPhy> ptPhy = CreateObject<Dect2020ExtPhy>();
        config.ApplyToPhy(ptPhy);

        Ptr<Dect2020ExtMac> ptMac = CreateObject<Dect2020ExtMac>();
        ptMac->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptMac->SetPhy(ptPhy);
        ptMac->ConfigureAssociationControlIE(associationControlIE);
        config.ApplyToMac(ptMac);

        ptPhy->SetMac(ptMac);

        Ptr<Node> ptNode = CreateObject<Node>();

        Ptr<Dect2020ExtNetDevice> ptNetDevice = CreateObject<Dect2020ExtNetDevice>();
        ptNetDevice->SetTerminationPointType(Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT);
        ptNetDevice->SetPhy(ptPhy);
        ptNetDevice->SetMac(ptMac);
        ptNetDevice->SetNode(ptNode);

        ptNode->AddDevice(ptNetDevice);

        InstallCenterMobilityModel(ptNode);
        ptPhy->SetMobility(ptNode->GetObject<MobilityModel>());
        ptMac->Initialize();
        ptMac->Start();
        ptPhy->Start();
    }

    ftMac->Initialize();
    ftMac->Start();
    ftPhy->Start();


    Simulator::Stop(Seconds(4));
    Simulator::Run();
    Simulator::Destroy();

    Dect2020ExtStatistics::OutputDroppedPackets();
    Dect2020ExtStatistics::OutputAcceptedPackets();

    return 0;
}