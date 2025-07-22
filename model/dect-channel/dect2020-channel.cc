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

void
Dect2020Channel::Add(Ptr<SimpleNetDevice> device)
{
    m_simpleChannel->Add(device);
}

Ptr<SimpleChannel>
Dect2020Channel::GetWrappedChannel(void) const
{
    return m_simpleChannel;
}

void
Dect2020Channel::SetAttribute(std::string name, const AttributeValue &value)
{
    m_simpleChannel->SetAttribute(name, value);
}

} // namespace ns3

