#include "dect2020-channel.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020Channel");
NS_OBJECT_ENSURE_REGISTERED(Dect2020Channel);

Dect2020Channel::Dect2020Channel()
{
    m_simpleChannel = CreateObject<SimpleChannel>();
}

Dect2020Channel::~Dect2020Channel()
{
    m_simpleChannel = nullptr;
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

// Virtual methods from Channel

std::size_t
Dect2020Channel::GetNDevices() const
{
    return m_simpleChannel->GetNDevices();
}

Ptr<NetDevice>
Dect2020Channel::GetDevice(std::size_t i) const
{
    return m_simpleChannel->GetDevice(i);
}

// Wrapped methods from SimpleChannel

void
Dect2020Channel::Add(Ptr<SimpleNetDevice> device)
{
    m_simpleChannel->Add(device);
}

void
Dect2020Channel::SetAttribute(std::string name, const AttributeValue &value)
{
    m_simpleChannel->SetAttribute(name, value);
}

// Dect2020Channel Setters and Getters

Ptr<SimpleChannel>
Dect2020Channel::GetWrappedChannel(void) const
{
    return m_simpleChannel;
}

} // namespace ns3

