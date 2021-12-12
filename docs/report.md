# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)
* [Appendix](#7-appendix)

# Abstract

Can you tell me where to go? - Distribution of low-level control in the face of wireless network dynamics

Traditionally Wireless Networked Control Systems (WNCSs), such as those in industrial settings, perform high-level control over the wireless network. Often the data exchanged between the device and the controller does not overload the network. This has led to past work in this area to be either focused on refining high-level control algorithms or have over-simplified the role of network dynamics. In this project, by taking an example of an autonomous robot and a building as a controller, we aim to study the distribution of low-level control between these two entities. The nested objective of this problem is to allow the building’s controller to take over the control of the robot to navigate it to its destination, thereby maintaining the privacy of the building’s infrastructure. We simulate two scenarios - one where the control resides remotely in the building, and second where the building’s control algorithm is running locally on the robot. We design an autonomous robot in Gazebo and control it via a ROS interface while the building's network is simulated using ns3. Using two communication modalities (WiFi and BLE) for the ns3 simulation, we show that the second scenario outperforms the first one for both cases with significantly lower end-to-end latency thereby concluding that the system benefits greatly from a local control algorithm. 

# 1. Introduction

Wireless networked control systems, such as those in industrial settings, utilize the wireless link to perform a high-level control of the devices (in this case, machines). The information exchanged between the device and the remote controller is often insufficient to create a load on the network. In this project, we aim to look at methodologies to perform low-level control from a wireless controller/server, essentially distributing it between the device and the controller. Since directly sending sensor data upstream would beg tighter control loops, the dynamics of the wireless network become more significant. Current work in this space either focuses on implementing high-level control via a wireless controller or over-simplifies the wireless network modeling. In the scenario when low-level (down to the device’s actuators) control is distributed over the network, we need a more precise system modeling to better understand the role that the network dynamics play. We envision that a plethora of applications, such as those in military and delivery systems, would benefit from this.

We frame our problem in a privacy-aware and security-aware fashion. While this is the key novelty element of our approach, we also consider the potential impacts of network dynamics. Here, we use an example of an autonomous robot taking the help of the building’s network to guide it to its destination. The underlying tenet of ensuring the privacy of the building’s infrastructure is fulfilled by exposing intimate control of the robot to the building. To achieve this, we aim to study two scenarios: (1) Building’s controller receives sensor data by the robot and sends navigation commands as downlink messages. In this case, the building’s controller resides in a remote location. (2) When the robot enters the building, during handshake the building sends a control algorithm to the robot. In this case, the control algorithm runs locally on the robot and communicates with the building’s network less frequently or not at all. 

We have outlined the key challenges to be as follows: (1) There hasn’t been much privacy-focused work in this space. In most cases, the autonomous robot requires a priori knowledge of its environment or maps it while it moves. While that adds to the novelty of the problem, it also opens up many challenging yet ultimately unfruitful avenues. (2) Simulators which combine robotics and network modeling are few and far between. (3) Hardware prototyping in robotics is often very time-intensive.

Because of the challenges, we choose to implement the aforementioned scenarios using individual physics and network simulators which we then integrate on a high level. To achieve this we would require in-depth understanding of these softwares and resourcefulness to debug issues we face during the integration phase. Other skills that we rely upon include programming, scripting and data analysis. 

We acknowledge the challenges ROS brings to this project and its learning curve. With our objective to study the distribution of control, we aim to measure our performance based on two key metrics: (1) Simulation of both the scenarios with both the communication modalities, and (2) Proficient analysis and explanation of observed end-to-end latency in all four simulations. 

# 2. Related Work

There is a large amount of prior work in Indoor navigation for autonomous vehicles. For our purposes, we needed a framework to integrate a network simulator and a physics/robotics simulators in order to analyze the performance of an autonomous robot and a building as a controller in a high network load condition. We identified Robonetsim <cite robonetsim url> (from CMU, 2013) that is a framework for seamless integration of network and multi-robot simulator and, building on the same paper, ROSNetSim <cite ROSNetSim IEEE url> (from UPenn, 2021). However, these papers either have too less support to work with state-of-the-art versions of simulators or too new to be stable and work seamlessly without issues.

Moreover, Autonomous navigation is widely studied in Gazebo simulations and the most common library is SLAM (Simultaneous Localization and Mapping). However, we find that it goes against one key motivation of our problem formulation - the robot has no knowledge of the building’s floor plan and nor does it wish to acquire it in a non-adversarial setting. For this reason, we choose to solve the navigation problem by supplying waypoints (next location coordinates) to the robot as the building’s downlink command.

# 3. Technical Approach

Hardware prototyping in the robotics domain is challenging and not a feasible implementation option for a class project. We therefore rely on physical and network simulation environments as a means to analyze distribution of control between the robot and the building’s network. 

While there are many open-source robotics simulators available, we find that Gazebo <cite> has a strong physics backend, a plethora of sensors and a relatively user-friendly GUI making it our robotic simulator of choice. In our first attempt, we built a two-wheeled robot with a camera sensor from scratch <cite purdue> by following this tutorial. However, it was quickly apparent that our barebones robot was not sufficiently suited to our needs. Opting for a smarter approach, we then used <cite actual robot link> to create a newer version with camera, laser and IMU sensors. Moreover, to simulate a building’s floor plan, we designed our own model as seen in Figure X <cite image>. Our custom floor plan helped us circumvent this known bug <cite> in Gazebo <version> where adding doors to the model leads to frequent crashes of the simulator. 

Among different Network Simulators, we chose to go with NS3 as:
1. NS3 has a simple C++ implementation of networks over different nodes and supports many wireless protocols such as WiFi, BLE, LORAWAN, etc.
2. NS3 is a command line based network simulation tool that easily integrated with our integrator script written in bash.
3. It is open source widely used and has a lot of support from the community
4. It was important for us to simulate only 2 nodes so NS3 was a more straightforward choice due to its simple implementation setup as opposed to other, more sophisticated, network simulators that are designed to simulate distributed networks of multiple nodes communicating using different protocols

We chose WiFi and BLE to simulate our 2 scenarios because:
1. WiFi is a widely used and understood protocol with sufficiently large packet size and sufficiently large data rate to support our application with the only exception of being high power consumption and relatively lower privacy and security features.
2. In contrast, we also chose to simulate our two scenarios with BLE as by compromising on the low data rate and packet size slightly, we get a low power communication protocol with less overhead per packet and we get configurability in terms of better security features, with 2 security modes and 4 distinct security levels, and data encryption especially with the latest version BLEv5.0 .

The gazebo simulation is used with a ROS interface and follows a subscriber/publisher model. This allows us to query the laser sensor values of the robot as well as issue a command for its navigation. 

Figure X outlines the system design for Scenario 1. In this case, the robot periodically posts its sensor values (specifically laser scan sensor’s readings) onto the ROS Subscriber. These values are retrieved by the Integrator using the rostopic command. The Integrator, as the name suggests, is a module to seamlessly integrate the two major system components i.e. the network and the physics simulator. Once it receives the required data, it promptly triggers the network simulation in NS3. We simulate a single link between a client node (in this case, the robot) and the server node (in this case, the building’s network) using one of the two communication modalities - WiFi and BLE. To demonstrate sufficient load on the network because of the low-level sensor data, we send 50 2048-byte packets (for WiFi) and 400 255-byte packets (for BLE) as uplink messages. (Please see Appendix for network load calculation). Based on these sensor readings, the server node calculates the next position of the robot and sends that to the client node as a downlink command. Since this is a fairly small payload, we simulate a single 2048-byte packet for WiFi and 8 255-byte packets for BLE. The Integrator then formulates a ROS command from the downlink message and issues it to the Publisher module. This command is picked up by Gazebo and is acted upon by moving the robot to said location. As soon as the execution is complete, new sensor readings are published on the ROS terminal and the process is repeated until the robot reaches its destination.

Scenario 2 is described using the system model in Figure X. Even though it has the same system components as Scenario 1, there are two key differences. First, we model the exchange of the building’s control module (to be run locally on the robot) using a one-time NS3 simulation at the very beginning. The downlink payload of 500 2048-byte packets (for WiFi) and 500 2048-byte packets (for BLE) is used to denote the large size of the control module firmware/file. Second, while the same logic as the 1st scenario is used to compute the next position of the robot, it is run locally, in the same context as Gazebo to demonstrate local computation. 

  
# 4. Evaluation and Results
  
To ensure the reliability of the system and our results, we run 10 iterations of each scenario for both communication modalities. While energy consumption and memory footprint are good performance metrics, as a first step we only consider end-to-end latency. We understand that packet losses are also a significant consequence of the network overload. However, by assuming that WiFi and BLE support internal link layer retries, we consider that no packet loss was seen in the system and any effect will be modeled into the system latency.

To ensure the reliability of the system and our results, we run 10 iterations of each scenario for both communication modalities. Below are our finds:

* Average time to destination for both scenarios using WiFi as the link between the robot and the building
* Average time to destination for both scenarios using BLE as the link between the robot and the building


Scenario 1 total end-to-end latency
Scenario 1 latency split, explain each type
Scenario 2 total end-to-end latency
Scenario 2 latency split, explain each type
Add graphs comparing multiple runs of simulations
  
# 5. Discussion and Conclusions
  
Talk about division of control in autonomous systems
Talk about why latency is a good metric - fast-changing environment need to quickly adapt
We observe that the second scenario gives better results in terms of latency but also has advantages in terms of privacy wrt the building
This software can run on a hardware enclave, signed software to be protected from the robot’s operating system
As an extension of this work, we could
Measure other metrics - power consumption, memory footprint, multi dimensional performance
Use a hybrid approach - some control on the network, some on the local device
  
# 6. References
Gazebo
Ns3
ROS
Previous work
  
# 7. Appendix

  Calculation for network load:
  
* 802.11/Scenario 1: 

    Size of /scan data published by robot = ~16500 bytes
    Size of each packet sent upstream = 2048 bytes
    Number of packets sent upstream = 50
    Total size of uplink message = 50 * 2048 = 51200 bytes
    Size of headers and auth mechanisms = 51200 - 16500 = 34700 bytes
    Size of per packet header and auth = 34700/50 = 694 bytes
    Size of per packet data payload = 1354 bytes
  
* 802.11/Scenario 2: 
    
    Assuming the size of the firmware file  = ~1 MB
    Size of each packet sent downstream = 2048 bytes
    Number of packets required to be sent downstream = 1024000/2048 = 500