#! /bin/bash
#add thing to get overall delay
ultimateStartTime=`date +%s.%3N`
source /opt/ros/melodic/setup.bash

NS3_PATH=~/Software/ns-allinone-3.30.1/ns-3.30.1

save_pwd=$(pwd);
cd $NS3_PATH
export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func|prefix_time:UdpEchoServerApplication=level_all|prefix_func|prefix_time'
cd $save_pwd

echo $pwd

input="next_position.txt"
line_x1=$(grep -n "Start" next_position.txt)
line_x1=${line_x1//[!0-9]/}
line_y1=$(grep -n "First Checkpoint" next_position.txt)
line_y1=${line_y1//[!0-9]/}
line_x2=$(grep -n "Second Checkpoint" next_position.txt)
line_x2=${line_x2//[!0-9]/}
line_y2=$(grep -n "Third Checkpoint" next_position.txt)
line_y2=${line_y2//[!0-9]/}
line_x3=$(grep -n "Fourth Checkpoint" next_position.txt)
line_x3=${line_x3//[!0-9]/}
last_line=$(grep -n "end" next_position.txt)
last_line=${last_line//[!0-9]/}
new_position=$(sed "$(($line_x1+1))q;d" next_position.txt)
sed -i "4s/^.*$/    x: $new_position/" test.yaml
new_position=$(sed "$(($line_y1+1))q;d" next_position.txt)
sed -i "5s/^.*$/    y: $new_position/" test.yaml

echo "linex1 is $line_x1"
echo "liney1 is $line_y1"
echo "linex2 is $line_x2"
echo "liney2 is $line_y2"
echo "linex3 is $line_x3"


# x=1
# while [ $x -lt 5 ]
# do
#   echo "Welcome $x times"
#   x=$(( $x + 1 ))
# done

line_number=2
timestamp=`date +%s`
printf "GazeboRead,NS3Link1,ReadFile,NS3Link2,GazeboPublish,SumLatency,StepRuntime\n" >> logs/log_scenario1_$timestamp.csv
# printf "$host\t$(date)\tTime Taken to checkout\t$Time_checkout\n" >> log.csv
# printf "$host\t$(date)\tTime Taken to add $loopmax 10MB svn files\t$Time_add\n" >> log.csv
# printf "$host\t$(date)\tTime Taken to commit $loopmax 10MB svn files\t$Time_commit\n" >> log.csv
#line_number=33
while [ $line_number -lt $last_line ]
do

  start_step=`date +%s.%3N`

  # read current_pos
  gazebo_read_start=`date +%s.%3N`
  # start_step=gazebo_read_start;
  STRING=$(rostopic echo /gazebo/model_states -n1 | grep -A3 -m2 "position" | tail -n3)
  #STRING="x=45,y=45,z=0"
  len=`expr length "$STRING"`
  gazebo_read_end=`date +%s.%3N`
  gazebo_read_runtime=$(bc -l <<<"${gazebo_read_end}-${gazebo_read_start}")
  gazebo_read_runtime=$(bc -l <<<"${gazebo_read_runtime}*1000")
  # echo "The length of $STRING is $len"
  # echo "Start time is $start"
  # echo "End time is $end"
  echo "Gazebo Read Latency in ms is $gazebo_read_runtime"

  ns3_app_start=`date +%s.%3N`

  #simulate one ns3 link
  save_pwd=$(pwd);
  cd $NS3_PATH
  # export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func|prefix_time:UdpEchoServerApplication=level_all|prefix_func|prefix_time'
  $NS3_PATH/waf --run "scratch/WiFi_simulation --nCsma=1 --nWifi=2 --dataLen=2044" > log.out 2>&1
  s=$(grep -A5 "At time 4s" ./log.out | awk '{print $1;}')
  #s=${s//[!0-9,.]/}
  #echo "s is $s"
  #echo "start output"

  #echo "The six times are: $s"
  ns3_start1=$(echo $s | awk '{print $1;}')
  ns3_start1=${ns3_start1//[!0-9,.]/}
  #echo "Start time is: $ns3_start1"
  ns3_stop1=$(echo $s|awk '{print $2;}')
  ns3_stop1=${ns3_stop1//[!0-9,.]/}
  #echo "Stop time is: $ns3_stop1"
  ns3_runtime1=$(bc -l <<<"${ns3_stop1}-${ns3_start1}")
  #echo "Total time is: $ns3_runtime1"
  ns3_runtime1=$(bc -l <<<"${ns3_runtime1}*1000")
  echo "Link Latency1 in ms is: $ns3_runtime1"

  ns3_start2=$(echo $s | awk '{print $5;}')
  ns3_start2=${ns3_start2//[!0-9,.]/}
  #echo "Start time is: $ns3_start2"
  ns3_stop2=$(echo $s|awk '{print $6;}')
  ns3_stop2=${ns3_stop2//[!0-9,.]/}
  #echo "Stop time is: $ns3_stop2"
  ns3_runtime2=$(bc -l <<<"${ns3_stop2}-${ns3_start2}")
  #echo "Total time is: $ns3_runtime2"
  ns3_runtime2=$(bc -l <<<"${ns3_runtime2}*1000")
  echo "Link Latency2 in ms is: $ns3_runtime2"

  cd $save_pwd

  ns3_app_stop=`date +%s.%3N`

  ns3_app_runtime=$(bc -l <<<"${ns3_app_stop}-${ns3_app_start}")
  ns3_app_runtime=$(bc -l <<<"${ns3_app_runtime}*1000")
  echo "NS3 app Latency in ms is $ns3_app_runtime"

  comp_start=`date +%s.%3N`
  new_position=$(sed "${line_number}q;d" next_position.txt)
  comp_end=`date +%s.%3N`
  comp_time=$(bc -l <<<"${comp_end}-${comp_start}")
  echo "Comp Latency in ms is $comp_time"

  gazebo_pub_start=`date +%s.%3N`

  # if [[ $line_number -eq $line_y1 ]]
  # then
  #   sed -i "8s/^.*$/    x: 1.57/" test.yaml
  #   rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  # elif [[ $line_number -eq $line_x2 ]]
  # then
  #   sed -i "8s/^.*$/    x: 3.1415/" test.yaml
  #   rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  # elif [[ $line_number -eq $line_y2 ]]
  # then
  #   sed -i "8s/^.*$/    x: -1.57/" test.yaml
  #   rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

  # if [[ $line_number -eq $line_x3 ]]
  # then
  #   sed -i "8s/^.*$/    x: 0.0/" test.yaml
  #   rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

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

  gazebo_pub_end=`date +%s.%3N`
  gazebo_pub_runtime=$(bc -l <<<"${gazebo_pub_end}-${gazebo_pub_start}")
  gazebo_pub_runtime=$(bc -l <<<"${gazebo_pub_runtime}*1000")
  echo "Gazebo Write Latency in ms is: $gazebo_pub_runtime"

  line_number=$(( $line_number + 1 ))

  sum_runtime=$(bc -l <<<"${gazebo_read_runtime}+${ns3_runtime1}+${comp_time}+${ns3_runtime2}+${gazebo_pub_runtime}")
  echo "Sum runtime is $sum_runtime"

  stop_step=`date +%s.%3N`
  step_runtime=$(bc -l <<<"${stop_step}-${start_step}")
  step_runtime=$(bc -l <<<"${step_runtime}*1000")
  echo "Total step runtime is $step_runtime"


printf "${gazebo_read_runtime},${ns3_runtime1},${comp_time},${ns3_runtime2},${gazebo_pub_runtime},$sum_runtime,$step_runtime\n" >> logs/log_scenario1_$timestamp.csv

done

# sed -i "4s/^.*$/    x: $x_val/" test.yaml
# sed -i "5s/^.*$/    y: $y_val/" test.yaml
# sed -i "6s/^.*$/    z: $z_val/" test.yaml

# rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState -f test.yaml

# x_val=$(sed -n '4p' < "test.yaml")
# x_val=${x_val//[!0-9,.,-]/}

# y_val=$(sed -n '5p' < "test.yaml")
# y_val=${y_val//[!0-9,.,-]/}

# z_val=$(sed -n '6p' < "test.yaml")
# z_val=${z_val//[!0-9,.,-]/}

#to take arguments in a bash file
# echo " I firmly believe that $1 is the best $2 for the office of $3"


# echo $(whoami)

# echo "What is your name?"
# read name
# if [ $name ]; then
# 	echo "$name sounds alright to me"
# else
# 	echo "GIVE ME YOUR NAMEE!!!"
# fi
