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

What are metrics by which you would check for success?
1. Whether we are able to simulate all the different cases of control distribution between 2 robots
2. How different is the network parameters such as latency/packet loss etc in each of the different technologies like WiFi, BLE and Zigbee
3. An understanding of the ideal assumptions taken by the simulator and hence how would the simulated values of network parameters differ from real world scenario


## 8. Execution Plan

Describe the key tasks in executing your project, and in case of team project describe how will you partition the tasks.
Once we are able to setup a simulator, each of us can take a different wireless techology and simulate that scenario and get the network parameters in that case. That would be either WiFi, BLE or Zigbee. We can also partition our tasks in a way that each of us would simulate a different case of control distribution between the 2 robots. Basically, In each case, a different amount of navigation information about the building is sent to the robot keeing privacy and security in mind. As of now, we have 2 different such cases. 

## 9. Related Work

### 9.a. Papers

List the key papers that you have identified relating to your project idea, and describe how they related to your project. Provide references (with full citation in the References section below).

### 9.b. Datasets

List datasets that you have identified and plan to use. Provide references (with full citation in the References section below).

### 9.c. Software

List softwate that you have identified and plan to use. Provide references (with full citation in the References section below).

## 10. References

List references correspondign to citations in your text above. For papers please include full citation and URL. For datasets and software include name and URL.
