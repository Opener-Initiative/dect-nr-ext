#include "dect-channel.h"

#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"

DectChannel::DectChannel()
{
}

DectChannel
DectChannel::Default()
{
    DectChannel channel;
    channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
    return channel;
}

ns3::Ptr<ns3::YansWifiChannel>
DectChannel::Create() const
{
    ns3::Ptr<ns3::YansWifiChannel> channel = ns3::CreateObject<ns3::YansWifiChannel>();
    ns3::Ptr<ns3::PropagationLossModel> loss = m_propagationLoss.front().Create<ns3::PropagationLossModel>();
    channel->SetPropagationLossModel(loss);
    ns3::Ptr<ns3::PropagationDelayModel> delay = m_propagationDelay.Create<ns3::PropagationDelayModel>();
    channel->SetPropagationDelayModel(delay);
    return channel;
}
