# DECT NR+ NS3 Technical Reference

## Applications

[ns3::UdpEchoServer](https://www.nsnam.org/docs/release/3.28/doxygen/classns3_1_1_udp_echo_server.html)  
[ns3::UdpEchoClient](https://www.nsnam.org/docs/release/3.28/doxygen/classns3_1_1_udp_echo_client.html)  

## Nodes
[ns3::Node](https://www.nsnam.org/docs/release/3.18/doxygen/classns3_1_1_node.html)

## Channel
#### `class DectChannel()`
### Description
Manages and creates wifi channel objects for the YANS PHY  
The implementation is based on:
[ns3::YansWifiHelper](https://www.nsnam.org/docs/release/3.18/doxygen/classns3_1_1_yans_wifi_phy_helper.html)

### Members
**private:**  
**`std::vector<ns3::ObjectFactory> m_propagationLoss`**  
**`ns3::ObjectFactory m_propagationDelay`**

### Methods
**public:**  

---
#### `DectChannel::Default()`
Creates the channel object with:

- Constant speed propagation delay model
- Log distance propagation loss model

**returns** DectChannel

---
#### `DectChannel::AddPropagationLoss()`
A template method that appends **DectChannel::m_propagationLoss** vector

---
#### `DectChannel::SetPropagationDelay()`
A template method that assigns propagation delay model to **DectChannel::m_propagationDelay**

---
#### `DectChannel::Create()`
Creates 
[ns3::YansWifiChannel](https://www.nsnam.org/docs/release/3.24/doxygen/classns3_1_1_yans_wifi_channel.html)
and sets the corresponding **m_propagationLoss** and **m_propagationDelay** values to the created YansWifiChannel

**returns**
[ns3::YansWifiChannel](https://www.nsnam.org/docs/release/3.24/doxygen/classns3_1_1_yans_wifi_channel.html)

---
## PHY
#### **`class DectPhy()`**
### Description
Creates and manages the PHY objects for YANS model  
The implementation is based on:
[ns3::YansWifiPhyHelper](https://www.nsnam.org/docs/release/3.18/doxygen/classns3_1_1_yans_wifi_phy_helper.html#details)

### Inheritance
[ns3::WifiPhyHelper](https://www.nsnam.org/docs/release/3.19/doxygen/classns3_1_1_wifi_phy_helper.html)

### Members
**public:**  
**`ns3::Ptr<ns3::YansWifiChannel> m_channel`**

### Methods
**public:**  

---
#### **`DectPhy::SetChannel(ns3::Ptr<YansWifiChannel> channel)`**  
Assigns the channel function argument to the private member **m_channel**

---

**private:**  

---
#### **`DectPhy::Create(ns3::Ptr<ns3::Node>)`**  
A pure virtual function that has to be overridden in the DectPhy derived class

---

## MAC
The MAC layer is to be developed in the future.