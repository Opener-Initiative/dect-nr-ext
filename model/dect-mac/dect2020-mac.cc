#include "dect2020-mac.h"
#include "ns3/dect2020-mac-pdu-helper.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020Mac");
NS_OBJECT_ENSURE_REGISTERED(Dect2020Mac);

Dect2020Mac::Dect2020Mac()
{
    m_netDevice = nullptr;
}

Dect2020Mac::~Dect2020Mac()
{
    m_netDevice = nullptr;
}

TypeId
Dect2020Mac::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020Mac")
        .SetParent<Object>()
        .AddConstructor<Dect2020Mac>();

    return tid;
}

TypeId
Dect2020Mac::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
Dect2020Mac::ProcessPacket(Ptr<Packet> packet)
{
    NS_LOG_INFO("HELLO FROM MAC =)");
}

void
Dect2020Mac::InitializeNetwork()
{
    NS_LOG_FUNCTION(this);
}

void 
Dect2020Mac::GenerateValidNetworkID()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020Mac::StartBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    NS_ASSERT_MSG((m_netDevice != nullptr), "NetDevice is not set");
    NS_ASSERT_MSG((m_netDevice->GetChannel() != nullptr), "Channel is not set");
    NS_ASSERT_MSG((m_netDevice->GetRDMode() == Dect2020NetDevice::RDMode::FT), "NetDevice is not in FT mode");
    // StartNetworkBeaconSweep
    // StartClusterBeaconTransmission
}

void
Dect2020Mac::StopBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020Mac::StartNetworkBeaconSweep()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020Mac::StopNetworkBeaconSweep()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020Mac::StartClusterBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
    NS_ASSERT_MSG((m_netDevice != nullptr), "NetDevice is not set");
    NS_ASSERT_MSG((m_netDevice->GetChannel() != nullptr), "Channel is not set");
    NS_ASSERT_MSG((m_netDevice->GetRDMode() == Dect2020NetDevice::RDMode::FT), "NetDevice is not in FT mode");
}

void
Dect2020Mac::StopClusterBeaconTransmission()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020Mac::SetNetDevice(Ptr<Dect2020NetDevice> netDevice)
{
    m_netDevice = netDevice;
}

Ptr<Dect2020NetDevice>
Dect2020Mac::GetNetDevice() const
{
    return m_netDevice;
}

} // namespace ns3