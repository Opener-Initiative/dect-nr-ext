#include "dect2020-channel.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020Channel");
NS_OBJECT_ENSURE_REGISTERED(Dect2020Channel);

Dect2020Channel::Dect2020Channel()
{
}

Dect2020Channel::~Dect2020Channel()
{
}

TypeId
Dect2020Channel::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020Channel")
        .SetParent<Object>()
        .AddConstructor<Dect2020Channel>()
        .AddAttribute("Delay",
                      "Delay through the channel",
                      TimeValue(Seconds(0)),
                      MakeTimeAccessor(&Dect2020Channel::m_delay),
                      MakeTimeChecker());

    return tid;
}

TypeId
Dect2020Channel::GetInstanceTypeId(void) const
{
    return GetTypeId();
}


void
Dect2020Channel::Add(Ptr<Dect2020NetDevice> device)
{
    m_devices.push_back(device);
}

void
Dect2020Channel::Send(Ptr<Packet> packet, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<Dect2020NetDevice> sender)
{
    NS_LOG_FUNCTION(this);
    for (auto i = m_devices.begin(); i != m_devices.end(); ++i)
    {
        Ptr<Dect2020NetDevice> tmp = *i;
        if (tmp == sender)
        {
            continue;
        }
        // if (m_blackListedDevices.find(tmp) != m_blackListedDevices.end())
        // {
        //     if (find(m_blackListedDevices[tmp].begin(), m_blackListedDevices[tmp].end(), sender) !=
        //         m_blackListedDevices[tmp].end())
        //     {
        //         continue;
        //     }
        // }

        Simulator::ScheduleWithContext(tmp->GetNode()->GetId(),
                                       m_delay,
                                       &Dect2020NetDevice::Receive,
                                       tmp,
                                       packet->Copy(),
                                       protocol,
                                       dst,
                                       src);
    }
}

// Dect2020Channel Setters and Getters

Time
Dect2020Channel::GetDelay(void) const
{
    return m_delay;
}

// Virtual methods from Channel

std::size_t
Dect2020Channel::GetNDevices() const
{
    return m_devices.size();
}

Ptr<NetDevice>
Dect2020Channel::GetDevice(std::size_t i) const
{
    return m_devices[i];
}

} // namespace ns3

