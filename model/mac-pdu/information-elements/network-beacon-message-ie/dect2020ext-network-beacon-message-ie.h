#ifndef DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_H
#define DECT2020EXT_NETWORK_BEACON_MESSAGE_IE_H

#include "ns3/header.h"

#include <vector>

namespace ns3
{

class Dect2020ExtNetworkBeaconMessageIE : public Header
{
	public:
		enum Dect2020ExtNetworkBeaconPeriod // ETSI TS 103 636-4 V 1.51 #6.4.2.2-1
		{
			NETWORK_BEACON_PERIOD_50MS = 0,
			NETWORK_BEACON_PERIOD_100MS = 1,
			NETWORK_BEACON_PERIOD_500MS = 2,
			NETWORK_BEACON_PERIOD_1000MS = 3,
			NETWORK_BEACON_PERIOD_1500MS = 4,
			NETWORK_BEACON_PERIOD_2000MS = 5,
			NETWORK_BEACON_PERIOD_4000MS = 6
		};

		enum Dect2020ExtClusterBeaconPeriod // ETSI TS 103 636-4 V 1.51 #6.4.2.2-1
		{
			CLUSTER_BEACON_PERIOD_10MS = 0,
			CLUSTER_BEACON_PERIOD_50MS = 1,
			CLUSTER_BEACON_PERIOD_100MS = 2,
			CLUSTER_BEACON_PERIOD_500MS = 3,
			CLUSTER_BEACON_PERIOD_1000MS = 4,
			CLUSTER_BEACON_PERIOD_1500MS = 5,
			CLUSTER_BEACON_PERIOD_2000MS = 6,
			CLUSTER_BEACON_PERIOD_4000MS = 7,
			CLUSTER_BEACON_PERIOD_8000MS = 8,
			CLUSTER_BEACON_PERIOD_16000MS = 9,
			CLUSTER_BEACON_PERIOD_32000MS = 10
		};

		Dect2020ExtNetworkBeaconMessageIE();
		virtual ~Dect2020ExtNetworkBeaconMessageIE();

		// Overloaded methods from the Header class
		static TypeId GetTypeId();
		virtual TypeId GetInstanceTypeId() const override;
		virtual uint32_t GetSerializedSize() const;
		virtual void Serialize(Buffer::Iterator start) const;
		virtual uint32_t Deserialize(Buffer::Iterator start);
		virtual void Print(std::ostream& os) const;

		// Setters and Getters
		void SetTxPowerIncluded(bool txPowerIncluded);
		bool GetTxPowerIncluded() const;
		void SetPowerConstraints(bool powerConstraints);
		bool GetPowerConstraints() const;
		void SetCurrentClusterChannelIncluded(bool currentClusterChannelIncluded);
		bool GetCurrentClusterChannelIncluded() const;
		void SetNetworkBeaconChannels(uint8_t networkBeaconChannels);
		uint8_t GetNetworkBeaconChannels() const;
		void SetNetworkBeaconPeriod(Dect2020ExtNetworkBeaconPeriod networkBeaconPeriod);
		Dect2020ExtNetworkBeaconPeriod GetNetworkBeaconPeriod() const;
		void SetClusterBeaconPeriod(Dect2020ExtClusterBeaconPeriod clusterBeaconPeriod);
		Dect2020ExtClusterBeaconPeriod GetClusterBeaconPeriod() const;
		void SetNextClusterChannel(uint16_t nextClusterChannel);
		uint16_t GetNextClusterChannel() const;
		void SetTimeToNext(uint32_t timeToNext);
		uint32_t GetTimeToNext() const;
		void SetClustersMaxTxPower(uint8_t clustersMaxTxPower);
		uint8_t GetClustersMaxTxPower() const;
		void SetCurrentClusterChannel(uint16_t currentClusterChannel);
		uint16_t GetCurrentClusterChannel() const;
		void SetAdditionalNetworkBeaconChannels(uint16_t additionalNetworkBeaconChannels[3]);
		uint16_t* GetAdditionalNetworkBeaconChannels();

	private:
		// ETSI TS 103 636-4 V 1.51 #6.4.2.2-1

		bool m_txPowerIncluded; // 1 relevant bit
		bool m_powerConstraints; // 1 relevant bit
		bool m_currentClusterChannelIncluded; // 1 relevant bit
		uint8_t m_networkBeaconChannels; // 2 relevant bits
		Dect2020ExtNetworkBeaconPeriod m_networkBeaconPeriod; // 4 relevant bits
		Dect2020ExtClusterBeaconPeriod m_clusterBeaconPeriod; // 4 relevant bits
		uint16_t m_nextClusterChannel; // 13 relevant bits
		uint32_t m_timeToNext; // 32 relevant bits
		uint8_t m_clustersMaxTxPower; // 4 relevant bits
		uint16_t m_currentClusterChannel; // 13 relevant bits
		uint16_t m_additionalNetworkBeaconChannels[3]{}; // 13 relevant bits
};

} // namespace ns3

#endif