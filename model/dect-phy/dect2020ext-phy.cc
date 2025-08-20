/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "dect2020ext-phy.h"

#include "ns3/dect2020-phy.h"
#include "ns3/dect2020-channel.h"
#include "ns3/dect2020-mac.h"
#include "ns3/dect2020-net-device.h"

#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("Dect2020ExtPhy");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(Dect2020ExtPhy);

TypeId
Dect2020ExtPhy::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Dect2020ExtPhy")
                            .SetParent<SpectrumPhy>()
                            .SetGroupName("Dect2020Ext")
                            .AddConstructor<Dect2020ExtPhy>();
    return tid;
}

Dect2020ExtPhy::Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
    // Create and initialize the actual Dect2020Phy implementation
    m_dect2020Phy = CreateObject<Dect2020Phy>();
}

Dect2020ExtPhy::~Dect2020ExtPhy()
{
    NS_LOG_FUNCTION(this);
}

/**
 * \brief Initializes the PHY layer and starts the frame timer.
 */
void
Dect2020ExtPhy::Start()
{
    NS_LOG_FUNCTION(this);
    m_dect2020Phy->Start();
}

/**
 * \brief Sets the MAC layer instance connected to this PHY.
 * \param mac Pointer to the MAC layer.
 */
void
Dect2020ExtPhy::SetMac(Ptr<Dect2020Mac> mac)
{
    NS_LOG_FUNCTION(this << mac);
    m_dect2020Phy->SetMac(mac);
}

/**
 * \brief Sets the network device associated with this PHY.
 * \param device Pointer to the NetDevice.
 */
void
Dect2020ExtPhy::SetDevice(Ptr<NetDevice> device)
{
    NS_LOG_FUNCTION(this << device);
    m_dect2020Phy->SetDevice(device);
}

/**
 * \brief Sets the SpectrumChannel used by this PHY and registers this PHY as receiver.
 * \param channel Pointer to the SpectrumChannel.
 */
void
Dect2020ExtPhy::SetChannel(Ptr<SpectrumChannel> channel)
{
    NS_LOG_FUNCTION(this << channel);
    m_dect2020Phy->SetChannel(channel);
}

/**
 * \brief Returns the associated SpectrumChannel.
 * \return Pointer to the SpectrumChannel.
 */
Ptr<SpectrumChannel>
Dect2020ExtPhy::GetChannel(void) const
{
    return m_dect2020Phy->GetChannel();
}

/**
 * \brief Returns the associated network device.
 * \return Pointer to the NetDevice.
 */
Ptr<NetDevice>
Dect2020ExtPhy::GetDevice(void) const
{
    return m_dect2020Phy->GetDevice();
}

/**
 * \brief Send a packet from the PHY layer to the SpectrumChannel.
 */
void
Dect2020ExtPhy::Send(Ptr<Packet> packet, Dect2020PHYControlFieldType1 physicalHeader)
{
    NS_LOG_FUNCTION(this << packet);
    m_dect2020Phy->Send(packet, physicalHeader);
}

/**
 * \brief Set the callback to be invoked when a packet is received.
 */
void
Dect2020ExtPhy::SetReceiveCallback(Callback<void, Ptr<Packet>, double> cb)
{
    NS_LOG_FUNCTION(this);
    m_dect2020Phy->SetReceiveCallback(cb);
}

/**
 * \brief Get the current absolute subslot index.
 */
uint16_t
Dect2020ExtPhy::GetCurrentAbsoluteSubslot() const
{
    return m_dect2020Phy->GetCurrentAbsoluteSubslot();
}

/**
 * \brief Set the mobility model for this PHY layer.
 */
void
Dect2020ExtPhy::SetMobility(Ptr<MobilityModel> m)
{
    NS_LOG_FUNCTION(this << m);
    m_dect2020Phy->SetMobility(m);
}

/**
 * \brief Get the associated mobility model.
 */
Ptr<MobilityModel>
Dect2020ExtPhy::GetMobility() const
{
    return m_dect2020Phy->GetMobility();
}

/**
 * \brief Get the spectrum model used for reception.
 */
Ptr<const SpectrumModel>
Dect2020ExtPhy::GetRxSpectrumModel() const
{
    return m_dect2020Phy->GetRxSpectrumModel();
}

/**
 * \brief Get the antenna object associated with this PHY.
 */
Ptr<Object>
Dect2020ExtPhy::GetAntenna() const
{
    return m_dect2020Phy->GetAntenna();
}

/**
 * \brief Set the antenna object for this PHY.
 */
void
Dect2020ExtPhy::SetAntenna(Ptr<Object> antenna)
{
    NS_LOG_FUNCTION(this << antenna);
    m_dect2020Phy->SetAntenna(antenna);
}

/**
 * \brief Begins the reception process of a DECT-2020 NR packet.
 */
void
Dect2020ExtPhy::StartRx(Ptr<SpectrumSignalParameters> params)
{
    NS_LOG_FUNCTION(this << params);
    m_dect2020Phy->StartRx(params);
}

/**
 * \brief Starts a periodic frame timer to simulate DECT-2020 NR slot-based timing.
 */
void
Dect2020ExtPhy::StartFrameTimer()
{
    NS_LOG_FUNCTION(this);
    m_dect2020Phy->StartFrameTimer();
}

/**
 * \brief Processes a single slot within a DECT-2020 NR frame.
 */
void
Dect2020ExtPhy::ProcessSlot(uint32_t slot, double slotStartTime)
{
    NS_LOG_FUNCTION(this << slot << slotStartTime);
    m_dect2020Phy->ProcessSlot(slot, slotStartTime);
}

/**
 * \brief Processes an individual subslot within a slot.
 */
void
Dect2020ExtPhy::ProcessSubslot(uint32_t slotId, uint32_t subslotId)
{
    NS_LOG_FUNCTION(this << slotId << subslotId);
    m_dect2020Phy->ProcessSubslot(slotId, subslotId);
}

/**
 * \brief Returns a pointer to the current Slot object.
 */
Slot*
Dect2020ExtPhy::GetCurrentSlot(uint32_t channelId)
{
    return m_dect2020Phy->GetCurrentSlot(channelId);
}

/**
 * \brief Returns a pointer to the current Subslot object.
 */
Subslot*
Dect2020ExtPhy::GetCurrentSubslot(uint32_t channelId)
{
    return m_dect2020Phy->GetCurrentSubslot(channelId);
}

/**
 * \brief Returns the transport block size in bits for a given MCS index.
 */
uint16_t
Dect2020ExtPhy::GetMcsTransportBlockSize(uint8_t mu, uint8_t beta, uint8_t mcsIndex)
{
    return m_dect2020Phy->GetMcsTransportBlockSize(mu, beta, mcsIndex);
}

/**
 * \brief Calculates the transmission duration in nanoseconds for a given packet.
 */
double
Dect2020ExtPhy::CalculateTxDurationNs(Dect2020PHYControlFieldType1 physicalHeaderField)
{
    return m_dect2020Phy->CalculateTxDurationNs(physicalHeaderField);
}

/**
 * \brief Calculates the time offset from the current subslot to a target subslot.
 */
Time
Dect2020ExtPhy::GetTimeToNextAbsoluteSubslot(uint8_t targetSfn,
                                             uint8_t targetSlot,
                                             uint8_t targetSubslot) const
{
    return m_dect2020Phy->GetTimeToNextAbsoluteSubslot(targetSfn, targetSlot, targetSubslot);
}

/**
 * \brief Converts a slot and subslot index into an absolute subslot index within a frame.
 */
int
Dect2020ExtPhy::GetAbsoluteSubslotIndex(uint8_t slot, uint8_t subslot) const
{
    return m_dect2020Phy->GetAbsoluteSubslotIndex(slot, subslot);
}

/**
 * \brief Receive a packet (not implemented in base class).
 */
void
Dect2020ExtPhy::Receive(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
    // Note: Receive is not implemented in the base class
    NS_LOG_WARN("Receive method not implemented in base class");
}

/**
 * \brief End reception (not implemented in base class).
 */
void
Dect2020ExtPhy::EndRx(Ptr<SpectrumSignalParameters> params)
{
    NS_LOG_FUNCTION(this << params);
    // Note: EndRx is not implemented in the base class
    NS_LOG_WARN("EndRx method not implemented in base class");
}

/**
 * \brief Get absolute subslot time (not implemented in base class).
 */
Time
Dect2020ExtPhy::GetAbsoluteSubslotTime(uint8_t targetSfn, uint8_t slot, uint8_t subslot) const
{
    NS_LOG_WARN("GetAbsoluteSubslotTime not implemented in base class");
    return Time(0);
}

/**
 * \brief Get time until subslot (not implemented in base class).
 */
Time
Dect2020ExtPhy::GetTimeUntilSubslot(uint8_t targetSfn, uint8_t targetSlot, uint8_t targetSubslot) const
{
    NS_LOG_WARN("GetTimeUntilSubslot not implemented in base class");
    return Time(0);
}

} // namespace ns3