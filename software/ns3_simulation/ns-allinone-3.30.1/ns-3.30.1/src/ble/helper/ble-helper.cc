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
 *	 This module can be found here:
 *   https://gitlab.com/Stijng/ns3-ble-module
 */
#include "ble-helper.h"
#include <ns3/ble-module.h>
#include <ns3/packet.h>
#include <ns3/mobility-model.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/multi-model-spectrum-channel.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/isotropic-antenna-model.h>
#include <ns3/drop-tail-queue.h>
#include <ns3/log.h>
#include "ns3/names.h"
#include <ns3/random-variable-stream.h>
#include <ns3/onoff-application.h>
#include "ns3/applications-module.h"
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/spectrum-helper.h>
#include "ns3/ipv4-global-routing-helper.h"
#include<ctime>
namespace ns3 {


NS_LOG_COMPONENT_DEFINE ("BleHelper");

/**
 * @brief Output an ascii line representing the Transmit event (with context)
 * @param stream the output stream
 * @param context the context
 * @param p the packet
 */
static void
AsciiBleMacTransmitSinkWithContext (
  Ptr<OutputStreamWrapper> stream,
  std::string context,
  Ptr<const Packet> p)
{
  *stream->GetStream () << "t " << Simulator::Now ().GetSeconds () 
    << " " << context << " " << *p << std::endl;
}

/**
 * @brief Output an ascii line representing the Transmit event (without context)
 * @param stream the output stream
 * @param p the packet
 */
static void
AsciiBleMacTransmitSinkWithoutContext (
  Ptr<OutputStreamWrapper> stream,
  Ptr<const Packet> p)
{
  *stream->GetStream () << "t " << Simulator::Now ().GetSeconds () 
    << " " << *p << std::endl;
}

BleHelper::BleHelper (void)
{
  m_channel = CreateObject<MultiModelSpectrumChannel> ();

  Ptr<LogDistancePropagationLossModel> lossModel = 
    CreateObject<LogDistancePropagationLossModel> ();
  m_channel->AddPropagationLossModel (lossModel);

  Ptr<ConstantSpeedPropagationDelayModel> delayModel = 
    CreateObject<ConstantSpeedPropagationDelayModel> ();
  m_channel->SetPropagationDelayModel (delayModel);
	m_spectrumModel = 0;
  ConstructAllChannels();
}

BleHelper::~BleHelper (void)
{
  m_channel->Dispose ();
  m_channel = 0;
	m_spectrumModel = 0;
}

void
BleHelper::EnableLogComponents (void)
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_LEVEL);
  LogComponentEnableAll (LOG_PREFIX_NODE);
  bool enableAll = true;
  bool enableWarn = false;
  if (enableAll)
  {
    LogComponentEnable ("BleErrorModel", LOG_LEVEL_ALL);
    LogComponentEnable ("BleNetDevice", LOG_LEVEL_ALL);
    LogComponentEnable ("BleLinkController", LOG_LEVEL_ALL);
    LogComponentEnable ("BleBBManager", LOG_LEVEL_ALL);
    LogComponentEnable ("BleLinkManager", LOG_LEVEL_ALL);
    LogComponentEnable ("BleLink", LOG_LEVEL_ALL);
    LogComponentEnable ("BlePhy", LOG_LEVEL_ALL);
    LogComponentEnable ("BleApplication", LOG_LEVEL_ALL);
    LogComponentEnable ("BleHelper", LOG_LEVEL_ALL);
   // LogComponentEnable ("BleMacHeader", LOG_LEVEL_ALL);
   // LogComponentEnable ("BleSpectrumSignalParameters", LOG_LEVEL_ALL);
  }
  else if (enableWarn)
  {
    LogComponentEnable ("BleErrorModel", LOG_LEVEL_WARN);
    LogComponentEnable ("BleNetDevice", LOG_LEVEL_WARN);
    LogComponentEnable ("BleNetDevice", LOG_DEBUG);
    LogComponentEnable ("BleLinkController", LOG_LEVEL_WARN);
    LogComponentEnable ("BleBBManager", LOG_LEVEL_WARN);
    LogComponentEnable ("BleLinkManager", LOG_LEVEL_WARN);
   // LogComponentEnable ("BleLinkManager", LOG_FUNCTION);
    LogComponentEnable ("BleLink", LOG_LEVEL_WARN);
    LogComponentEnable ("BlePhy", LOG_LEVEL_WARN);
    LogComponentEnable ("BleApplication", LOG_LEVEL_WARN);
    LogComponentEnable ("BleHelper", LOG_LEVEL_WARN);
  }
  else
  {
    LogComponentEnable ("BleErrorModel", LOG_LEVEL_INFO);
    LogComponentEnable ("BleNetDevice", LOG_LEVEL_INFO);
    LogComponentEnable ("BleNetDevice", LOG_LOGIC);
    LogComponentEnable ("BleLinkController", LOG_LEVEL_INFO);
    LogComponentEnable ("BleBBManager", LOG_LEVEL_INFO);
    LogComponentEnable ("BleLinkManager", LOG_LEVEL_INFO);
   // LogComponentEnable ("BleLinkManager", LOG_FUNCTION);
    LogComponentEnable ("BleLinkManager", LOG_LOGIC);
    LogComponentEnable ("BleLink", LOG_LEVEL_INFO);
    LogComponentEnable ("BlePhy", LOG_LEVEL_INFO);
    LogComponentEnable ("BleApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("BleHelper", LOG_LEVEL_INFO);
   // LogComponentEnable ("BleMacHeader", LOG_LEVEL_ALL);
   // LogComponentEnable ("BleSpectrumSignalParameters", LOG_LEVEL_ALL);
  }
}

void
BleHelper::AddMobility (Ptr<BlePhy> phy, Ptr<MobilityModel> m)
{
  phy->SetMobility (m);
}

void
BleHelper::ConstructAllChannels()
{
    SpectrumChannelHelper channelHelper;
    channelHelper.SetChannel ("ns3::MultiModelSpectrumChannel");
    bool nakagami = false;
    if (nakagami)
    {
    	channelHelper.AddPropagationLoss ("ns3::OkumuraHataPropagationLossModel",
            "Frequency",DoubleValue(2400e6));
     	channelHelper.AddPropagationLoss ("ns3::NakagamiPropagationLossModel",
            "m0",DoubleValue(1),"m1",DoubleValue(1),"m2",DoubleValue(1));
    }
    else
    {
      channelHelper.AddPropagationLoss ("ns3::OkumuraHataPropagationLossModel",
          "Frequency",DoubleValue(2400e6));
    }
    channelHelper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
    
  for (int i=0; i<40; i++)
  {
    Ptr<SpectrumChannel> c = channelHelper.Create ();
    m_allChannels.push_back(c);
  }
}

NetDeviceContainer
BleHelper::Install (NodeContainer c, bool only_transmit)
{
  NetDeviceContainer devices;
	Mac16Address::Allocate();
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
		Ptr<Node> nodeI = *i;
		Ptr<BleNetDevice> anandi = CreateObject<BleNetDevice> ();
		devices.Add(anandi);
		Ptr<BlePrivacyManager> blp = CreateObject<BlePrivacyManager> ();
		anandi->SetPrivacyManager (blp);
		blp->SetNetDevice (anandi);
		if (only_transmit)
			anandi->SetOnlyTransmit( true ); // only in case of broadcast traffic for sniffer
		Ptr<BlePhy> sfp = Create<BlePhy> ();
        Ptr<BleLinkController> blc = CreateObject<BleLinkController> ();
		if (m_spectrumModel == 0)
			m_spectrumModel = sfp->GetRxSpectrumModel();
		else
			sfp->SetRxSpectrumModel (m_spectrumModel);
		anandi->SetPhy (sfp);
        anandi->SetLinkController (blc);
		anandi->SetAddress(Mac48Address::Allocate());
        blc->SetNetDevice (anandi);
        blc->SetAllChannels (m_allChannels);
		sfp->SetDevice(anandi);
		sfp->SetMobility (nodeI->GetObject<MobilityModel> ());
		sfp->SetChannel (m_channel);
		sfp->SetRxAntenna (Create<IsotropicAntennaModel> ());
		nodeI->AddDevice(anandi);
		anandi->SetGenericPhyTxStartCallback (MakeCallback(&BlePhy::StartTx,sfp));
		sfp->SetTransmissionEndCallback( 
            MakeCallback(&BleNetDevice::NotifyTransmissionEnd,anandi));
		sfp->SetReceptionEndCallback ( 
            MakeCallback(&BleLinkController::CheckReceivedAckPacket,blc));
		sfp->SetReceptionStartCallback ( 
            MakeCallback(&BleNetDevice::NotifyReceptionStart,anandi));
        blc->SetCheckedAckCallback (
            MakeCallback(&BleNetDevice::NotifyReceptionEndOk, anandi));
        blc->SetCheckedAckErrorCallback (
            MakeCallback(&BleNetDevice::NotifyReceptionEndError, anandi));
    for (std::list<callbacktuple>::iterator it = 
        m_callbacks.begin(); it!= m_callbacks.end();it++)
    	{
  			anandi->TraceConnectWithoutContext(std::get<0>(*it),std::get<1>(*it));
    	}
    }
  return devices;
}

void 
BleHelper::AddCallbacks (std::string traceSource, CallbackBase callback)
{
	m_callbacks.push_back(std::make_tuple(traceSource,callback));
}

Ptr<SpectrumChannel>
BleHelper::GetChannel (void)
{
  return m_channel;
}

void
BleHelper::SetChannel (Ptr<SpectrumChannel> channel)
{
  m_channel = channel;
}

void
BleHelper::SetChannel (std::string channelName)
{
  Ptr<SpectrumChannel> channel = Names::Find<SpectrumChannel> (channelName);
  m_channel = channel;
}


int64_t
BleHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<BleNetDevice> Ble = DynamicCast<BleNetDevice> (netDevice);
      if (Ble)
        {
          //currentStream += Ble->AssignStreams (currentStream);
        }
    }
  return (currentStream - stream);
}

/**
 * @brief Write a packet in a PCAP file
 * @param file the output file
 * @param packet the packet
 */
static void
PcapSniffBle (Ptr<PcapFileWrapper> file, Ptr<const Packet> packet)
{
  file->Write (Simulator::Now (), packet);
  NS_LOG_UNCOND ("Packet Sniffed");
}

void
BleHelper::EnablePcapInternal (std::string prefix, 
    Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //NS_LOG_UNCOND (this << "\t" << prefix << "\t" << nd << "\t" << promiscuous << "\t" << explicitFilename);
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.
  //

  // In the future, if we create different NetDevice types, we will
  // have to switch on each type below and insert into the right
  // NetDevice type
  //
  Ptr<BleNetDevice> device = nd->GetObject<BleNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("BleHelper::EnablePcapInternal(): Device " 
          << device << " not of type ns3::BleNetDevice");
      return;
    }

  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out,
                                                  PcapHelper::DLT_IEEE802_15_4);

  if (promiscuous == true)
    {
      device->TraceConnectWithoutContext ("PromiscSniffer", 
          MakeBoundCallback (&PcapSniffBle, file));

    }
  else
    {
      device->TraceConnectWithoutContext ("Sniffer", 
          MakeBoundCallback (&PcapSniffBle, file));
    }
}

void
BleHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream,
  std::string prefix,
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("Enabel ascii internal");
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  Ptr<BleNetDevice> device = nd->GetObject<BleNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("BleHelper::EnableAsciiInternal(): Device " 
          << device << " not of type ns3::BleNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = 
        asciiTraceHelper.CreateFileStream (filename);

      // Ascii traces typically have "+", '-", "d", "r", and sometimes "t"
      // The Mac and Phy objects have the trace sources for these
      //

      asciiTraceHelper.HookDefaultReceiveSinkWithoutContext<BleNetDevice> (
          device, "MacRx", theStream);

      device->TraceConnectWithoutContext ("MacTx", 
          MakeBoundCallback (&AsciiBleMacTransmitSinkWithoutContext, theStream));

      asciiTraceHelper.HookDefaultEnqueueSinkWithoutContext<BleNetDevice> (
          device, "MacTxEnqueue", theStream);
      asciiTraceHelper.HookDefaultDequeueSinkWithoutContext<BleNetDevice> (
          device, "MacTxDequeue", theStream);
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<BleNetDevice> (
          device, "MacTxDrop", theStream);

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to provide a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static
  // functions that are always there waiting for just such a case.
  //


  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid 
    << "/$ns3::BleNetDevice/MacRx";
  device->TraceConnect ("MacRx", oss.str (), MakeBoundCallback (
        &AsciiTraceHelper::DefaultReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" 
    << deviceid << "/$ns3::BleNetDevice/Mac/MacTx";
  device->TraceConnect ("MacTx", oss.str (), 
      MakeBoundCallback (&AsciiBleMacTransmitSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" 
    << deviceid << "/$ns3::BleNetDevice/Mac/MacTxEnqueue";
  device->TraceConnect ("MacTxEnqueue", oss.str (), 
      MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, 
        stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" 
    << deviceid << "/$ns3::BleNetDevice/Mac/MacTxDequeue";
  device->TraceConnect ("MacTxDequeue", oss.str (), 
      MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, 
        stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" 
    << deviceid << "/$ns3::BleNetDevice/Mac/MacTxDrop";
  device->TraceConnect ("MacTxDrop", oss.str (), 
      MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));
}

ApplicationContainer 
BleHelper::GenerateBroadcastTraffic(Ptr<RandomVariableStream> var, 
    NodeContainer nodes, int packet_size, double start, 
    double duration, double interval, int mobility_profile, std::vector<int> dev_class)
{
  ApplicationContainer apps;
  double offset = 0;
  int countdev=0;

  int flag=0;//to check if the inter public packet interval is greater than 10ms as specified by the standard

  Ptr<NormalRandomVariable> x = CreateObject<NormalRandomVariable> ();

  // interval in seconds, timeoffset in microsends (1000*1000)

  if ( mobility_profile==0 || mobility_profile==2 || mobility_profile==3 )
  {
  		for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); i++)
    {
   		flag=0;

        RngSeedManager::SetSeed (time(0));  // Changes seed from default of 1
		RngSeedManager::SetRun (time(0));   // Changes run number from default of 1

		while (flag==0)
		{
			if (dev_class[countdev]==0) //this is a parameter related to characterstic time (time between two beacons)
	        	interval = x->GetValue(0.05, 0.01);
	        else if (dev_class[countdev]==1)
	        	interval = x->GetValue(0.30, 0.01);
	        else if (dev_class[countdev]==2)
	        	interval = x->GetValue(0.50, 0.05);
	        else if (dev_class[countdev]==3)
	        	interval = x->GetValue(2.0, 0.5);

	        if (interval>0.01)
	        	flag=1;
		}

        apps.Add(GenerateBroadcastTraffic (
              var, *i, packet_size, start, duration, interval, offset, mobility_profile));
        //offset = offset+offsetIncr;

        countdev++;
	}
  }

  else if ( mobility_profile==1 )
  {
  	//for mobility scenario 1(static but free to appear and stay for an arbitary time)
	Ptr<UniformRandomVariable> urv = CreateObject<UniformRandomVariable> ();
	for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); i++)
    {
    	flag=0;

    	double this_start = urv->GetValue(0, duration);
    	double this_duration = urv->GetValue(0, duration - this_start);
    	//offset = 1000*1000*this_start;
    	offset=0;

        RngSeedManager::SetSeed (time(0));  // Changes seed from default of 1
		RngSeedManager::SetRun (time(0));   // Changes run number from default of 1

       	
       	while (flag==0)
		{
			if (dev_class[countdev]==0) //this is a parameter related to characterstic time (time between two beacons)
	        	interval = x->GetValue(0.05, 0.01);
	        else if (dev_class[countdev]==1)
	        	interval = x->GetValue(0.30, 0.01);
	        else if (dev_class[countdev]==2)
	        	interval = x->GetValue(0.50, 0.05);
	        else if (dev_class[countdev]==3)
	        	interval = x->GetValue(2.0, 0.5);

	        if (interval>0.01)
	        	flag=1;
		}

        apps.Add(GenerateBroadcastTraffic (
              var, *i, packet_size, this_start, this_duration, interval, offset, mobility_profile));

        countdev++;
	}
  }

  // Last node sends to first one
  apps.Start(Seconds(start));
  apps.Stop (Seconds(start+duration));
  return apps;
}

ApplicationContainer 
BleHelper::GenerateTraffic(Ptr<RandomVariableStream> var, NodeContainer nodes, 
    int packet_size, double start, double duration, double interval)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End ()-1; i++)
    {
        Ptr<Node> destination = *(i+1);
        apps.Add(GenerateTraffic (var, *i, packet_size, start, 
              duration, interval, destination)); 
	}
  // Last node sends to first one
  bool fullLoop = false;
  if (fullLoop)
  {
    NodeContainer::Iterator i = nodes.Begin();
    NodeContainer::Iterator j = nodes.End();
    Ptr<Node> destination = *i;
    Ptr<Node> source = *(j-1);
    apps.Add(GenerateTraffic (var, source, packet_size, start, 
          duration, interval, destination)); 
  }
  apps.Start(Seconds(start));
  apps.Stop (Seconds(start+duration));
  return apps;
}

Ptr<Application>
BleHelper::GenerateTraffic(Ptr<RandomVariableStream> var, Ptr<Node> node, 
    int packet_size, double start, double duration, 
    double interval, Ptr<Node> destination)
{
  Ptr<BleApplication> app = CreateObject<BleApplication>();

  app->SetAttribute ("InterPacketTime",TimeValue( Seconds (interval)));
  app->SetAttribute ("TimeOffset",TimeValue( MicroSeconds (0)));
  app->SetAttribute ("DataSize",UintegerValue (packet_size));
  app->SetAttribute ("StartTime", TimeValue( Seconds (start + var-> GetValue ())));
  app->SetAttribute ("StopTime", TimeValue( Seconds (start + duration ))); 
  Ptr<BleNetDevice> dest = DynamicCast<BleNetDevice>(destination->GetDevice(0)); 

  app->SetAttribute ("Destination", Mac48AddressValue( dest->GetAddress48()));
  node->AddApplication (app);
  return app;
}

Ptr<Application>
BleHelper::GenerateBroadcastTraffic(Ptr<RandomVariableStream> var, Ptr<Node> node, int packet_size, double start, double duration, double interval, double offset, int mobility_profile)
{
  Ptr<BleApplication> app = CreateObject<BleApplication>();

  app->SetAttribute ("InterPacketTime",TimeValue( Seconds (interval)));
  //NS_LOG_WARN ("offset = " << offset);
  app->SetAttribute ("TimeOffset",TimeValue( MicroSeconds (offset)));
  app->SetAttribute ("DataSize",UintegerValue (packet_size));
  //app->SetAttribute ("StartTime", TimeValue( Seconds (start + var-> GetValue ())));

  if ( mobility_profile==0 || mobility_profile==2 || mobility_profile==3 )
  {
  	app->SetAttribute ("StartTime", TimeValue( Seconds (start + var-> GetValue ())));
  	app->SetAttribute ("StopTime", TimeValue( Seconds ( start + duration )));//start + duration
  }

  else if ( mobility_profile==1 )
  {
  		//for mobility scenario 1(static but free to appear and stay for an arbitary time)
  		app->SetAttribute ("StartApp", TimeValue( Seconds (start )));
		app->SetAttribute ("StopApp", TimeValue( Seconds (duration )));//start + duration
  }

  app->SetAttribute ("Destination", Mac48AddressValue(Mac48Address("FF:FF:FF:FF:FF:FF")));
  //NS_LOG_UNCOND("InterPacketTime: " << interval << " TimeOffset: " << offset << " DataSize: " << packet_size
				//<< " StartTime: " << start + var-> GetValue () << " StopTime: " << start + duration);
  node->AddApplication (app);
  return app;
}


void
BleHelper::InstallNetworkApplication (std::string type, 
                                std::string n0, const AttributeValue &v0,
                                std::string n1, const AttributeValue &v1,
                                std::string n2, const AttributeValue &v2,
                                std::string n3, const AttributeValue &v3,
                                std::string n4, const AttributeValue &v4,
                                std::string n5, const AttributeValue &v5,
                                std::string n6, const AttributeValue &v6,
                                std::string n7, const AttributeValue &v7)
{
  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  m_netApp.push_back(factory);
}

void
BleHelper::CreateBroadcastLink (NetDeviceContainer c, 
    bool scheduled, uint32_t nbConnInterval, bool collAvoid)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (c.GetN() > 1);
  uint32_t nbOffset = c.GetN()*(c.GetN()+1)/2; // offset == number of nodes

  std::list<Ptr<BleBBManager>> otherBBManagers;
  Ptr<BleNetDevice> netDevice1 = DynamicCast<BleNetDevice> (*c.Begin());
  Ptr<BleBBManager> instantiatingBBM = netDevice1->GetBBManager(); 

  for (NetDeviceContainer::Iterator i = c.Begin ()+1; i != c.End (); ++i)
  {
    Ptr<NetDevice> netDevice = (*i);
    Ptr<BleNetDevice> BleND2 = DynamicCast<BleNetDevice> (netDevice);
    otherBBManagers.push_back(BleND2->GetBBManager());
  }
  Ptr<BleLink> link = instantiatingBBM->CreateLinkScheduledMultipleNodes(
          otherBBManagers, 
          scheduled, nbOffset, nbConnInterval, collAvoid);
}

void
BleHelper::CreateAllLinks (NetDeviceContainer c, 
    bool scheduled, uint32_t nbConnInterval)
{
  NS_LOG_FUNCTION (this);
  uint32_t nbOffset = 0;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<NetDevice> netDevice = (*i);
      Ptr<BleNetDevice> BleND1 = DynamicCast<BleNetDevice> (netDevice);
      for (NetDeviceContainer::Iterator j = i+1; j != c.End(); ++j)
      {
        Ptr<NetDevice> netDevice2 = (*j);
        Ptr<BleNetDevice> BleND2 = DynamicCast<BleNetDevice> (netDevice2);
        Ptr<BleLink> link2 = BleND1->GetBBManager()->CreateLinkScheduled(
          BleND2->GetBBManager(), 
          BleLinkManager::Role::MASTER_ROLE, 
          scheduled, nbOffset, nbConnInterval);
        nbOffset++;
      }
 
    }
}
	
} // namespace ns3

