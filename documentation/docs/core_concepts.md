# Core Concepts of DECT NR+ NS3 Simulation

The core idea of DECT NR+ NS3 simulation is to gather simulation results of MAC-to-MAC communication between
devices. Therefore, the most important concepts to be simulated are:

- PHY layer
- MAC layer
- DECT NR+ Topology

Each script in NS3 consists of defining the core NS3 networking abstractions such as: nodes, net devices, applications, PHY, MAC, channel, and mobility (physical topology). In this documentation section, it will be presented how the core DECT NR+ concepts were abstracted in
the NS3 simulation environment.

## Applications
[ns3::UdpEchoServer](https://www.nsnam.org/docs/release/3.28/doxygen/classns3_1_1_udp_echo_server.html)  
[ns3::UdpEchoClient](https://www.nsnam.org/docs/release/3.28/doxygen/classns3_1_1_udp_echo_client.html)  

## Nodes
[ns3::Node](https://www.nsnam.org/docs/release/3.18/doxygen/classns3_1_1_node.html)

## Channel
[ns3::YansWifiChannel](https://www.nsnam.org/docs/release/3.24/doxygen/classns3_1_1_yans_wifi_channel.html)

## PHY
[ns3::YansWifiPhy](https://www.nsnam.org/docs/release/3.19/doxygen/classns3_1_1_yans_wifi_phy.html)

## MAC
The MAC layer is to be developed in the future.