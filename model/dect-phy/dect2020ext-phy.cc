#include "dect2020ext-phy.h"

#include "ns3/simulator.h"
#include "ns3/dect2020ext-channel-manager.h"

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
                            .AddConstructor<Dect2020ExtPhy>()
                            .AddTraceSource("SFN",
                                            "System Frame Number (SFN)",
                                            MakeTraceSourceAccessor(&Dect2020ExtPhy::m_SFN),
                                            "ns3::TracedValueCallback::Uint32");
    return tid;
}

Dect2020ExtPhy::Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
    m_SFN = 0;
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

void
Dect2020ExtPhy::StartFrameTimer()
{
    NS_LOG_FUNCTION(this);

    m_frameStartTime = Simulator::Now();
    m_currentSubslotId = 0;
    uint32_t subslotsPerSlot = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    m_totalSubslotsPerFrame = SLOTS_PER_FRAME * subslotsPerSlot;
    double subslotDurationNs = SLOT_DURATION_NS / subslotsPerSlot;

    // Schedule all subslots for this frame
    for (uint32_t slot = 0; slot < SLOTS_PER_FRAME; slot++)
    {
        for (uint32_t subslot = 0; subslot < subslotsPerSlot; subslot++)
        {
            double subslotStartTime = (slot * SLOT_DURATION_NS) + (subslot * subslotDurationNs);

            Simulator::Schedule(NanoSeconds(subslotStartTime),
                                &Dect2020ExtPhy::ProcessSubslot,
                                this,
                                slot,
                                subslot,
                                slot * subslotsPerSlot + subslot);
        }
    }

    Simulator::Schedule(MicroSeconds(FRAME_DURATION_US),
                        &Dect2020ExtPhy::StartFrameTimer,
                        this);
}

void
Dect2020ExtPhy::ProcessSlot(uint32_t slot, double slotStartTime)
{
}

void
Dect2020ExtPhy::ProcessSubslot(uint32_t slotId, uint32_t subslotId, uint32_t absoluteSubslotId)
{
    if (slotId == 0 && subslotId == 0)
    {
        m_SFN++;
    }
    uint32_t subslotsPerSlot = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    m_currentSubslotId = slotId * subslotsPerSlot + subslotId;
    NS_LOG_INFO(slotId << " " << subslotId << " " << absoluteSubslotId);
}

Slot*
Dect2020ExtPhy::GetCurrentSlot()
{
    return nullptr;
}

Subslot*
Dect2020ExtPhy::GetCurrentSubslot()
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(m_currentSubslotId);
    // TODO: Implement subslot retrieval logic
    return nullptr;
}

void
Dect2020ExtPhy::OnNewFrame(Callback<void, uint32_t, uint32_t> callback)
{
    m_SFNCallback = callback;
    TraceConnectWithoutContext("SFN", m_SFNCallback);
}

void
Dect2020ExtPhy::DisconnectSFNCallback()
{
    TraceDisconnectWithoutContext("SFN", m_SFNCallback);
}

void
Dect2020ExtPhy::SetSubcarrierScalingFactor(uint8_t subcarrierScalingFactor)
{
    NS_LOG_FUNCTION(this << subcarrierScalingFactor);
    m_subcarrierScalingFactor = subcarrierScalingFactor;
}

uint8_t
Dect2020ExtPhy::GetSubcarrierScalingFactor() const
{
    return m_subcarrierScalingFactor;
}

Time
Dect2020ExtPhy::GetNextSubslotTime()
{
    NS_LOG_FUNCTION(this);

    uint32_t subslotsPerSlot = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    double subslotDurationNs = SLOT_DURATION_NS / subslotsPerSlot;

    uint32_t nextSubslotId = (m_currentSubslotId + 1) % m_totalSubslotsPerFrame;

    // If we're going to the next frame
    if (nextSubslotId == 0)
    {
        Time nextFrameStart = m_frameStartTime + MicroSeconds(FRAME_DURATION_US);
        return nextFrameStart;
    }

    Time nextSubslotTime = m_frameStartTime + NanoSeconds(nextSubslotId * subslotDurationNs);
    return nextSubslotTime;
}

} // namespace ns3