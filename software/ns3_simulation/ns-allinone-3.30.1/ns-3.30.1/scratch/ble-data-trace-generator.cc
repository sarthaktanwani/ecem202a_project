/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 Inria
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Abhishek Mishra <abhishek.mishra@inria.fr>
 *   Adaptation 
 *   from the BLE ns-3 module by Stijn Geysen <stijn.geysen@student.kuleuven.be>
 *   This module can be found here:
 *   https://gitlab.com/Stijng/ns3-ble-module
 */

// Include a header file from your module to test.
//#include <ns3/log.h>
#include <ns3/core-module.h>
#include <ns3/ble-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/packet.h>
#include <ns3/rng-seed-manager.h>
#include <ns3/spectrum-module.h>
#include <ns3/mobility-module.h>
#include <ns3/energy-module.h>
#include <ns3/spectrum-value.h>
#include <ns3/spectrum-analyzer.h>
#include <iostream>
#include <ns3/isotropic-antenna-model.h>
#include <ns3/trace-helper.h>
#include <ns3/drop-tail-queue.h>
#include <unordered_map>
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/ns-tag.h"
#include "ns3/internet-module.h"
#include <ns3/okumura-hata-propagation-loss-model.h>
#include <ns3/random-variable-stream.h>
#include <map>
#include <vector>
#include <sys/stat.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BleData");

  /*****************
   * Configuration *
   *****************/

  int nbIterations = 1;
  double length = 10; //<! Square room with length as distance 30
  int pktsize = 255; //!< Size of packtets, in bytes
  // int duration = 40 + 60 + 10; //<! Duration of the simulation in seconds 110
  int duration = 10;
  int packetSendDuration = 40 + 60; //<! Time during which new packets should be queued 100
  bool verbose = true; // Enable logging false
  bool nakagami = true; // enable nakagami path loss false
  bool dynamic = false; // Wether the nodes are moving yes or no false
  bool scheduled = true; // Schedule the TX windows instead of random parameters.
  bool single_device = true; //single or multiple device classes
  bool give_rand_invl = false; //do we want to specify the randomization interval for the network devices
  int rand_invl = 0; // input MAC randomization interval in seconds
  bool broadcastAvoidCollisions = true; 
          // Try to avoid 2 nodes being in advertising mode at the same time
  uint32_t nNodes = 2; // Number of nodes 25
  uint32_t nbConnInterval = 5;
        // [MAX 3200]  nbConnInterval*1,25ms = size of connection interval. 
        // if nbConnInterval = 0, each link will get a random conn interval 3200
  double interval = 0.02;
  // double interval = 2*nNodes; //!< Time between two packets from the same node 
  //         //(for good results, should be larger than nNodes*nbConnInterval(s) 
  Ptr<OutputStreamWrapper> m_stream = 0; // Stream for waterfallcurve
  Ptr<UniformRandomVariable> randT = CreateObject<UniformRandomVariable> ();

  std::unordered_map<uint32_t,std::tuple<uint32_t,uint32_t,uint32_t,
    uint32_t,uint32_t,uint32_t,uint32_t,uint32_t> > errorMap;
  std::unordered_map<uint32_t,Ptr<BleNetDevice> > deviceMap;

  // char *dev_names[2] = {(char *)"Apple, Inc.", (char *)"Samsung, Inc."};
  char *dev_names[2] = {(char *)"Samsung, Inc.", (char *)"Apple, Inc."};
  std::map<std::string, uint32_t> mac_map;//map from anon mac to node id
  std::vector<int> dev_class;//array contains device class at index: node id

  std::vector< std::pair<std::string, std::string> > IRK_idendev_pairs;//contains pair of IRK(Identity Resolution Key) and Identity address pairs

  /************************
   * End of configuration *
   ************************/

/// Save that the message has been transmitted
	void
Transmitted (Ptr<OutputStreamWrapper> stream, const Ptr<const Packet> packet)
{
	Ptr<Packet> copy = packet->Copy();
	BleMacHeader header;
	copy->RemoveHeader(header);

  *stream->GetStream () << '+' << Simulator::Now ().GetSeconds () << 's' << "\t\t\t" << " Tx  " << "\t" << " src " 
                              << header.GetSrcAddr() << "\t" << "dest " << header.GetDestAddr() << std::endl;
}

// save that a message has been received
void Received (Ptr<OutputStreamWrapper> stream, const Ptr<const Packet> packet)
{
    //*stream->GetStream () << "Packet sniffed  " << packet << " dest addr = " << addr << std::endl;

  //NS_LOG (LOG_INFO, "Packet received  " << packet);
	Ptr<Packet> copy = packet->Copy();
	BleMacHeader header;
	copy->RemoveHeader(header);
	uint8_t buffer[6];
    header.GetDestAddr().CopyTo(buffer);
    uint32_t addr = buffer[5];

    uint8_t buffer_src[6];
    header.GetSrcAddr().CopyTo(buffer_src);
    // uint32_t addr_src = buffer[5];

     //*stream->GetStream () << "Packet sniffed  " << packet << " src addr " << addr_src << " dest addr = " << addr << std::endl;
    std::get<1>(errorMap[addr-1])++;
}

void ReceivedError (Ptr<OutputStreamWrapper> stream, const Ptr<const Packet> packet)
{
  //NS_LOG (LOG_DEBUG, "Packet received  " << packet);
	Ptr<Packet> copy = packet->Copy();
	BleMacHeader header;
	copy->RemoveHeader(header);
	uint8_t buffer[6];
    header.GetDestAddr().CopyTo(buffer);
    uint32_t addr = buffer[5];
    std::get<3>(errorMap[addr-1])++;
}

// save that a message has been uniquely received
void ReceivedUnique (Ptr<OutputStreamWrapper> stream, const Ptr<const Packet> packet,
        const Ptr<const BleNetDevice>  netdevice, const uint32_t node_id)
{
	Ptr<Packet> copy = packet->Copy();
	BleMacHeader header;
	copy->RemoveHeader(header);

    *stream->GetStream () << '+' << Simulator::Now ().GetSeconds() << 's' << "\t\t" << " Rx  " << "\t" << " src " 
                              << header.GetSrcAddr() << "\t" << "dest " << netdevice->GetAddress48() << "\ttx_dev " << dev_names[dev_class[node_id]] << std::endl;
}





void ReceivedBroadcast (Ptr<OutputStreamWrapper> stream, const Ptr<const Packet> packet, 
    const Ptr<const BleNetDevice>  netdevice, const uint32_t node_id)
{
	Ptr<Packet> copy = packet->Copy();
	BleMacHeader header;
	copy->RemoveHeader(header);
	uint8_t buffer[6];
    header.GetDestAddr().CopyTo(buffer);
    //netdevice->GetAddress16().CopyTo(buffer);
    uint32_t addr = buffer[5];

    uint8_t buffer_src[6];
    header.GetSrcAddr().CopyTo(buffer_src);
    // uint32_t addr_src = buffer_src[5];

    *stream->GetStream () <<  Simulator::Now ().GetSeconds () <<  "\t" << header.GetSrcAddr() << "\t" << dev_names[dev_class[node_id]] << std::endl;

    //NS_LOG (LOG_INFO, "Packet unique received  " 
    //<< packet << " dest addr = " << addr);
	std::get<4>(errorMap[addr-1])++;
}






void TXWindowSkipped (Ptr<OutputStreamWrapper> stream, const Ptr<const BleNetDevice> nd)
{
  //NS_LOG (LOG_DEBUG, "Packet received  " << packet);
	uint8_t buffer[6];
    nd->GetAddress48().CopyTo(buffer);
    uint32_t addr = buffer[5];
    std::get<5>(errorMap[addr-1])++;
}

// Record the ground truth of MAC Randomization
void MAC_GT (Ptr<OutputStreamWrapper> stream, const uint32_t id_node, const std::string this_mac)
{
  //mac_map.insert(std::pair<Mac48Address, uint32_t>(Mac48Address(this_mac.c_str()), id_node));
  *stream->GetStream () << this_mac << "\t" << id_node << std::endl;
  //std::cout << (int)id_node << std::endl;
}

int main (int argc, char** argv)
{
  //User input params
  bool pcap = false;
  bool verbose = false;
  int mobility_profile=0; //0 - Static & confined, 1 - Static & free_to_leave, 2 - Mobile & confined, 3 - Mobile & free_to_leave
  int device_class=0;
  int packets;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue ("pcap", "Write PCAP traces.", pcap);
  cmd.AddValue ("mobility_profile", "Mobility Profile", mobility_profile);
  cmd.AddValue ("device_class", "Device Class", device_class);
  cmd.AddValue ("num_devices", "Number of devices", nNodes);
  cmd.AddValue ("rand_invl", "MAC randomization interval", rand_invl);
  cmd.AddValue ("packets", "Number of packets", packets);
  
  cmd.Parse (argc,argv);
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_LEVEL);
  LogComponentEnableAll (LOG_PREFIX_NODE);
 




  // Enable logging
  BleHelper helper;
  if (verbose)
    helper.EnableLogComponents();

  if (mobility_profile==0 || mobility_profile==1)
    dynamic=false;
  else if (mobility_profile==2 || mobility_profile==3)
    dynamic=true;

  if (device_class==0)
    single_device=true;
  else
    single_device=false;

  //NS_LOG_UNCOND(mobility_profile << "\t" << device_class << "\t" << nNodes << "\t" << dynamic << "\t" << single_device);

  Packet::EnablePrinting ();
  Packet::EnableChecking ();

  NS_LOG_INFO ("BLE Data file");

  // Enable debug output
  NS_LOG_INFO ("Enable debug output");
  



  for (uint8_t iterationI=0;iterationI<nbIterations;iterationI++){
		std::cout << "Iteration: " << (int)iterationI << std::endl;
 
    randT->SetAttribute("Max", DoubleValue (600));
   
    NS_LOG (LOG_INFO, "Ble Data setup starts now");

    NodeContainer bleDeviceNodes;
    bleDeviceNodes.Create(nNodes);

    if (mkdir("traces", 0777) == -1)
        // NS_LOG_UNCOND("Error or Directory already exists");
         NS_LOG_INFO("Directory already exists");
    else
        NS_LOG_UNCOND("Directory created");

    AsciiTraceHelper ascii;
    Ptr<OutputStreamWrapper> data_stream = ascii.CreateFileStream ("./traces/ble_data_"+std::to_string(nNodes)+"_"+std::to_string(mobility_profile)+"_"+std::to_string(device_class)+".tr");
    Ptr<OutputStreamWrapper> gt_stream = ascii.CreateFileStream ("./traces/mac_gt_data_"+std::to_string(nNodes)+"_"+std::to_string(mobility_profile)+"_"+std::to_string(device_class)+".tr");




    // Create mobility
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> nodePositionList = 
      CreateObject<ListPositionAllocator> ();
    for (uint32_t nodePositionsAssigned = 0; 
        nodePositionsAssigned < nNodes; nodePositionsAssigned++)
    {
      double x,y;
      x = randT->GetInteger(0,length);
      y = randT->GetInteger(0,length);
      *gt_stream->GetStream () << "NodeId:" << "\t" << nodePositionsAssigned << "\t" << x << "\t" << y << std::endl;
      nodePositionList->Add (Vector (x,y,1.0));
    }
    mobility.SetPositionAllocator (nodePositionList);
    if (dynamic)
      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", "Mode", StringValue ("Time"),
                              "Time", StringValue ("2s"),
                              "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                              "Bounds", StringValue ("0|10|0|10"));
    else
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install(bleDeviceNodes);

    // Create the nodes
    NetDeviceContainer bleNetDevices;
    bleNetDevices = helper.Install (bleDeviceNodes, false);// nodes can receive too!

    // Set addresses
    NS_LOG (LOG_INFO, "Set addresses");
    for (uint32_t nodeI = 0; nodeI < nNodes; nodeI++)
    {
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetStream(gt_stream);
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetMacMap(mac_map);

      if (rand_invl>0)
      {
        DynamicCast<BleNetDevice>(
            bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetRandInterval(rand_invl);
      }
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(nodeI))->GetPrivacyManager()->UpdateAddress(nodeI);


      Ptr<UniformRandomVariable> xval = CreateObject<UniformRandomVariable> ();
      double randval = xval->GetValue(0, 1);
      if (randval < 0.5 || single_device)
      {
        DynamicCast<BleNetDevice>(
            bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetDeviceClass(0);
        dev_class.push_back(0);
      }
      else
      {
        DynamicCast<BleNetDevice>(
          bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetDeviceClass(1);
        dev_class.push_back(1);
      }

      IRK_idendev_pairs.emplace_back( DynamicCast<BleNetDevice> (bleNetDevices.Get(nodeI))->GetPrivacyManager()->GetIRK(),
                        DynamicCast<BleNetDevice>(bleNetDevices.Get(nodeI))->GetIdentityAddress48() );

      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(nodeI))->GetPrivacyManager()->SetBleStandard(0);
      NS_LOG (LOG_INFO, "address = " 
          << DynamicCast<BleNetDevice>(bleNetDevices.Get(nodeI))->GetAddress ());
    }






    //store the IRK-Identity address pairs at the net devices and the sniffers
    for(unsigned int j=0; j< IRK_idendev_pairs.size(); j++)
    {

      //NS_LOG_UNCOND(IRK_idendev_pairs[j].first << "\t" << IRK_idendev_pairs[j].second);

      // for nodes
      for (uint32_t nodeI = 0; nodeI < nNodes; nodeI++)
      {
        DynamicCast<BleNetDevice>(
            bleNetDevices.Get(nodeI))->GetPrivacyManager()->AddIRK(IRK_idendev_pairs[j].first, IRK_idendev_pairs[j].second);
      }

    }





      // Create links between the nodes
      helper.CreateAllLinks (bleNetDevices, scheduled, nbConnInterval);
      // helper.CreateBroadcastLink (bleNetDevices, scheduled, nbConnInterval);
     
      NS_LOG (LOG_INFO, " Generate data ");
      //ApplicationContainer apps = 
      //  helper.GenerateBroadcastTraffic (
      //  randT, bleDeviceNodes, pktsize, 0, packetSendDuration, interval);
      ApplicationContainer apps = 
        helper.GenerateTraffic (
            randT, bleDeviceNodes, pktsize, 0, packetSendDuration, interval);
    



     // Hookup functions to measure performance

    for (uint32_t i=0; i< bleNetDevices.GetN(); i++)
    {
      uint8_t buffer[6];
      Mac48Address::ConvertFrom(
          bleNetDevices.Get(i)->GetAddress()).CopyTo(buffer);
      uint32_t addr = buffer[5];  
      deviceMap[addr ]=DynamicCast<BleNetDevice>(bleNetDevices.Get(i));
      uint32_t x  = bleNetDevices.Get(i)->GetNode()
        ->GetObject<MobilityModel>()->GetPosition ().x;
      uint32_t y  = bleNetDevices.Get(i)
        ->GetNode()->GetObject<MobilityModel>()->GetPosition ().y;
      errorMap[addr-1] = std::make_tuple (0,0,0,0,0,0,x,y);
    }




        // Sniffer trace callbacks

    for (uint32_t i=0; i< bleNetDevices.GetN(); i++)
    {
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "MacTx",MakeBoundCallback(&Transmitted, data_stream));
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "MacRx",MakeBoundCallback(&ReceivedUnique, data_stream));
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "MacRxBroadcast",MakeBoundCallback(&ReceivedBroadcast, data_stream));
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "MacPromiscRx",MakeBoundCallback(&Received, data_stream));
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "MacRxError",MakeBoundCallback(&ReceivedError, data_stream));
      DynamicCast<BleNetDevice>(
          bleNetDevices.Get(i))->TraceConnectWithoutContext (
          "TXWindowSkipped",MakeBoundCallback(&TXWindowSkipped, data_stream));
    }





    NS_LOG (LOG_INFO, "Simulator will run now");
    
    if(packets == 4)
      Simulator::Stop(MilliSeconds (80));
    else if(packets == 4000)
      Simulator::Stop(Seconds (100));
    else
      Simulator::Stop(Seconds (duration));
    
    Simulator::Run ();

    for (uint32_t i=0; i< bleNetDevices.GetN(); i++)
    {
            uint8_t buffer[6];
            Mac48Address::ConvertFrom(
                bleNetDevices.Get(i)->GetAddress()).CopyTo(buffer);
            uint32_t addr = buffer[5];  
            Ptr<BleNetDevice> netdevice =
              DynamicCast<BleNetDevice>(bleNetDevices.Get(i));
            NS_LOG (LOG_DEBUG, "nd = " << netdevice << " addr = " << addr);
			// std::tuple<uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,
      //         uint32_t,uint32_t,uint32_t> tuple = errorMap[i];
		
    }
    errorMap.clear();
    Simulator::Destroy ();


    *data_stream->GetStream () << "Done" << std::endl;
  }
  
  NS_LOG_INFO("Done.");
  return 0;
}


