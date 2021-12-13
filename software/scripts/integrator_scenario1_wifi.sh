#! /bin/bash

#setup ROS
source /opt/ros/melodic/setup.bash

#setup NS3 Path to run the simulation in
NS3_PATH=~/Software/ns-allinone-3.30.1/ns-3.30.1

# parsing important line numbers from the next_position.txt file...
# to be used in the below loop to send next position values to gazebo
input="../next_position.txt"
line_x1=$(grep -n "Start" $input)
line_x1=${line_x1//[!0-9]/}
line_y1=$(grep -n "First Checkpoint" $input)
line_y1=${line_y1//[!0-9]/}
line_x2=$(grep -n "Second Checkpoint" $input)
line_x2=${line_x2//[!0-9]/}
line_y2=$(grep -n "Third Checkpoint" $input)
line_y2=${line_y2//[!0-9]/}
line_x3=$(grep -n "Fourth Checkpoint" $input)
line_x3=${line_x3//[!0-9]/}
last_line=$(grep -n "end" $input)
last_line=${last_line//[!0-9]/}
new_position=$(sed "$(($line_x1+1))q;d" $input)
sed -i "4s/^.*$/    x: $new_position/" test.yaml
new_position=$(sed "$(($line_y1+1))q;d" $input)
sed -i "5s/^.*$/    y: $new_position/" test.yaml

# Start loop with line numeber 2 as the first line just contains the text "Start"
line_number=2

# store the timestamp and append it to file names to make it distinct and prevent
# overwriting the same file
timestamp=`date +%s`
printf "GazeboRead,NS3Link1,ReadFile,NS3Link2,GazeboPublish,E2E_Latency,StepRuntime\n" >> ../../data/logs/wifi/scenario1/log_scenario1_$timestamp.csv

# Loop to write each new value on ROS terminal from the next_position.txt file
while [ $line_number -lt $last_line ]
do

  #save timestamp for step runtime
  start_step=`date +%s.%3N`

  #save timestamp for Gazebo Read Latency
  gazebo_read_start=`date +%s.%3N`

  STRING=$(rostopic echo -n1 /scan)
  
  # Get the length of sensor data
  len=`expr length "$STRING"`
  gazebo_read_end=`date +%s.%3N`
  gazebo_read_runtime=$(bc -l <<<"${gazebo_read_end}-${gazebo_read_start}")
  gazebo_read_runtime=$(bc -l <<<"${gazebo_read_runtime}*1000")

  echo "Gazebo Read Latency in ms is $gazebo_read_runtime"

  # save timestamp for NS3 app latency
  ns3_app_start=`date +%s.%3N`

  # Need to send ~50 packets for 100 kB of data for WiFi
  packets=50

  #simulate one ns3 link
  save_pwd=$(pwd);
  cd $NS3_PATH
  export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func|prefix_time:UdpEchoServerApplication=level_all|prefix_func|prefix_time'
  $NS3_PATH/waf --run "scratch/WiFi_simulation --nCsma=1 --nWifi=1 --dataLen=2048 --packets=$packets" > log.out 2>&1
  
  # Run the simulation and parse and store the output
  s=$(grep -A1 "ScheduleTransmit" ./log.out | awk '{print $1;}')
  s=${s//[!0-9,.,+]/}

  i=2
  ns3_runtime1=0
  while [ $i -le $(($packets*2+1)) ]
  do
      start=$(cut -d + -f $i <<< $s)
      end=$(cut -d + -f $((i+1)) <<< $s)
      runtime=$(bc -l <<<"${end}-${start}")
      ns3_runtime1=$(bc -l <<<"${ns3_runtime1}+${runtime}")
      
      i=$(( $i + 2 ))
  done
  ns3_runtime1=$(bc -l <<<"${ns3_runtime1}*1000")
  # Get the first link latency in ms
  echo "Link Latency1 in ms is: $ns3_runtime1"

  $NS3_PATH/waf --run "scratch/WiFi_simulation --nCsma=1 --nWifi=1 --dataLen=1024 --packets=1" > log.out 2>&1
  s=$(grep -A1 "server sent" ./log.out | awk '{print $1;}')
  s=${s//[!0-9,.,+]/}

  ns3_start2=$(cut -d + -f 2 <<< $s)
  ns3_start2=${ns3_start2//[!0-9,.]/}
  
  ns3_stop2=$(cut -d + -f 3 <<< $s)
  ns3_stop2=${ns3_stop2//[!0-9,.]/}
  
  ns3_runtime2=$(bc -l <<<"${ns3_stop2}-${ns3_start2}")
  
  ns3_runtime2=$(bc -l <<<"${ns3_runtime2}*1000")
  # Get the second link latency in ms
  echo "Link Latency2 in ms is: $ns3_runtime2"

  #Go back to the saved directory
  cd $save_pwd

  ns3_app_stop=`date +%s.%3N`

  #calculate the NS3 app latency
  ns3_app_runtime=$(bc -l <<<"${ns3_app_stop}-${ns3_app_start}")
  ns3_app_runtime=$(bc -l <<<"${ns3_app_runtime}*1000")
  echo "NS3 app Latency in ms is $ns3_app_runtime"

  #Get the building controller computation latency
  comp_start=`date +%s.%3N`
  new_position=$(sed "${line_number}q;d" $input)
  comp_end=`date +%s.%3N`
  comp_time=$(bc -l <<<"${comp_end}-${comp_start}")
  echo "Comp Latency in ms is $comp_time"

  #save timestamp for gazebo publish
  gazebo_pub_start=`date +%s.%3N`

  if [[ $line_number -eq $line_y1 || $line_number -eq $line_x2 ||  $line_number -eq $liney2 || $line_number -eq $line_x3 || $line_number -eq $last_line ]]
  then
    line_number=$(( $line_number + 1 ))

  elif [[ $line_number -lt $line_y1 ]]
  then
    echo "$line_number < $line_y1"
    sed -i "4s/^.*$/    x: $new_position/" test.yaml
    rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  elif [[ $line_number -gt $line_y1 && $line_number -lt $line_x2 ]]
  then
    echo "$line_y1 > $line_number  > $line_x2"
    sed -i "5s/^.*$/    y: $new_position/" test.yaml
    rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  elif [[ $line_number -gt $line_x2 && $line_number -lt $line_y2 ]]
  then
    echo "$line_x2 > $line_number  > $line_y2"
    sed -i "4s/^.*$/    x: $new_position/" test.yaml
    rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  elif [[ $line_number -gt $line_y2 && $line_number -lt $line_x3 ]]
  then
    echo "$line_y2 > $line_number  > $line_x3"
    sed -i "5s/^.*$/    y: $new_position/" test.yaml
    rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml


  elif [[ $line_number -gt $line_x3 && $line_number -lt $last_line ]]
  then
    echo "$line_x3 > $line_number  > $last_line"
    sed -i "4s/^.*$/    x: $new_position/" test.yaml
    rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml
  fi

  #Get the Gazebo write latency
  gazebo_pub_end=`date +%s.%3N`
  gazebo_pub_runtime=$(bc -l <<<"${gazebo_pub_end}-${gazebo_pub_start}")
  gazebo_pub_runtime=$(bc -l <<<"${gazebo_pub_runtime}*1000")
  echo "Gazebo Write Latency in ms is: $gazebo_pub_runtime"

  line_number=$(( $line_number + 1 ))

  #Get the End to End latency
  sum_runtime=$(bc -l <<<"${gazebo_read_runtime}+${ns3_runtime1}+${comp_time}+${ns3_runtime2}+${gazebo_pub_runtime}")
  echo "E2E Latency is $sum_runtime"

  #Get the total step runtime
  stop_step=`date +%s.%3N`
  step_runtime=$(bc -l <<<"${stop_step}-${start_step}")
  step_runtime=$(bc -l <<<"${step_runtime}*1000")
  echo "Total step runtime is $step_runtime"

#write each data value to the log file
printf "${gazebo_read_runtime},${ns3_runtime1},${comp_time},${ns3_runtime2},${gazebo_pub_runtime},$sum_runtime,$step_runtime\n" >> ../../data/logs/wifi/scenario1/log_scenario1_$timestamp.csv

done