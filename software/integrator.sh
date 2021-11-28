#! /bin/bash
STRING="x=45,y=45,z=0"
len=`expr length "$STRING"`
echo "The length of $STRING is $len"

NS3_PATH=/home/sarthak/Documents/for_embedded_systems_project/standalone_ns3/repos/ns-3-allinone/ns-3.30

cd $NS3_PATH
export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func|prefix_time:UdpEchoServerApplication=level_all|prefix_func|prefix_time'
echo $($NS3_PATH/waf --run "scratch/WiFi_simulation --nCsma=1 --nWifi=2 --dataLen=2044" | grep "At")
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
