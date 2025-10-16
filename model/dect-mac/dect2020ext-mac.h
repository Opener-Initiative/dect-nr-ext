#ifndef DECT2020EXT_MAC_H
#define DECT2020EXT_MAC_H

#include "ns3/dect2020ext-net-device.h"
#include "ns3/dect2020ext-channel.h"
#include "ns3/dect2020ext-phy.h"
#include "ns3/dect2020ext-channel-manager.h"

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include <map>
#include <vector>

namespace ns3
{

class Dect2020ExtChannelManager;
class Dect2020ExtNetDevice;
class Dect2020ExtPhy;

// struct ChannelEvaluation
// {
//     uint16_t channelId;
//     uint32_t free = 0;
//     uint32_t possible = 0;
//     uint32_t busy = 0;

//     uint32_t Total() const
//     {
//         return free + possible + busy;
//     }
// };

// struct SubslotScanContext
// {
//     uint32_t channelId;
//     uint32_t subslotCount; // Counter for how many Subslots have been measured
//     ChannelEvaluation evaluation;
//     std::function<void(const ChannelEvaluation&)>
//         onComplete; // Callback to be called when the scan is complete (OperatingChannelSelection,
//                     // LBT, ...)
// };

class Dect2020ExtMac : public Object
{

public:
    Dect2020ExtMac();
    virtual ~Dect2020ExtMac();

    static TypeId GetTypeId(void);

    void OperatingChannelSelection(void);
    void ScheduleBackgroundScan(Time delay);
    void OnRssiMeasurement(uint16_t channelId, double rssiDbm);
    void MeasureRSSIOnChannel(Ptr<Dect2020ExtChannel> channel, int64_t frameDurationNs, uint32_t numSubslots);
    void BackgroundScan(uint32_t numSubslots, std::vector<Ptr<Dect2020ExtChannel>> validChannels, uint16_t numOfValidChannels);

    // Setters
    void SetNetDevice(Ptr<Dect2020ExtNetDevice> device);
    void SetPhy(Ptr<Dect2020ExtPhy> phy);
    void SetChannelManager(Ptr<Dect2020ExtChannelManager> channelManager);

private:
    Ptr<Dect2020ExtNetDevice> m_netDevice;
    Ptr<Dect2020ExtPhy> m_phy;
    std::map<uint16_t, std::vector<double>> m_rssiByChannel;
    Ptr<Dect2020ExtChannelManager> m_channelManager;

    // std::map<uint32_t, ChannelEvaluation> m_scanEvaluations;

    // ####################
    // ##### Constants ####
    // ####################

    // ETSI TS 103 636-4 V2.1.1 #7.2-1
    const double RSSI_THRESHOLD_MIN = -85; // dBm
    const double RSSI_THRESHOLD_MAX = -50; // dBm TODO: RSSI_THRESHOLDMAX = -52 - max TX Power of RD
    const int SCAN_MEAS_DURATION = 24;     // slots
    const double SCAN_SUITABLE = 0.75;     // Threshold when an operating channel can be considered
                                           // fulfilling the operating conditions.
    const int SCAN_STATUS_VALID = 300;     // seconds
};

} // namespace ns3

#endif /* DECT2020EXT_MAC_H */