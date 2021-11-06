# Project Proposal

## 1. Motivation & Objective
 
Traditionally wireless networked control systems, such as those in industrial settings, utilize the wireless link to perform a high-level control of the devices (in this case, machines). The information exchanged between the device and the remote controller is often insufficient to create a load on the network. 
In this project, we aim to look at methodologies to perform low-level control from a wireless controller, essentially distributing control between the device and the controller. Since low-level control would beg tighter control loops, the dynamics of the wireless network become more significant.  

## 2. State of the Art & Its Limitations
 
Current work in this space either focuses on implementing high-level control via a wireless controller or over-simplifies the wireless network modeling. In the scenario when low-level (down to the device’s actuators) control is distributed over the network, we need more precise system modeling to better understand the role that the network dynamics play.

## 3. Novelty & Rationale
  
Actuator-level control of an autonomous robot is not very well explored. In addition, we frame our problem in a privacy-aware and security-aware fashion. While this adds novelty to our approach, we also consider the potential impacts of network dynamics. 

Here, we consider an example of an autonomous robot taking the help of the building’s network to guide it to its destination. A guiding principle of exposing intimate control of the robot to the building is to ensure the privacy of the building infrastructure. To achieve this, we aim to study two scenarios:
1. Building’s controller receives sensor data by the robot and sends commands for its actuators. In this case, the building’s controller resides in a remote location.
2. When the robot enters the building, during handshake the building sends a control algorithm to the robot. In this case, the control algorithm runs locally on the robot and communicates with the building’s network less frequently.

## 4. Potential Impact

As we see it, this project is a step in the direction of exploring the opportunities offered by distributed low-level control. A plethora of applications, such as those in military and delivery systems, would benefit from this.

## 5. Challenges

We have outlined the key challenges to be as follows:
1. There hasn’t been much work done in this space. While that adds to the novelty of the problem, it also opens up many challenging yet ultimately unfruitful avenues
2. Simulators which combine robotics and network modeling are few and far between. 
3. Hardware prototyping in robotics is often very time-intensive.

## 6. Requirements for Success

1. Understanding of wireless network dynamics and communication standards such as 802.11ac, 802.15.4 etc
2. Robot modeling and interplay of different sensors and actuators
3. Knowledge of network simulators
4. Programming experience - C/Python is predominantly used


## 7. Metrics of Success

1. Simulation of a tight wireless networked control loop
2. Analysis of latency characteristics in the two aforementioned scenarios

## 8. Execution Plan

Step 1: Currently, both team members are working to analyze different simulation environments and whether they appropriately fit the project’s requirements.

Step 2: The simulation task will be divided into two parts. One team member will work on the network simulator and the other on the robotic simulator. 

Step 3: Once both the simulators are set up, we will work together to integrate the two interfaces.

Step 4: The two scenarios mentioned in the problem statement will be realized, either one by each member or both together. The division of this task depends on the integration and stability of the simulation environment.

Step 5: Finally, we will analyze the results from each scenario. Here, we can each take up different tasks of data processing and parallelize the analysis. 

## 9. Related Work

### 9.a. Papers

[1] Control as a service (CaaS): cloud-based software architecture for automotive control applications
[2] 

### 9.b. Datasets

We currently do not forsee requiring a dataset for this project.

### 9.c. Software

We are analyzing the following software packages:

1. ROS-NetSim
2. Network Simulators
   1. NS-3
   2. ReNode
   3. MiniNet
3. Robotics Simulators
   1. Player/Stage/Gazebo
   2. Pybullet
   3. Mujoco

## 10. References

[1] https://dl.acm.org/doi/10.1145/2756755.2756758; H. Esen, M. Adachi, D. Bernardini, A. Bemporad, D. Rost and J. Knodel, "Control as a service (CaaS): Cloud-based software architecture for automotive control applications", Proc. 2nd Int. Workshop Swarm at Edge Cloud, pp. 13-18, Apr. 2015
[2] 
