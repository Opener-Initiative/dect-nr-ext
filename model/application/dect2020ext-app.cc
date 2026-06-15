#include "dect2020ext-app.h"
#include "ns3/dect2020ext-mac.h"
#include "ns3/simulator.h"

namespace ns3
{

Dect2020ExtApp::Dect2020ExtApp()
{
}

Dect2020ExtApp::~Dect2020ExtApp()
{
}

TypeId
Dect2020ExtApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtApp")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtApp>();

    return tid;
}

void
Dect2020ExtApp::Start()
{
    if (!m_mac || !m_packetGenerationPeriodSet)
    {
        NS_FATAL_ERROR("Missing configuration for the application");
        return;
    }
    else
    {
        GeneratePacket(); // First trigger
    }
}

void
Dect2020ExtApp::Stop()
{
    if (m_event.IsPending())
    {
        m_event.Cancel();
    }
}

void
Dect2020ExtApp::GeneratePacket()
{
    if (m_enqueuedPacketQuantity < m_packetLimit)
    {
        m_mac->EnqueuePacket();
        m_enqueuedPacketQuantity++;
        m_event = Simulator::Schedule(m_packetGenerationPeriod, &Dect2020ExtApp::GeneratePacket, this);
    }
    else
    {
        Stop();
    }
}

void
Dect2020ExtApp::SetPacketGenerationPeriod(Time period)
{
    m_packetGenerationPeriod = period;
    m_packetGenerationPeriodSet = true;
}

void
Dect2020ExtApp::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
}

void
Dect2020ExtApp::SetPacketLimit(int quantity)
{
    m_packetLimit = quantity;
}

}; // namespace ns3