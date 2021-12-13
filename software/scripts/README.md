This folder contains the project scripts. Description of each script is as follows:

* generate_next_position.py - Python script that acts as the building's computational entity. It is responsible for generating the next position of the robot. 
* integrator_parent_wifi.sh - Bash script that runs Scenario 1 and Scenario 2 simulations in WiFi.
* integrator_scenario1_wifi.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 1 in WiFi.
* integrator_scenario2_wifi.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 2 in WiFi.
* integrator_parent_ble.sh - Bash script that runs Scenario 1 and Scenario 2 simulations in BLE.
* integrator_scenario1_ble.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 1 in BLE.
* integrator_scenario2_ble.sh - Bash script that interfaces Gazebo, ROS and NS3 for Scenario 2 in BLE.
* post_process_data.py - Python script that processes collected log files and produces a final csv with latency values and breakdown for both scenarios.
* test.yaml - ROS publish command template file to be used by the integrators.
