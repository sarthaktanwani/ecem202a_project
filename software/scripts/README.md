This folder contains the project scripts. Description of each script is as follows:

* generate_next_position.py - Python script that acts as the building's computational entity. It is responsible for generating the next position of the robot. 
* integrator_parent.sh - Bash script that runs Scenario 1 and Scenario 2 simulations.
* integrator_scenario1.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 1.
* integrator_scenario2.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 2.
* post_process_data.py - Python script that processes collected log files and produces a final csv with latency values and breakdown for both scenarios.
* test.yaml - ROS publish command template file to be used by the integrators.
