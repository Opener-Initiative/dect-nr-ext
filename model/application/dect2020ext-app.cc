#include "dect2020ext-app.h"
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
    if (!m_mac || !m_txPeriodSet)
    {
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
    Ptr<Packet> packet = Create<Packet>();
    m_mac->EnqueuePacket(packet);

    m_event = Simulator::Schedule(m_txPeriod, &Dect2020ExtApp::GeneratePacket, this);
}

void
Dect2020ExtApp::SetTxPeriod(Time period)
{
    m_txPeriod = period;
    m_txPeriodSet = true;
}

void
Dect2020ExtApp::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
}

}; // namespace ns3