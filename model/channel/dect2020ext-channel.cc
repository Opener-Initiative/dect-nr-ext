#include "dect2020ext-channel.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtChannel");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtChannel);

TypeId
Dect2020ExtChannel::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtChannel")
                            .SetParent<Object>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtChannel>();
    return tid;
}

Dect2020ExtChannel::Dect2020ExtChannel()
{
    NS_LOG_FUNCTION(this);

    std::vector<Slot> m_slots;
    std::vector<Slot> m_slotsLastFrame;
}

Dect2020ExtChannel::~Dect2020ExtChannel()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtChannel::AddSlot(Slot slot)
{
    m_slots.push_back(slot);

    if (m_slots.size() == 24)
    {
        m_slotsLastFrame = m_slots; // Deep copy of the fresh initialized Slot List
    }
}

} // namespace ns3
