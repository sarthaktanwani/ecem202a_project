# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)

# Abstract
Can you tell me where to go? - Distribution of low-level control in the face of wireless network dynamics

Traditionally Wireless Networked Control Systems (WNCSs), such as those in industrial settings, perform high-level control over the wireless network. Often the data exchanged between the device and the controller does not overload the network. This has led to past work in this area to be either focused on refining high-level control algorithms or have over-simplified the role of network dynamics. In this project, by taking an example of an autonomous robot and a building as a controller, we aim to study the distribution of low-level control between the two entities. The nested objective of this problem is to allow the building’s controller to take over the actuator-level control of the robot to navigate it to its destination, thereby maintaining the privacy of the building’s infrastructure. We simulate two scenarios - one where the control resides remotely in the building, and second where a part of the building’s control algorithm is running locally on the robot. We design an autonomous robot in Gazebo and control it via a ROS interface while the building's network is simulated using NS3. We show that the second scenario outperforms the first one with <number> lower end-to end latency thereby concluding that the system benefits greatly from a local control algorithm. 

# 1. Introduction

This section should cover the following items:

* Motivation & Objective: What are you trying to do and why? (plain English without jargon)
* State of the Art & Its Limitations: How is it done today, and what are the limits of current practice?
* Novelty & Rationale: What is new in your approach and why do you think it will be successful?
* Potential Impact: If the project is successful, what difference will it make, both technically and broadly?
* Challenges: What are the challenges and risks?
* Requirements for Success: What skills and resources are necessary to perform the project?
* Metrics of Success: What are metrics by which you would check for success?

Wireless networked control systems, such as those in industrial settings, utilize the wireless link to perform a high-level control of the devices (in this case, machines). The information exchanged between the device and the remote controller is often insufficient to create a load on the network. In this project, we aim to look at methodologies to perform low-level control from a wireless controller, essentially distributing control between the device and the controller. Since low-level control would beg tighter control loops, the dynamics of the wireless network become more significant. Current work in this space either focuses on implementing high-level control via a wireless controller <cite> or over-simplifies the wireless network modeling<cite>. In the scenario when low-level (down to the device’s actuators) control is distributed over the network, we need a more precise system modeling to better understand the role that the network dynamics play.
Actuator-level control of an autonomous robot is not very well explored. In addition, we frame our problem in a privacy-aware and security-aware fashion. While this adds novelty to our approach, we also consider the potential impacts of network dynamics. Here, we consider an example of an autonomous robot taking the help of the building’s network to guide it to its destination. A guiding principle of exposing intimate control of the robot to the building is to ensure the privacy of the building infrastructure. To achieve this, we aim to study two scenarios: (1) Building’s controller receives sensor data by the robot and sends commands for its actuators. In this case, the building’s controller resides in a remote location. (2) When the robot enters the building, during handshake the building sends a control algorithm to the robot. In this case, the control algorithm runs locally on the robot and communicates with the building’s network less frequently.
As we see it, this project is a step in the direction of exploring the opportunities offered by distributed low-level control. A plethora of applications, such as those in military and delivery systems, would benefit from this.
We have outlined the key challenges to be as follows: (1) There hasn’t been much work done in this space. While that adds to the novelty of the problem, it also opens up many challenging yet ultimately unfruitful avenues. (2) Simulators which combine robotics and network modeling are few and far between. (3)Hardware prototyping in robotics is often very time-intensive.

<requirements for success> <performance metrics> 


# 2. Related Work
  

# 3. Technical Approach

-Simulation because hardware prototyping in robotics and control theory is hard
-Why Gazebo for a robotic simulator?
-Cite robot link and sensors
-Why NS3 for a network simulator?
-Cite communication technology (wifi?)
-How we combine the two - ROS
-Explain scenario 1 simulation
-Explain scenario 2 simulation

  # 4. Evaluation and Results

-While energy consumption, memory footprint are good performance metrics, we only consider latency to begin with
-Scenario 1 total end-to-end latency
-Scenario 1 latency split, explain each type
-Scenario 2 total end-to-end latency
-Scenario 2 latency split, explain each type
-Add graphs comparing multiple runs of simulations

  # 5. Discussion and Conclusions

-Talk about division of control in autonomous systems
-Talk about why latency is a good metric - fast-changing environment need to quickly adapt
-We observe that the second scenario gives better results in terms of latency but also has advantages in terms of privacy wrt the building
-This software can run on a hardware enclave, signed software to be protected from the robot’s operating system
-As an extension of this work, we could
      -Measure other metrics - power consumption, memory footprint, multi dimensional performance
      -Use a hybrid approach - some control on the network, some on the local device

  # 6. References
  
-Gazebo
-Ns3
-ROS
-Previous work
