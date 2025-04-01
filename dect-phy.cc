#include "dect-phy.h"
#include "dect-channel.h"

#include "ns3/interference-helper.h"
#include "ns3/preamble-detection-model.h"
#include "ns3/error-rate-model.h"
#include "ns3/frame-capture-model.h"
#include "ns3/yans-wifi-phy.h"

DectPhy::DectPhy() : ns3::WifiPhyHelper(1), m_channel(nullptr)
{
    m_phys.front().SetTypeId("ns3::YansWifiPhy");
    SetInterferenceHelper("ns3::InterferenceHelper");
    SetErrorRateModel("ns3::TableBasedErrorRateModel");
}

void
DectPhy::SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel)
{
    m_channel = channel;
}

std::vector<ns3::Ptr<ns3::WifiPhy>>
DectPhy::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const
{
    // Creates a YansWifiPhy pointer 
    ns3::Ptr<ns3::YansWifiPhy> phy = m_phys.front().Create<ns3::YansWifiPhy>();

    // Sets interference
    ns3::Ptr<ns3::InterferenceHelper> interference = m_interferenceHelper.Create<ns3::InterferenceHelper>();
    phy->SetInterferenceHelper(interference);

    // Sets error rate model
    ns3::Ptr<ns3::ErrorRateModel> error = m_errorRateModel.front().Create<ns3::ErrorRateModel>();
    phy->SetErrorRateModel(error);

    if (m_frameCaptureModel.front().IsTypeIdSet())
    {
        auto frameCapture = m_frameCaptureModel.front().Create<ns3::FrameCaptureModel>();
        phy->SetFrameCaptureModel(frameCapture);
    }
    if (m_preambleDetectionModel.front().IsTypeIdSet())
    {
        auto preambleDetection = m_preambleDetectionModel.front().Create<ns3::PreambleDetectionModel>();
        phy->SetPreambleDetectionModel(preambleDetection);
    }

    phy->SetChannel(m_channel);
    phy->SetDevice(device);

    return std::vector<ns3::Ptr<ns3::WifiPhy>>({phy});
}