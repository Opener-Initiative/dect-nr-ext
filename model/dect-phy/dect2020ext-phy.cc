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
    uint32_t subslotsPerSlot = m_channelManager->GetSubslotsPerSlot();
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
    // uint32_t subslotsPerSlot = Dect2020ExtChannelManager::GetSubslotsPerSlot();
    // m_currentSubslotId = slotId * subslotsPerSlot + subslotId;
    // NS_LOG_INFO(slotId << " " << subslotId << " " << absoluteSubslotId);
}

Slot*
Dect2020ExtPhy::GetCurrentSlot()
{
    // TODO: Implement slot retrieval logic
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

void
Dect2020ExtPhy::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
    m_mac->SetPhy(this);
}

uint32_t
Dect2020ExtPhy::GetCurrentSFN() const
{
    return m_SFN.Get();
}

void
Dect2020ExtPhy::SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager)
{
    m_channelManager = channelManager;
}

Ptr<Dect2020ExtChannelManager>
Dect2020ExtPhy::GetChannelManager() const
{
    return m_channelManager;
}

void
Dect2020ExtPhy::SetDevice(Ptr<NetDevice> d)
{
    m_device = d;
}

Ptr<NetDevice>
Dect2020ExtPhy::GetDevice() const
{
    return m_device;
}

void
Dect2020ExtPhy::SetMobility(Ptr<MobilityModel> m)
{
    m_mobilityModel = m;
}

Ptr<MobilityModel>
Dect2020ExtPhy::GetMobility() const
{
    return m_mobilityModel;
}

void
Dect2020ExtPhy::SetChannel(Ptr<SpectrumChannel> c)
{
    m_channel = c;
    if (m_channel)
    {
        m_channel->AddRx(this);
    }
}

Ptr<SpectrumChannel>
Dect2020ExtPhy::GetChannel() const
{
    return m_channel;
}

Ptr<const SpectrumModel>
Dect2020ExtPhy::GetRxSpectrumModel() const
{
    return m_spectrumModel;
}

void
Dect2020ExtPhy::SetAntenna(Ptr<Object> antenna)
{
    m_antenna = antenna;
}

Ptr<Object>
Dect2020ExtPhy::GetAntenna() const
{
    return m_antenna;
}

void
Dect2020ExtPhy::StartRx(Ptr<SpectrumSignalParameters> params)
{
    NS_LOG_FUNCTION(this << params);
    // TODO: Implement this method
}

} // namespace ns3