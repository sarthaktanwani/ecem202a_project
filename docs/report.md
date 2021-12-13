# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [Individual Contribution](#6-individual-contribution)
* [References](#7-references)
* [Appendix](#8-appendix)

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

There are lots of physics simulators that can be used such as Gazebo[1], pybullet[2] and Mujoco[3]. Autonomous navigation has been widely studied in different contexts and applications. For e.g., [4] - This paper proposes automatic robot navigation systems and aims to achieve robot navigation that is adaptive in environment crowded with many people. It uses VREP and Fabot2D to simulate the surroundings. [5] - This is another article where the authors propose a neuro-inspired cognitive navigation framework for robot navigation, in this case, the virtual world for the robot on each stage on the middleware is created on the Gazebo Simulator.

Autonomous navigation is widely studied in Gazebo simulations and the most common library is SLAM (Simultaneous Localization and Mapping). However, we find that it goes against one key motivation of our problem formulation - the robot has no knowledge of the building’s floor plan and nor does it wish to acquire it in a non-adversarial setting. For this reason, we choose to solve the navigation problem by supplying waypoints (next location coordinates) to the robot as the building’s downlink command.

There is a large amount of prior work in Indoor navigation for autonomous vehicles. For e.g., [6] Exploring Edge Computing for Multitier Industrial Control - This interesting paper leverages edge computing by switching control between local and wireless entities in the face of adverse wireless conditions. For our purposes, we needed a framework to integrate a network simulator and a physics/robotics simulator in order to analyze the performance of an autonomous robot and a building as a controller in a high network load condition. We identified Robonetsim [7] (from CMU, 2013) that is a framework for seamless integration of network and multi-robot simulator and, building on the same paper, ROSNetSim [8] (from UPenn, 2021). RoboNetSim combines the physics simulator ARGoS and NS3 for its application. The current available version still uses ARGoS2 and NS3.16 that were released around the same time in 2012. Hence, it still has dependacies that require the use of old and unsupported versions of ARGoS, NS3, and the underlying OS i.e. Ubuntu Linux. ROSNetSim was released very recently in April 2021 and currently has issues with installation on the latest Ubuntu versions (20.04, 18). It is under development. Hence, these papers either have too less support to work with state-of-the-art versions of simulators or too new to be stable and work seamlessly without issues.

For communication between the autonomous robot and the building controller, we identified WiFi and BLE would be a good set of wireless technologies covering a wide range of application requirements. WiFi is a widely used and understood protocol with sufficiently large packet size and sufficiently large data rate to support our application with the only exception of being high power consumption and relatively lower privacy and security features. In contrast with WiFi, we also chose to simulate our two scenarios with BLE. It is also widely used in battery powered IoT application as a primary means of communication, as opposed to WiFi, due to its low power requirement.By compromising on the data rate and packet size slightly, we get a low-power communication protocol with less overhead per packet and configurability in terms of better security features, with 2 security modes and 4 distinct security levels, and data encryption especially with the latest version BLEv5.

# 3. Technical Approach

Hardware prototyping in the robotics domain is challenging and not a feasible implementation option for a class project. We therefore rely on physical and network simulation environments as a means to analyze distribution of control between the robot and the building’s network. 

While there are many open-source robotics simulators available, we find that Gazebo <cite> has a strong physics backend, a plethora of sensors and a relatively user-friendly GUI making it our robotic simulator of choice. In our first attempt, we built a two-wheeled robot with a camera sensor from scratch <cite purdue> by following this tutorial. However, it was quickly apparent that our barebones robot was not sufficiently suited to our needs. Opting for a smarter approach, we then used <cite actual robot link> to create a newer version with camera, laser and IMU sensors. Moreover, to simulate a building’s floor plan, we designed our own model as seen in Figure 1. Our custom floor plan helped us circumvent this known bug <cite> in Gazebo <version> where adding doors to the model leads to frequent crashes of the simulator. 

  ![gazebo-simulation](https://user-images.githubusercontent.com/90207206/145735829-5cc65e8a-c687-4ced-b862-a479fa81f174.png)

Among different network simulators, we chose to go with NS3 as:
1. NS3 has a simple C++ implementation of networks over different nodes and supports many wireless protocols such as WiFi, BLE, LORAWAN, etc.
2. NS3 is a command line based network simulation tool that easily integrated with our integrator script written in bash.
3. It is open source widely used and has a lot of support from the community
4. It was important for us to simulate only 2 nodes so NS3 was a more straightforward choice due to its simple implementation setup as opposed to other, more sophisticated, network simulators that are designed to simulate distributed networks of multiple nodes communicating using different protocols

The gazebo simulation is used with a ROS interface and follows a subscriber/publisher model. This allows us to query the laser sensor values of the robot as well as issue a command for its navigation. To perform low-level control of the robot, we try to issue position commands for its three components - left wheel, right wheel and base, simultaneously. However, ROS only accepts a single command for an individual component at a time resulting in an unstable rendering of the robot (continuously rolling) in Gazebo. For this reason, we are limited to issuing the next position of the robot’s model as a whole.

  ![system-design](https://user-images.githubusercontent.com/90207206/145730591-764e63ae-cee5-493e-8360-c92e80843530.JPG)
  
Figure 2 outlines the system design for Scenario 1. In this case, the robot periodically posts its sensor values (specifically laser scan sensor’s readings) onto the ROS Subscriber. These values are retrieved by the Integrator using the rostopic command. The Integrator, as the name suggests, is a module to seamlessly integrate the two major system components i.e. the network and the physics simulator. Once it receives the required data, it promptly triggers the network simulation in NS3. We simulate a single link between a client node (in this case, the robot) and the server node (in this case, the building’s network) using one of the two communication modalities - WiFi and BLE. To demonstrate sufficient load on the network because of the low-level sensor data, we send 50 2048-byte packets (for WiFi) and 400 255-byte packets (for BLE) as uplink messages. (Please see Appendix for network load calculation). Based on these sensor readings, the server node calculates the next position of the robot and sends that to the client node as a downlink command. Since this is a fairly small payload, we simulate a single 2048-byte packet for WiFi and 8 255-byte packets for BLE. The Integrator then formulates a ROS command from the downlink message and issues it to the Publisher module. This command is picked up by Gazebo and is acted upon by moving the robot to said location. As soon as the execution is complete, new sensor readings are published on the ROS terminal and the process is repeated until the robot reaches its destination.
  
![system-design-2](https://user-images.githubusercontent.com/90207206/145730605-26609b8f-54bf-46a4-957a-bf4eac607d48.JPG)

Scenario 2 is described using the system model in Figure 3. Even though it has the same system components as Scenario 1, there are two key differences. First, we model the exchange of the building’s control module (to be run locally on the robot) using a one-time NS3 simulation at the very beginning. The downlink payload of 500 2048-byte packets (for WiFi) and 4000 255-byte packets (for BLE) is used to denote the large size of the control module firmware/file. Second, while the same logic as the 1st scenario is used to compute the next position of the robot, it is run locally, in the same context as Gazebo to demonstrate local computation. 

  
# 4. Evaluation and Results
  
To ensure the reliability of the system and our results, we run 10 iterations of each scenario for both communication modalities. While energy consumption and memory footprint are good performance metrics, as a first step we only consider end-to-end latency. We understand that packet losses are also a significant consequence of the network overload. However, by assuming that WiFi and BLE support internal link layer retries, we consider that no packet loss was seen in the system and any effect will be modeled into the system latency.

## WiFi as the link between the robot and the building
  
  ### Average time to destination
  
We observe that in Scenario 1 the robot takes 260 seconds on average to reach its destination while in Scenario 2 it takes 245 seconds on average. This 5% decrease can be attributed to the one-time network cost in Scenario 2 whereas the robot needs to send (significantly smaller) uplink messages every step of the way in Scenario 1.
  
  ![wifi-average-simulation-latency](https://user-images.githubusercontent.com/90207206/145735841-9fbffe98-dc81-4ccd-b98c-8d40d5844b55.JPG)
  
Additionally, graph 1 shows the average completion time for each iteration of both the scenarios. It is interesting to note that while Scenario 1 has little to no latency variability between iterations, this is not the case for Scenario 2. We believe that this system behavior stems from Gazebo’s reaction time i.e. as the interval between two issued commands decreases, Gazebo gets slower to react/execute the command. Although this is clearly a simulation limitation, it is important to remember that system reactivity is a crucial constraint in real systems as well.

### Breakdown of end-to-latency
  
   ![wifi-latency-breakdown](https://user-images.githubusercontent.com/90207206/145730701-39cd930a-0671-4c46-8d6f-a112ea8f7a70.JPG)
  
Diving deeper into the split of the observed end-to-end latency, we prove our hypothesis of the key differentiating factor being the network component. Graph 2 demonstrates the latency breakdown for one simulation for both scenarios. As compared to Scenario 2, the network latency component is significant (albeit relatively small compared to Gazebo’s contribution).


  ## BLE as the link between the robot and the building
  
  ### Average time to destination
  
  We observe that in Scenario 1 the robot takes 470 seconds on average to reach its destination while in Scenario 2 it takes 280 seconds on average. Similar to WiFi, this 40% reduction can be accounted by the one-time network latency.
  
  ![ble-average-simulation-latency](https://user-images.githubusercontent.com/90207206/145736239-4808cfb0-591a-404b-b1c9-43c9a6f27510.png)

  Graph 3 shows the average completion time for each iteration of both the scenarios. Our rationale for latency variability of Scenario 2 remains the same as for WiFi. In contrast to WiFi simulations, however, there a much larger gap between the latency values of the two scenarios. We believe that this is the results of BLE itself, since we are limited to sending only 255-byte packets for the same amount of payload which is a considerable drop from 2048-byte packets in WiFi simulations.
  
  ### Breakdown of end-to-latency
  
  ![ble-latency-breakdown](https://user-images.githubusercontent.com/90207206/145736404-37f1f2d8-7bba-4a6b-bde4-3a68cc6d5ceb.png)

  From graph 4, we notice that network latency takes up a significant portion (almost 40%) of the the end-to-end latency in Scenario 1. This is much larger than the 5% network contribution in Scenario 2.

## Comparing WiFi and BLE
  
  ![wifi-vs-ble](https://user-images.githubusercontent.com/90207206/145736586-d1f63db5-da67-4522-99a4-7e6ace8e912a.JPG)

  From the above graphs, it is evident that WiFi is a faster communication modality than BLE which aligns with our understanding of the two technologies. Interestingly, while WiFi is almost 50% faster than BLE for Scenario 1, the latency difference is not nearly as significant for Scenario 2. This is because the network overhead of sending sensor data in each step quickly adds up which is not the case for one-time transfer of building's control module. Additionally, the number of packets exchanged for BLE is 4x WiFi's because of the payload restriction.

  
# 5. Discussion and Conclusions
  
This project adopts a different perspective of distribution of control in navigation of autonomous systems. We derive our motivation from the recent work around the vulnerabilities of autonomous systems and frame our problem in the context of wireless control. Fast-changing environments such as these require an equally adaptive system which leads us to choose network and system latency as our performance metrics. 

Local control of the robot i.e. Scenario 2 outperforms Scenario 1 i.e. wireless control of the robot in terms of navigation time with network latency being the key differentiating factor. Moreover, we find that control over BLE proves to be significantly slower than WiFi which is on par with what we know about both these communication technologies.
  
Although in Scenario 2 the building’s controller is running locally on the robot, we envision it to be separate from the robot’s controller. The control module can be sent as a signed firmware package to ensure authenticity of the image. Moreover, this software can run in a secure mode (for example, a hardware enclave) to maintain separation from the robot’s firmware.As a future direction of this work, we would like to analyze the multi-dimensional performance of the system by measuring other metrics such as power consumption and memory footprint. Additionally, it would be interesting to study the effects of network variability for a hybrid control approach where part of the control lies with the robot and part with the building.
  
# 6. Individual Contribution
  
  ### Pragya
  * Gazebo simulation, robot, floorplan
  * Data processing scipts
  * Data processing, graphs, inferences
  * Project report (80% contribution)
  * Project presentation (80% contribution)
  
  ### Sarthak
  * ns3 simulation: BLE and WiFi
  * Integrator scripts: BLE and WiFi
  * Project report (20% contribution)
  * Project presentation (20% contribution)
  * Project website
  
# 7. References

[1] Link: https://ieeexplore.ieee.org/document/1389727; N. Koenig and A. Howard, "Design and use paradigms for Gazebo, an open-source multi-robot simulator," 2004 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS) (IEEE Cat. No.04CH37566), 2004, pp. 2149-2154 vol.3, doi: 10.1109/IROS.2004.1389727.
Website: http://gazebosim.org/
GitHub: https://github.com/osrf/gazebo

[2]Pybullet Links: website: https://pybullet.org/wordpress/; GitHub: https://github.com/bulletphysics/bullet3

[3] Link: https://ieeexplore.ieee.org/document/6386109; E. Todorov, T. Erez and Y. Tassa, "MuJoCo: A physics engine for model-based control," 2012 IEEE/RSJ International Conference on Intelligent Robots and Systems, 2012, pp. 5026-5033, doi: 10.1109/IROS.2012.6386109.
Website: https://mujoco.org/
GitHub: https://github.com/openai/mujoco-py

[4] https://ieeexplore.ieee.org/document/8279360; Y. Kato, K. Kamiyama and K. Morioka, "Autonomous robot navigation system with learning based on deep Q-network and topological maps," 2017 IEEE/SICE International Symposium on System Integration (SII), 2017, pp. 1040-1046, doi: 10.1109/SII.2017.8279360.

[5] Joo, S.-H.; Manzoor, S.; Rocha, Y.G.; Bae, S.-H.; Lee, K.-H.; Kuc, T.-Y.; Kim, M. Autonomous Navigation Framework for Intelligent Robots Based on a Semantic Environment Modeling. Appl. Sci. 2020, 10, 3219. https://doi.org/10.3390/app10093219

[6] https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=9211472; Y. Ma, C. Lu, B. Sinopoli and S. Zeng, "Exploring Edge Computing for Multitier Industrial Control," in IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, vol. 39, no. 11, pp. 3506-3518, Nov. 2020, doi: 10.1109/TCAD.2020.3012648.

[7] Michal Kudelski, Luca M. Gambardella, Gianni A. Di Caro, RoboNetSim: An integrated framework for multi-robot and network simulation, Robotics and Autonomous Systems, Volume 61, Issue 5, 2013, Pages 483-496, ISSN 0921-8890, https://doi.org/10.1016/j.robot.2013.01.003.
Website: http://www.giannidicaro.com/robonetsim.html
Link: https://www.sciencedirect.com/science/article/pii/S0921889013000080;

[8] https://ieeexplore.ieee.org/document/9345354; M. Calvo-Fullana, D. Mox, A. Pyattaev, J. Fink, V. Kumar and A. Ribeiro, "ROS-NetSim: A Framework for the Integration of Robotic and Network Simulators," in IEEE Robotics and Automation Letters, vol. 6, no. 2, pp. 1120-1127, April 2021, doi: 10.1109/LRA.2021.3056347.

  
# 8. Appendix

  ## Calculation for network load:
  
While both WiFi and BLE are standardized protocols and have predefined headers and authentication mechanisms, we assume that our communication requires additional headers to ensure safe and reliable exchange of the robot's sensitive sensor data. For this reason, we calculate payload and header split based on generated sensor readings of the robot's laser scanner.
Please note that we found WiFi simulations to be unstable when packet count was over 100 which is why we decided to split the payload into 50 packets of 2048 bytes each. Moreover, the BLE calculation is done keeping in mind that the maximum packet payload is limited to 255 bytes.

  
### 802.11/Scenario 1: 

    Size of /scan data published by robot = ~16500 bytes
    Size of each packet sent upstream = 2048 bytes
    Number of packets sent upstream = 50
    Total size of uplink message = 50 * 2048 = 102400 bytes
    Size of headers and auth mechanisms = 102400 - 16500 = 85900 bytes
    Size of per packet header and auth = 85900/50 = 1718 bytes
    Size of per packet data payload = 330 bytes

  
### 802.11/Scenario 2: 
    
    Assuming the size of the firmware file  = ~1 MB
    Size of each packet sent downstream = 2048 bytes
    Number of packets required to be sent downstream = 1024000/2048 = 500
  
 ### BLE/Scenario 1: 

    Size of /scan data published by robot = ~16500 bytes
    Size of each packet sent upstream = 255 bytes
    Number of packets sent upstream = 400
    Total size of uplink message = 400 * 255 = 102400 bytes
    Size of headers and auth mechanisms = 102400 - 16500 = 85900 bytes
    Size of per packet header and auth = 85900 /400 = 214 bytes
    Size of per packet data payload = 41 bytes
  
 ### BLE/Scenario 2: 
  
    Assuming the size of the firmware file  = ~1 MB
    Size of each packet sent downstream = 255 bytes
    Number of packets required to be sent downstream = 1024000/255 = 4000

