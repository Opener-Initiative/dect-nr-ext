#ifndef DECT_CHANNEL_H
#define DECT_CHANNEL_H 

#include "ns3/yans-wifi-channel.h"
#include "ns3/object-factory.h"
#include "ns3/pointer.h"
#include "ns3/object.h"

#include <string>

class DectChannel
{
    public:
        DectChannel();
        ~DectChannel();

        static DectChannel Default();

        template <typename... Ts>
        void AddPropagationLoss(std::string name, Ts&&... args);

        template <typename... Ts>
        void SetPropagationDelay(std::string name, Ts&&... args);

        ns3::Ptr<ns3::YansWifiChannel> Create() const;

    private:
        std::vector<ns3::ObjectFactory> m_propagationLoss;
        ns3::ObjectFactory m_propagationDelay;
};

template <typename... Ts>
void
DectChannel::AddPropagationLoss(std::string name, Ts&&... args)
{
    m_propagationLoss.push_back(ns3::ObjectFactory(name, std::forward<Ts>(args)...));
}

template <typename... Ts>
void
DectChannel::SetPropagationDelay(std::string name, Ts&&... args)
{
    m_propagationDelay = ns3::ObjectFactory(name, std::forward<Ts>(args)...);
}

#endif /* DECT_CHANNEL_H */