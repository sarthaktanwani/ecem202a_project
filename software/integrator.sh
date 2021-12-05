#! /bin/bash
#add thing to get overall delay
ultimateStartTime=`date +%s.%3N`
source /opt/ros/melodic/setup.bash
start=`date +%s.%3N`
#sleep 1
STRING=$(rostopic echo /gazebo/model_states -n1 | grep -A3 -m2 "position" | tail -n3)
#STRING="x=45,y=45,z=0"
len=`expr length "$STRING"`
end=`date +%s.%3N`
runtime=$(bc -l <<<"${end}-${start}")
echo "The length of $STRING is $len"
echo "Start time is $start"
echo "End time is $end"
echo "Status Latency is $runtime"

NS3_PATH=~/Software/ns-allinone-3.30.1/ns-3.30.1

cd $NS3_PATH
export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func|prefix_time:UdpEchoServerApplication=level_all|prefix_func|prefix_time'
$NS3_PATH/waf --run "scratch/WiFi_simulation --nCsma=1 --nWifi=2 --dataLen=2044" > log.out 2>&1
s=$(grep -A1 "At time 4s" ./log.out | awk '{print $1;}')
#s=${s//[!0-9,.]/}
echo "start output"

echo "The two times are: $s"
startTime=$(echo $s | awk '{print $1;}')
startTime=${startTime//[!0-9,.]/}
echo "Start time is: $startTime"
stopTime=$(echo $s|awk '{print $2;}')
stopTime=${stopTime//[!0-9,.]/}
echo "Stop time is: $stopTime"
totalTime=$(bc -l <<<"${stopTime}-${startTime}")
echo "Total time is: $totalTime"
totalTime=$(bc -l <<<"${totalTime}*1000")
echo "Total time in ms is: $totalTime"
cd 

echo $pwd
#input="/home/pragyasharma/ECE202/ecem202a_project/software/demo.txt"

#while IFS= read -r line
#do
#  echo "$line"
#done < "$input"

sed -n '5p' < "/home/pragyasharma/ECE202/ecem202a_project/software/demo.txt"

cmd=$("pwd")
echo $cmd

#rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState \"model_name: 'diff_drive' '{linear'

template1=$("rostopic pub -1 /gazebo/set_model_state gazebo_msgs/ModelState \"model_name: 'diff_drive'\n")
pose=$("pose:\n")
position=$("  position:\n")
px1=$("    x: 10\n")
py1=$("    y: 10\n")
pz1=$("    z: 10.0\n")
orientation=$("  orientation:\n")
px2=$("    x: 0.0\n")
py2=$("    y: 0.0\n")
pz2=$("    z: 0.0\n")
pw1=$("    w: 0.0\n")
twist=$("twist:\n")
linear=$("  linear:\n")
px3=$("    x: 0.0\n")
py3=$("    y: 0.0\n")
pz3=$("    z: 0.0\n")
angular=$("  angular:\n")
px4=$("    x: 0.0\n")
py4=$("    y: 0.0\n")
pz4=$("    z: 0.0\n")
pref=$("reference_frame: ''\"\n")
pdq=$("\n)
echo $template
echo $pose
echo $position
echo $px1
echo $py1
echo $pz1
echo $orientation
echo $px2
echo $py2
echo $pz2
echo $pw1
echo $twist
echo $linear
echo $px3
echo $py3
echo $pz3
echo $angular
echo $px4
echo $py4
echo $pz4
echo $pref
echo $pdq

#to take arguments in a bash file
echo " I firmly believe that $1 is the best $2 for the office of $3"


echo $(whoami)

echo "What is your name?"
read name
if [ $name ]; then
	echo "$name sounds alright to me"
else
	echo "GIVE ME YOUR NAMEE!!!"
fi
