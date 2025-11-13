#include "dect2020ext-phy.h"
#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-spectrum-signal-parameters.h"

#include "ns3/simulator.h"
#include "ns3/pointer.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhy");
NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhy);

const std::vector<uint16_t> Dect2020ExtPhy::m_singleSlotSingleStreamTransportBlockSizesMu1Beta1
{
    136,  // MCS 0
    296,  // MCS 1
    456,  // MCS 2
    616,  // MCS 3
    936,  // MCS 4
    1256, // MCS 5
    1416, // MCS 6
    1576, // MCS 7
    1896, // MCS 8
    2040, // MCS 9
    2296, // MCS 10
    2552  // MCS 11
};

Dect2020ExtPhy::Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
    m_SFN = 0;
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

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
                                                "ns3::TracedValueCallback::Uint32")

                                .AddAttribute("config_BandNumber",
                                              "DECT-2020 band number configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtPhy::m_bandNumber),
                                              MakeUintegerChecker<uint8_t>(1, 21))

                                .AddAttribute("config_SubcarrierScalingFactor",
                                              "DECT-2020 subcarrier scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtPhy::m_subcarrierScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8))

                                .AddAttribute("config_FourierTransformScalingFactor",
                                              "DECT-2020 Fourier transform scaling factor configuration",
                                              UintegerValue(1),
                                              MakeUintegerAccessor(&Dect2020ExtPhy::m_fourierTransformScalingFactor),
                                              MakeUintegerChecker<uint8_t>(1, 8))

                                .AddAttribute("config_ChannelManager",
                                              "Dect2020ExtChannelManager configuration",
                                              PointerValue(nullptr),
                                              MakePointerAccessor(&Dect2020ExtPhy::m_channelManager),
                                              MakePointerChecker<Dect2020ExtChannelManager>());
    return tid;
}

void
Dect2020ExtPhy::Start()
{
    NS_LOG_FUNCTION(this);
    StartFrameTimer();
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

/**
 * Assigns the MAC to the PHY
 */
void
Dect2020ExtPhy::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
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

uint16_t
Dect2020ExtPhy::GetMcsTransportBlockSize(uint8_t mu, uint8_t beta, uint8_t mcsIndex)
{
    if (mu == 1 && beta == 1)
    {
        if (mcsIndex >= m_singleSlotSingleStreamTransportBlockSizesMu1Beta1.size())
        {
            NS_LOG_WARN("Invalid MCS index for (mu = 1, beta = 1)");
            return 0;
        }
        return m_singleSlotSingleStreamTransportBlockSizesMu1Beta1[mcsIndex];
    }

    NS_LOG_WARN("Unsupported (mu,beta) combination in GetMCSTransportBlockSize");
    return 0;
}

void
Dect2020ExtPhy::SetBandNumber(uint8_t bandNumber)
{
    NS_LOG_FUNCTION(this << bandNumber);
    m_bandNumber = bandNumber;
}

uint8_t
Dect2020ExtPhy::GetBandNumber() const
{
    return m_bandNumber;
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
Dect2020ExtPhy::SetFourierTransformScalingFactor(uint8_t fourierTransformScalingFactor)
{
    NS_LOG_FUNCTION(this << fourierTransformScalingFactor);
    m_fourierTransformScalingFactor = fourierTransformScalingFactor;
}

uint8_t
Dect2020ExtPhy::GetFourierTransformScalingFactor() const
{
    return m_fourierTransformScalingFactor;
}

void
Dect2020ExtPhy::PrintConfiguration(void) const
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO("DECT-2020 PHY Configuration:");
    NS_LOG_INFO("Band Number: " << static_cast<uint32_t>(m_bandNumber));
    NS_LOG_INFO("Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO("Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor));

    std::cout << "DECT-2020 PHY Configuration:" << "\n";
    std::cout << "Band Number: " << static_cast<uint32_t>(m_bandNumber) << "\n";
    std::cout << "Subcarrier Scaling Factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor) << "\n";
    std::cout << "Fourier Transform Scaling Factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor) << "\n";
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

void
Dect2020ExtPhy::Send(Ptr<Packet> packet, Dect2020ExtPHYControlFieldType1 physicalHeader)
{
    NS_LOG_FUNCTION(this << packet << physicalHeader);
    NS_ASSERT_MSG(packet, "Packet is null");
    NS_ASSERT_MSG(this->m_device, "m_device is null");
    NS_ASSERT_MSG(this->m_mac, "m_mac is null");
    NS_ASSERT_MSG(this->m_channelManager, "m_channelManager is null");

    // Create and initialize the transmission parameters
    Ptr<Dect2020ExtSpectrumSignalParameters> params = Create<Dect2020ExtSpectrumSignalParameters>();
    params->txPhy = this;
    params->txPacket = packet->Copy(); // deep copy to avoid modifying original
    params->txPacket->AddHeader(physicalHeader);
    params->m_currentChannelId = this->m_mac->GetOperatingChannelId();

    // Calculate the TX duration based on the packet size and MCS
    Time duration = Time(NanoSeconds(CalculateTxDurationNs(physicalHeader)));
    params->duration = duration;

    Ptr<const SpectrumModel> spectrumModel = m_channelManager->GetSpectrumModel(m_bandNumber);
    Ptr<SpectrumValue> psd = Create<SpectrumValue>(spectrumModel);
    params->psd = psd;

    // Start the transmission
    Simulator::Schedule(duration, &ns3::SpectrumChannel::StartTx, m_channel, params);
    DynamicCast<Dect2020ExtNetDevice>(m_device)->m_txTrace(packet);
}

double
Dect2020ExtPhy::CalculateTxDurationNs(Dect2020ExtPHYControlFieldType1 physicalHeader)
{
    // TODO: Implement this method

    // if (physicalHeaderField.GetPacketLengthType() == 0) // Packet length is given in subslots
    // {
    //     // TBD
    // }
    // else if (physicalHeaderField.GetPacketLengthType() == 1) // Packet length is given in slots
    // {
    //     return physicalHeaderField.GetPacketLength() * 416670; // 416.67 ns per slot
    // }
    // else
    // {
    //     NS_LOG_WARN("Invalid packet length type");
    //     return 0;
    // }

    return 0;
}

} // namespace ns3