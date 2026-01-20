Made by: Andrii Amitan

# Introduction

The purpose of this report is to outline the achievements in the technical implementation of the project. The theoretical part, the ETSI standards, has been discussed multiple times during meetings. However, some important technical details couldn’t be demonstrated. In the report, the differences between Dect2020 and Dect2020Ext, core development approaches, and improvements will be outlined.

# Dect2020Ext vs Dect2020

In February 2025, I started working on DECT NR+ implementation in NS3. While I have been familiarizing myself with the simulator and trying my best to understand DECT NR+, another NS3 implementation of DECT NR+ was shared with me. All DECT NR+ models inside this implementation had the prefix "Dect2020" (like Dect2020Mac). This is the reason I call it “Dect2020” implementation. When I took a look at the source code of Dect2020, I understood that I could learn a lot from it. Also, I decided to make Dect2020 the foundation of my DECT NR+ NS3 project. The only problem for me was that my knowledge and experience with radio, computer networks, and network simulators was way less than the experience of Dect2020’s developer. I made a decision to create a separate NS3 module, which I later called Dect2020Ext (Dect2020 Extension). This was also important to allow Dect2020 and Dect2020Ext to co-exist inside the NS3 CMake build system. Dect2020Ext took a lot from Dect2020; however, there are core differences and improvements that will also be covered in this report.

# Core Development Principles and Approaches behind Dect2020Ext

I made my best to develop and maintain the implementation with the following considerations:

- Maintaining an extensible and configurable code base
	- When I have been analyzing Dect2020, I understood the sophisticated and impressive vision and idea behind the implementation. However, as a developer who was trying to improve Dect2020, I lacked the extensibility features of the module. For example, changing some important constants or MAC-level behavior was extremely hard since that meant that I would have to dive into the implementation and change the code directly without any configuration or extensibility features. The reason for the lack of such features is also understandable to me since there wasn’t so much time allocated for Dect2020 to make it work and be extensible at the same time. With all of this in my mind, I did my best to keep Dect2020Ext extensible and configurable.
- Using the built-in NS3 features for Dect2020Ext development
	- While diving into Dect2020, I noticed a model and one functionality that were done in a custom way, not like other models in NS3. Particularly, the Dect2020ChannelManager and RSSI1 measurements. Further in the report, Dect2020ChannelManager and RSSI1 measurements will be discussed, and how they differ from Dect2020Ext.

# Improvements

## Dect2020Ext Project Structure

Dect2020Ext follows the classic NS3 module structure with the following directories:

- examples - a place where examples (simulations) can be stored and executed from
- helper - a place for helper models to reside in
- model - the main folder that contains the source code for all Dect2020Ext models
- test - a place to store all tests made using NS3 TestSuite

The core improvement in the structure is the function-oriented structuring of the model directory. Here is the structure of Dect2020Ext’s model folder:

- dect-channel
- dect-channel-manager
- dect-mac
- dect-net-device
- dect-phy
- dect-types (currently unused)
- execution-grid (currently in testing)
- mac-pdu  

This improvement allows quick access to files that require editing. This approach also improves the extensibility of the code base.

## Active Maintenance of the GitHub repository

The Dect2020 project didn’t have an appropriate GitHub repository simply due to time restrictions and the project’s scope. I constantly maintain the Dect2020Ext GitHub repository by making frequent commits to track all features that are developed. Also, recently, I started entering some project’s issues to the “Known Issues” section of the GitHub repository. I am doing this to not by any means forget about the issue and leave without a proper fix. In my opinion, an already well-maintained GitHub repository will allow possible future developers to use my code more easily and also have fewer confusing questions about the code base.

## Testing

Dect2020 doesn’t have a dedicated tests folder, probably due to time restrictions. Dect2020Ext has a dedicated test folder where I can write tests for all DECT NR+ models. For example, writing tests for headers and IEs of MAC PDU is useful to test Serialization/Deserialization methods of the models. Having tests allows for bug-free code development and also gives the opportunity for code users to test the models in this version of NS3.

## RSSI1 Measurements

In Dect2020, power on each channel inside the operating band is managed manually:

1. Power is manually put to the std::map channel occupancy data structure to track the occupancy of each channel within the operating band
2. Power is manually taken from the std::map channel occupancy data structure based on the signal’s duration (provided in Dect2020 Spectrum Signal Parameters)

In Dect2020Ext, the improvement was inspired by the NS3 LTE module. In Dect2020Ext, even though this is a relatively new improvement that has to be tested, the power is measured directly from ns3::SingleModelSpectrumChannel and tracked using Dect2020Ext Interference Model. The Interference Model receives all incoming signals at the PHY and stores them for the period of their duration in its ns3::SpectrumValue. The ns3::SpectrumValue data structure represents the power that the device is receiving currently. This makes RSSI1 measurements easier and more NS3-like, which also improves its integration into existing NS3 models. Improved integration also allows me to better learn from existing NS3 models and maybe take some useful concepts from them, just like LTE’s interference model.

If this approach had been discovered at the start of my work or if I had more experience with radio and network simulations, RSSI1 measurements would have been implemented intuitively and would have taken less stress and time to implement.

## RSSI1 Measurement Improvement -> Improved Spectrum Management Procedure

Due to the new RSSI1 measurement improvement, I decided to also change the Spectrum Management Procedure. Particularly, how power is measured on each subslot and then evaluated during the Background Scan. Previously, the Background Scan used so-called ns3::SpectrumAnalyzer, which connected to ns3::SingleModelSpectrumChannel as RX PHY and basically counted received power with a user-defined sampling rate. Using the spectrum analyzer was confusing since it definitely wasn’t designed for such use cases. Using the spectrum analyzer also made the code less readable and intuitive.

The improvement was to use the new RSSI1 measurement feature that comes from the PHY and its interference model. Here is the approximate explanation of the improvement:

1. At the start of each channel’s scan (period of 10ms or one frame), the MAC will tune the device to the appropriate channel
2. The MAC will then schedule sampling power from the PHY interference model with 5 samples for each subslot
3. At the end of each subslot, MAC will schedule another function that would collect all power measurements across the subslot and store the average result

This not only improves the code readability, but in my opinion, also makes the implementation more realistic and NS3-like.

## Execution Grid

The Execution Grid is an improved version of PHY Frame Timer, which would have the capability of scheduling operations at different subslots or slots, advanced time tracking, and synchronization assist. The Execution Grid proved its usefulness in the new implementation of the Spectrum Management Procedure. During the Execution Grid’s initialization process, time offsets (in nanoseconds) for all slots and subslots are counted. This allows for precisely scheduled operations at the start and end of slots and subslots. Moreover, it allows us to count the time till the next frame or the time till a specific slot or subslot in the next frame.

Possible improvements of the Execution Grid will be allowing the model to execute repetitive operations and beacon transmission. Also, I believe that it would be useful for synchronization, but this idea goes far beyond the scope of this report and can be discussed later.