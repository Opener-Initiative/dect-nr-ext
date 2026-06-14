#include "dect2020ext-phy.h"
#include "dect2020ext-interference.h"

#include "ns3/dect2020ext-phy-control-field-type-1.h"
#include "ns3/dect2020ext-phy-control-field-type-2.h"
#include "ns3/dect2020ext-channel-manager.h"
#include "ns3/dect2020ext-spectrum-signal-parameters.h"
#include "ns3/dect2020ext-mac-header-type.h"
#include "ns3/dect2020ext-interference.h"
#include "ns3/dect2020ext-statistics.h"
#include "ns3/dect2020ext-config-values.h"

#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include <cstdint>

namespace ns3
{

using PhyState = Dect2020ExtPhy::PhyState;

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
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

TypeId
Dect2020ExtPhy::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhy")
                                .SetParent<Object>()
                                .SetGroupName("Dect2020Ext")
                                .AddConstructor<Dect2020ExtPhy>();
    return tid;
}

void
Dect2020ExtPhy::Start()
{
    NS_LOG_FUNCTION(this);

    // Configuration
    std::string errorMessage = " was not set for Dect2020ExtPhy";

    NS_ABORT_MSG_IF(!m_bandNumberSet, "Band number" << errorMessage);
    NS_ABORT_MSG_IF(!m_subcarrierScalingFactorSet, "Subcarrier scaling factor" << errorMessage);
    NS_ABORT_MSG_IF(!m_fourierTransformScalingFactorSet, "Fourier transform scaling factor" << errorMessage);
    NS_ABORT_MSG_IF(!m_channelManager, "Channel Manager" << errorMessage);
    NS_ABORT_MSG_IF(!m_channel, "Spectrum Channel" << errorMessage);
    NS_ABORT_MSG_IF(!m_antenna, "Antenna" << errorMessage);
    NS_ABORT_MSG_IF(!m_device, "Net Device" << errorMessage);
    NS_ABORT_MSG_IF(!m_mobilityModel, "Mobility Model" << errorMessage);
    NS_ABORT_MSG_IF(!m_receiveCallbackSet, "Receive callback" << errorMessage);
    NS_ABORT_MSG_IF(!m_mac, "MAC" << errorMessage);

    if (m_mac->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::FT)
    {
        std::ostringstream oss;
        oss << "FT_PHY[" << m_device->GetNode()->GetId() << "]";
        PHY_IDENTITY = oss.str();
    }
    else if (m_mac->GetTerminationPointType() == Dect2020ExtNetDevice::Dect2020ExtTerminationPointType::PT)
    {
        std::ostringstream oss;
        oss << "PT_PHY[" << m_device->GetNode()->GetId() << "]";
        PHY_IDENTITY = oss.str();
    }

    // State Machine
    m_currentState = IDLE;

    // Interference
    m_interference = CreateObject<Dect2020ExtInterference>();
    m_interference->m_mac = this->m_mac;
    m_interference->Initialize(m_channelManager->GetSpectrumModel(m_bandNumber));

    // Register as a receiver only after full initialization, so signals
    // are never delivered to a PHY whose m_interference is still null.
    m_channel->AddRx(this);
}

void
Dect2020ExtPhy::SetMac(Ptr<Dect2020ExtMac> mac)
{
    m_mac = mac;
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
            NS_FATAL_ERROR("Invalid MCS index for (mu = 1, beta = 1)");
            return 0;
        }
        return m_singleSlotSingleStreamTransportBlockSizesMu1Beta1[mcsIndex];
    }

    NS_LOG_WARN("Unsupported (mu,beta) combination in GetMCSTransportBlockSize");
    return 0;
}

void
Dect2020ExtPhy::SetReceiveCallback(Callback<void, Ptr<Packet>, double, bool, bool> callback)
{
    m_receiveCallback = callback;
    m_receiveCallbackSet = true;
}

PhyState
Dect2020ExtPhy::GetState() const
{
    return m_currentState;
}

void
Dect2020ExtPhy::SetBandNumber(uint8_t bandNumber)
{
    NS_LOG_FUNCTION(this << bandNumber);
    m_bandNumber = bandNumber;
    m_bandNumberSet = true;
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
    m_subcarrierScalingFactorSet = true;
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
    m_fourierTransformScalingFactorSet = true;
}

uint8_t
Dect2020ExtPhy::GetFourierTransformScalingFactor() const
{
    return m_fourierTransformScalingFactor;
}

void
Dect2020ExtPhy::PrintConfiguration() const
{
    NS_LOG_FUNCTION(this);
    NS_LOG_INFO(PHY_IDENTITY << ": dect-2020 PHY configuration:");
    NS_LOG_INFO(PHY_IDENTITY << ": band number: " << static_cast<uint32_t>(m_bandNumber));
    NS_LOG_INFO(PHY_IDENTITY << ": subcarrier scaling factor: " << static_cast<uint32_t>(m_subcarrierScalingFactor));
    NS_LOG_INFO(PHY_IDENTITY << ": fourier transform scaling factor: " << static_cast<uint32_t>(m_fourierTransformScalingFactor));

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
    // Cast the generic SpectrumSignalParameters to the specific DECT-2020 NR parameters
    Ptr<Dect2020ExtSpectrumSignalParameters> dectParams = DynamicCast<Dect2020ExtSpectrumSignalParameters>(params);

    // Drop packet if the params are not DECT params
    if (!dectParams)
    {
        NS_LOG_INFO("Received non-DECT signal (e.g., from WaveformGenerator), ignoring");
        return;
    }

    // Extract the channel index within the spectrum model
    uint16_t channelIndex = dectParams->m_currentChannelNumber -
        m_channelManager->GetFirstValidChannelNumber(m_channelManager->GetBandNumber(dectParams->m_currentChannelNumber));

    // Extract the received signal strength (in Watt and dBm) from the PSD
    double rssiPacket = (*params->psd)[channelIndex];
    double rssiPacketDbm = m_channelManager->WToDbm(rssiPacket);
    Ptr<Dect2020ExtNetDevice> dectNetDevice = DynamicCast<Dect2020ExtNetDevice>(this->m_device);
    double minRxSensitivityDbm = -99.7 + this->m_mac->GetRxGainFromIndex(dectNetDevice->m_rxGain);

    // Add the received signal to the interference (signal will be removed after the duration)
    m_interference->AddSignal(params->psd, params->duration, channelIndex);

    // Drop packet if the device is not currently tuned to the correct channel
    if (dectParams->m_currentChannelNumber != m_mac->GetOperatingChannelNumber())
    {
        NS_LOG_INFO(PHY_IDENTITY << ": Received packet on wrong channel");
        NS_LOG_INFO(PHY_IDENTITY << ": Current channel: " << m_mac->GetOperatingChannelNumber());
        NS_LOG_INFO(PHY_IDENTITY << ": Channel from dect params: " << dectParams->m_currentChannelNumber);
        return;
    }

    // Drop packet if below minimum RX sensitivity
    if (rssiPacketDbm < minRxSensitivityDbm)
    {
        NS_LOG_INFO(PHY_IDENTITY << ": Dropping packet due to weak signal");
        Dect2020ExtStatistics::m_droppedPacketsPhy[PHY_IDENTITY]++;
        return;
    }

    // Drop packet if PHY is not in IDLE state
    if (m_currentState != IDLE)
    {
        NS_LOG_INFO(PHY_IDENTITY << ": Dropping packet due to collision");
        Dect2020ExtStatistics::m_droppedPacketsPhy[PHY_IDENTITY]++;
        return;
    }

    // Change state to RX - switch bach to IDLE after params->duration
    ChangeState(RX);
    Simulator::Schedule(params->duration, &Dect2020ExtPhy::ChangeState, this, IDLE);

    double rssiChannelWatt = (*m_interference->m_allSignals)[channelIndex];
    double rssiChannelDbm = m_channelManager->WToDbm(rssiChannelWatt);
    NS_LOG_INFO(PHY_IDENTITY << ": Received packet on the current channel " << dectParams->m_currentChannelNumber << " with RSSI " << rssiPacketDbm << " dBm");
    NS_LOG_INFO(PHY_IDENTITY << ": Total channel " << dectParams->m_currentChannelNumber << " power: " << rssiChannelDbm << " dBm");

    if (m_phyTest)
    {
        return;
    }
    else
    {
        // Otherwise, receive packet successfully
        Dect2020ExtStatistics::m_acceptedPacketsPhy[PHY_IDENTITY]++;

        // Pass the packet to the MAC after the whole duration (reception) of the signal
        Simulator::Schedule(params->duration, &Dect2020ExtPhy::EndRx, this, dectParams->m_txPacket, rssiPacketDbm, dectParams->m_data, dectParams->m_random);
    }
}

template <typename PhyControlFieldT>
void
Dect2020ExtPhy::Send(Ptr<Packet> packet, PhyControlFieldT phyControlField, bool data, bool random)
{
    NS_ASSERT_MSG(packet, "Packet is null");

    if (!IsAvailable())
    {
        NS_FATAL_ERROR("The PHY is not available, but the MAC attempted to transmit");
    }

    // Create and initialize the transmission parameters
    Ptr<Dect2020ExtSpectrumSignalParameters> params = Create<Dect2020ExtSpectrumSignalParameters>();
    params->txPhy = this;
    params->m_txPacket = packet->Copy(); // deep copy to avoid modifying original
    params->m_txPacket->AddHeader(phyControlField);
    params->m_currentChannelNumber = m_mac->GetOperatingChannelNumber();
    params->m_data = data;
    params->m_random = random;
    params->txMobility = m_mobilityModel;

    // Calculate the TX duration based on the packet size and MCS
    Time duration = Time(NanoSeconds(CalculateTxDurationNs(phyControlField)));
    params->duration = duration;

    // Get the channel index --> this is the index of the channel used for PSD
    uint16_t channelIndex = m_mac->GetOperatingChannelNumber() - m_channelManager->GetFirstValidChannelNumber(m_bandNumber);

    // Set the PSD value for the current channel
    Ptr<Dect2020ExtNetDevice> dectNetDevice = DynamicCast<Dect2020ExtNetDevice>(this->m_device);
    double txPowerWatt = m_channelManager->DbmToW(dectNetDevice->m_txPowerDbm);

    // Create PSD and initialize
    Ptr<const SpectrumModel> spectrumModel = m_channelManager->GetSpectrumModel(m_bandNumber);
    Ptr<SpectrumValue> psd = Create<SpectrumValue>(spectrumModel);

    for (uint32_t i = 0; i < psd->GetSpectrumModel()->GetNumBands(); ++i)
    {
        (*psd)[i] = 0.0;
    }

    // Set power only on the current channel
    (*psd)[channelIndex] = txPowerWatt;
    params->psd = psd;

    // Start the transmission immediately
    m_channel->StartTx(params);
    DynamicCast<Dect2020ExtNetDevice>(m_device)->m_txTrace(packet);

    NS_LOG_INFO(PHY_IDENTITY << ": Sending packet");

    // Change the state to TX -> switch back to IDLE after params->duration
    ChangeState(TX);
    Simulator::Schedule(params->duration, &Dect2020ExtPhy::ChangeState, this, IDLE);
}

template void Dect2020ExtPhy::Send<Dect2020ExtPhyControlFieldType1>(Ptr<Packet>, Dect2020ExtPhyControlFieldType1, bool, bool);
template void Dect2020ExtPhy::Send<Dect2020ExtPhyControlFieldType2>(Ptr<Packet>, Dect2020ExtPhyControlFieldType2, bool, bool);

template <typename PhyControlFieldT>
double
Dect2020ExtPhy::CalculateTxDurationNs(PhyControlFieldT physicalHeader)
{
    if (physicalHeader.GetPacketLengthType() == 0) // Packet length is given in subslots
    {
        double subslotDurationNs = SLOT_DURATION_NS / m_channelManager->GetSubslotsPerSlot();
        return physicalHeader.GetPacketLength() * subslotDurationNs;
    }
    else if (physicalHeader.GetPacketLengthType() == 1) // Packet length is given in slots
    {
        return physicalHeader.GetPacketLength() * SLOT_DURATION_NS;
    }
    else
    {
        NS_FATAL_ERROR("Invalid packet length type");
        return 0;
    }
}

template double Dect2020ExtPhy::CalculateTxDurationNs<Dect2020ExtPhyControlFieldType1>(Dect2020ExtPhyControlFieldType1);
template double Dect2020ExtPhy::CalculateTxDurationNs<Dect2020ExtPhyControlFieldType2>(Dect2020ExtPhyControlFieldType2);

void
Dect2020ExtPhy::ChangeState(PhyState newState)
{
    std::string state = "";
    switch (newState)
    {
        case IDLE: state = "IDLE"; break;
        case TX: state = "TX"; break;
        case RX: state = "RX"; break;
    }
    m_currentState = newState;
    NS_LOG_INFO(PHY_IDENTITY << ": State changed to " << state);
}

void
Dect2020ExtPhy::EndRx(Ptr<Packet> packet, double rssiDbm, bool data, bool random)
{
    m_receiveCallback(packet, rssiDbm, data, random);
}

bool
Dect2020ExtPhy::IsAvailable()
{
    return m_currentState == IDLE;
}

} // namespace ns3