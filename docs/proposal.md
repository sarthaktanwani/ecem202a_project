# Project Proposal

## 1. Motivation & Objective
 
What are you trying to do and why? (plain English without jargon)
We are trying to study the distribution of control in a non-adversarial networked control system with respect to network dynamics. In other words, we are studying the different cases of distribution of control between robots wirelessly and how does the network behave and its parameters in these cases.

## 2. State of the Art & Its Limitations
 
How is it done today, and what are the limits of current practice?
1. Absence of simulators that can simulate both a physics based robotics environment as well as wireless network between robots that can properly simulate real-world scenarios without taking ideal assumptions
2. Most of the literature on distribution of control between robots is between an autonomous robot and a human. However, we are studying the distribution of control between a mobile autonomous robot and a static autonomous robot

## 3. Novelty & Rationale
  
What is new in your approach and why do you think it will be successful?
It will be successful because it allows for a more intimate cotrol of the mobile autonomous robot without compromising on the security and privacy of the building infrastructure(in our case). Our approach clearly marks the distinction between how much information the autonomous mobile robot needs and gets in order to navigate inside a building. We have created 3 different cases with varying levels of control distribution between the mobile robot and the building and we will study the network parameters in each case. The network parameters would play an important role in outlining what level of control a robot needs in each case vs the load on the network. This could, in the future, be used to make design choices like what network technology to use, what is the average power consumption... etc

## 4. Potential Impact

If the project is successful
If the project is successful, what difference will it make, both technically and broadly?

## 5. Challenges

What are the challenges and risks?
1. The main challenge is the lack of proper software support that can simulate both a physics based robotics environment and the wireless network between robots simultaneously.

## 6. Requirements for Success

What skills and resources are necessary to perform the project?
1. Understanding of different wireless technologies like WiFi, BLE, Zigbee
2. Software that can simulate both physical robots and their wireless communication over network
3. C/C++/Python programming
4. Basics of simulation software

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
