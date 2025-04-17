# DECT NR+ NS3 simulation project documentation

[Project's private GitHub repository](https://github.com/seelennebel/dect_nr)  
[ETSI DECT NR+ Specification](https://www.etsi.org/deliver/etsi_ts/103600_103699/10363601/01.03.01_60/ts_10363601v010301p.pdf)

## Introduction
**DECT NR+** is a non-cellular radio standard that has been recognised by ITU-R as an IMT-2020 (5G) standard for Ultra Reliable Low Latency Communication (URLLC) and Massive Machine-Type Communication (mMTC) since autumn 2021. DECT-2020 NR is generally designed to operate in bands below 6 GHz, but has access to the 1.9 GHz range almost worldwide.

The project aims to implement the DECT NR MAC layer functionality (e.g. architecture, device addressing, channel structure, channel access, spectrum management techniques...) in the NS3 simulator in order to perform exhaustive system-level simulations of the DECT NR MAC layer, with the aim of modelling how networked devices share and access the communication medium (e.g. radio channel) - under real-world constraints - and how this affects the performance, scalability and QoS (Quality of Service) of DECT NR networks.

**Main Goals of MAC-Level System Simulations are:**

- Evaluate of protocol efficiency
- Model network throughput and scalability
- Understand algorithm behaviour, such as scheduling, error handling, interference handling, spectrum sharing, etc.
- Explore new algorithms & strategies
- Test practical scenarios before deployment


## [DECT NR+ NS3 Technical Reference](technical_reference.md)  
## [Installation](installation.md)  
## [Simulation](simulation.md)