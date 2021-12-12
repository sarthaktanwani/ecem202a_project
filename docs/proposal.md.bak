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

The rationale behind identifying the following as key papers was to either understand the recent work in networked control systems or to identify the gaps in current systems (from which we eventually dervice our project's problem statement):

[1] Exploring Edge Computing for Multitier Industrial Control - This interesting papers leverages edge computing by switching control between local and wireless entities in the face of adverse wireless conditions. 

[2] Control as a service (CaaS): cloud-based software architecture for automotive control applications -  This paper offers a cloud-based throttle profile controller for a vehicle as a PoC. They find that network variation indeed play a significant role in their system which they wish to explore as future work.

[3] Networked Control Systems: Estimation and Control over Lossy Networks - Even though this paper talks about networked control system and network dynamics in particular, it only focuses on one component of the network (packet losses) and over-simplifies the rest of the modeling.

[4] Closing the loop: Architectures and algorithms for real-time control over wireless networks - The author of thesis explores networked control in industrial and medical settings.


### 9.b. Datasets

We currently do not forsee requiring a dataset for this project.

### 9.c. Software

We are analyzing the following software packages:

1. ROS-NetSim
2. RoboNetSim
3. Network Simulators
   1. NS-3
   2. ReNode
   3. MiniNet
4. Robotics Simulators
   1. Gazebo
   2. Pybullet
   3. Mujoco

## 10. References

Key Papers :

[1] https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=9211472; Y. Ma, C. Lu, B. Sinopoli and S. Zeng, "Exploring Edge Computing for Multitier Industrial Control," in IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, vol. 39, no. 11, pp. 3506-3518, Nov. 2020, doi: 10.1109/TCAD.2020.3012648.

[2] https://dl.acm.org/doi/10.1145/2756755.2756758; H. Esen, M. Adachi, D. Bernardini, A. Bemporad, D. Rost and J. Knodel, "Control as a service (CaaS): Cloud-based software architecture for automotive control applications", Proc. 2nd Int. Workshop Swarm at Edge Cloud, pp. 13-18, Apr. 2015

[3] https://web.ece.ucsb.edu/~hespanha/published/LossyNetworksArticlev3.pdf; J. P. Hespanha and A. R. Mesquita, "Networked Control Systems: Estimation and Control over Lossy Networks", Encyclopedia of Systems and Control, pp. 842-849, 2015. 

[4] https://www.proquest.com/docview/1288377691?fromopenview=true&pq-origsite=gscholar&accountid=14512; Pajic M. Closing the loop: Architectures and algorithms for real-time control over wireless networks. University of Pennsylvania; 2012.

Simulation software:

[1] Gazebo
Link: https://ieeexplore.ieee.org/document/1389727; N. Koenig and A. Howard, "Design and use paradigms for Gazebo, an open-source multi-robot simulator," 2004 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS) (IEEE Cat. No.04CH37566), 2004, pp. 2149-2154 vol.3, doi: 10.1109/IROS.2004.1389727.

[2] Mujoco
Link: https://ieeexplore.ieee.org/document/6386109; E. Todorov, T. Erez and Y. Tassa, "MuJoCo: A physics engine for model-based control," 2012 IEEE/RSJ International Conference on Intelligent Robots and Systems, 2012, pp. 5026-5033, doi: 10.1109/IROS.2012.6386109.

1. ROS-NetSim

Link: https://ieeexplore.ieee.org/document/9345354;
M. Calvo-Fullana, D. Mox, A. Pyattaev, J. Fink, V. Kumar and A. Ribeiro, "ROS-NetSim: A Framework for the Integration of Robotic and Network Simulators," in IEEE Robotics and Automation Letters, vol. 6, no. 2, pp. 1120-1127, April 2021, doi: 10.1109/LRA.2021.3056347.

2. RoboNetSim

Website: http://www.giannidicaro.com/robonetsim.html
Link: https://www.sciencedirect.com/science/article/pii/S0921889013000080;
Michal Kudelski, Luca M. Gambardella, Gianni A. Di Caro, RoboNetSim: An integrated framework for multi-robot and network simulation, Robotics and Autonomous Systems, Volume 61, Issue 5, 2013, Pages 483-496, ISSN 0921-8890, https://doi.org/10.1016/j.robot.2013.01.003.

3. Network Simulators
   
3.1 NS-3

Website: https://www.nsnam.org/
GitHub: https://www.nsnam.org/

3.2 ReNode

Website: https://renode.io/
GitHub: https://github.com/renode/renode

3.3 MiniNet

Website: http://mininet.org/
GitHub: https://github.com/mininet/mininet
Link: https://ieeexplore.ieee.org/document/6860404
R. L. S. de Oliveira, C. M. Schweitzer, A. A. Shinoda and Ligia Rodrigues Prete, "Using Mininet for emulation and prototyping Software-Defined Networks," 2014 IEEE Colombian Conference on Communications and Computing (COLCOM), 2014, pp. 1-6, doi: 10.1109/ColComCon.2014.6860404.

4. Robotics Simulators

4.1 Gazebo

Website: http://gazebosim.org/
GitHub: https://github.com/osrf/gazebo
Link: https://ieeexplore.ieee.org/document/1389727; N. Koenig and A. Howard, "Design and use paradigms for Gazebo, an open-source multi-robot simulator," 2004 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS) (IEEE Cat. No.04CH37566), 2004, pp. 2149-2154 vol.3, doi: 10.1109/IROS.2004.1389727.

4.2 Pybullet

website: https://pybullet.org/wordpress/
GitHub: https://github.com/bulletphysics/bullet3

4.3 Mujoco

Website: https://mujoco.org/
GitHub: https://github.com/openai/mujoco-py
Link: https://ieeexplore.ieee.org/document/6386109; E. Todorov, T. Erez and Y. Tassa, "MuJoCo: A physics engine for model-based control," 2012 IEEE/RSJ International Conference on Intelligent Robots and Systems, 2012, pp. 5026-5033, doi: 10.1109/IROS.2012.6386109.


