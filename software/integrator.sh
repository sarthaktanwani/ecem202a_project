#! /bin/bash
STRING="x=45,y=45,z=0"
len=`expr length "$STRING"`
echo "The length of $STRING is $len"

NS3_PATH=/home/sarthak/Documents/for_embedded_systems_project/standalone_ns3/repos/ns-3-allinone/ns-3.30

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
