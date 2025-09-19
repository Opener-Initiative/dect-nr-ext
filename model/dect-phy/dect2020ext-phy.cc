#include "dect2020ext-phy.h"

#include "ns3/simulator.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhy");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhy);

TypeId
Dect2020ExtPhy::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhy")
                            .SetParent<Object>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhy>();
    return tid;
}

Dect2020ExtPhy::Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtPhy::StartFrameTimer()
{
    NS_LOG_FUNCTION(this);

    for (uint32_t slot = 0; slot < 24; slot++)
    {
        double slotStartTime = slot * SLOT_DURATION_NS;

        Simulator::Schedule(NanoSeconds(slotStartTime),
                            &Dect2020ExtPhy::ProcessSlot,
                            this,
                            slot,
                            slotStartTime);
    }

    Simulator::Schedule(MicroSeconds(FRAME_DURATION_US),
                        &Dect2020ExtPhy::StartFrameTimer,
                        this);
}

void
Dect2020ExtPhy::ProcessSlot(uint32_t slot, double slotStartTime)
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtPhy::ProcessSubslot(uint32_t slotId, uint32_t subslotId)
{
    NS_LOG_FUNCTION(this);
}

Slot*
Dect2020ExtPhy::GetCurrentSlot(uint32_t channelId)
{
    NS_LOG_FUNCTION(this);
}

Subslot*
Dect2020ExtPhy::GetCurrentSubslot(uint32_t channelId)
{
    NS_LOG_FUNCTION(this);
}

} // namespace ns3