/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 KU Leuven
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
 * Author: Stijn Geysen <stijn.geysen@student.kuleuven.be> 
 *   Adaptation 
 *   from the LoRa ns-3 module lora-net-device,
 *   written by Brecht Reynders.
 *   This module can be found here:
 *   github.com/networkedsystems/lora-ns3/blob/master/model/lora-mac-header.h
 */

//#include "ns3/ble-phy.h"
#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/queue-item.h"
#include "ns3/simulator.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/generic-phy.h"
//#include "ns3/ns-tag.h"
#include "ns3/includes.h"

//include "ble-mac-header.h"
//#include "ble-bb-manager.h"
//#include "ble-link-manager.h"
//#include "ble-link-controller.h"
#include "ble-net-device.h"
#include "ns3/llc-snap-header.h"
#include "ns3/aloha-noack-mac-header.h"
#include <ns3/random-variable-stream.h>

namespace ns3 {

	NS_LOG_COMPONENT_DEFINE ("BleNetDevice");

	NS_OBJECT_ENSURE_REGISTERED (BleNetDevice);

	TypeId
		BleNetDevice::GetTypeId (void)
		{
			static TypeId tid = TypeId ("ns3::BleNetDevice")
				.SetParent<NetDevice> ()
				.AddConstructor<BleNetDevice> ()
				.AddAttribute ("Address",
						"The MAC address of this device.",
						Mac48AddressValue (Mac48Address ("00:00:00:00:00:01")),
						MakeMac48AddressAccessor (&BleNetDevice::m_address),
						MakeMac48AddressChecker ())
				.AddAttribute ("Mtu", "The Maximum Transmission Unit",
						UintegerValue (255), 
						//UintegerValue (30*4), 
						MakeUintegerAccessor (&BleNetDevice::SetMtu,
							&BleNetDevice::GetMtu),
						MakeUintegerChecker<uint16_t> (1,65535))
				.AddAttribute ("Phy", "The PHY layer attached to this device.",
						PointerValue (),
						MakePointerAccessor (&BleNetDevice::GetPhy,
							&BleNetDevice::SetPhy),
						MakePointerChecker<Object> ())
				.AddTraceSource ("MacTx",
						"Trace source indicating a packet has arrived "
						"for transmission by this device",
						MakeTraceSourceAccessor (&BleNetDevice::m_macTxTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("MacTxDrop",
						"Trace source indicating a packet has been dropped "
						"by the device before transmission",
						MakeTraceSourceAccessor (&BleNetDevice::m_macTxDropTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("MacPromiscRx",
						"A packet has been received by this device, has been "
						"passed up from the physical layer "
						"and is being forwarded up the local protocol stack.  "
						"This is a promiscuous trace,",
						MakeTraceSourceAccessor (&BleNetDevice::m_macPromiscRxTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("MacRxBroadcast",
						"A broadcast packet has been received by this device, "
						"has been passed up from the physical layer "
						"and is being forwarded up the local protocol stack.  "
						"This is a non-promiscuous trace,",
						MakeTraceSourceAccessor (&BleNetDevice::m_macRxBroadcastTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("MacRx",
						"A packet has been received by this device, "
						"has been passed up from the physical layer "
						"and is being forwarded up the local protocol stack.  "
						"This is a non-promiscuous trace,",
						MakeTraceSourceAccessor (&BleNetDevice::m_macRxTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("MacRxError",
						"A packet has been received by this device, "
						"has been passed up from the physical layer "
						"and is being forwarded up the local protocol stack, But BER was too high  "
						"This is a non-promiscuous trace,",
						MakeTraceSourceAccessor (&BleNetDevice::m_macRxErrorTrace),
						"ns3::Packet::TracedCallback")
				.AddTraceSource ("TXWindowSkipped",
						"Two Link managers wanted to use the PHY at the same time, "
                        "so one of them needed to skip a TX window, ",
						MakeTraceSourceAccessor (&BleNetDevice::m_macTXWindowSkipped),
						"ns3::BleNetDevice::TracedCallback")
	
				;
			return tid;
		}

	BleNetDevice::BleNetDevice ()
	{
		NS_LOG_FUNCTION (this);
	    m_node = 0;

    Ptr<BleNetDevice> nd_pointer = Ptr<BleNetDevice>(this);

    m_bbManager = CreateObject<BleBBManager> ();
    m_bbManager->SetNetDevice(nd_pointer);

    Ptr<BlePhy> phy = CreateObject<BlePhy> ();
    phy->SetDevice(nd_pointer);
    m_bbManager->SetPhy(phy);
    this->SetPhy (phy);
    this->SetLinkController(CreateObject<BleLinkController> ());
    this->GetLinkController()->SetNetDevice(nd_pointer);

    Ptr<DropTailQueue<QueueItem>> packetQueue = 
      Create<DropTailQueue<QueueItem>> ();
    //packetQueue->SetMaxPackets(QUEUE_SIZE_PACKETS);
    QueueSize qs("100p");
    packetQueue->SetMaxSize(qs);
    m_only_transmit=false;
    this->SetQueue(packetQueue);
    //NS_LOG_UNCOND("device initialized");
    //this->UpdateAddress();

    //NS_LOG_INFO ("BleNetDevice constructor done");
	}

	BleNetDevice::~BleNetDevice ()
	{
		NS_LOG_FUNCTION (this);
	}

	void
		BleNetDevice::DoDispose ()
		{
			NS_LOG_FUNCTION (this);
			m_queue = 0;
			m_node = 0;
			m_phy = 0;
			m_rxCallback = MakeNullCallback <bool, 
                         Ptr<NetDevice>, Ptr<const Packet>, 
                         uint16_t, const Address& > ();
			m_promiscRxCallback = MakeNullCallback <bool, 
                                Ptr<NetDevice>,Ptr<const Packet>, 
                                uint16_t, const Address&, 
                                const Address&, PacketType > ();
			NetDevice::DoDispose ();
		}


  /*
   * Inherited from NetDevice
   */
	void
		BleNetDevice::SetIfIndex (const uint32_t index)
		{
			NS_LOG_FUNCTION (index);
			m_ifIndex = index;
		}

	uint32_t
		BleNetDevice::GetIfIndex (void) const
		{
			NS_LOG_FUNCTION (this);
			return m_ifIndex;
		}
    
//  the maximum transmission unit (MTU) is the size of the largest 
//  protocol data unit (PDU) that can be communicated in a single, 
//  network layer, transaction.
	bool
		BleNetDevice::SetMtu (uint16_t mtu)
		{
			NS_LOG_FUNCTION (mtu);
			m_mtu = mtu;
			return true;
		}

	uint16_t
		BleNetDevice::GetMtu (void) const
		{
			NS_LOG_FUNCTION (this);
			return m_mtu;
		}


	void
		BleNetDevice::SetQueue (Ptr<DropTailQueue<QueueItem>> q)
		{
			NS_LOG_FUNCTION (this);
			NS_LOG_FUNCTION (q);
			m_queue = q;
		}


	void
		BleNetDevice::SetAddress (Address address)
		{
			NS_LOG_FUNCTION (this);
            NS_LOG_LOGIC ("NEW ADDRESS SET: " << address);
			m_address = Mac48Address::ConvertFrom (address);
		}

	Address
		BleNetDevice::GetAddress (void) const
		{
			NS_LOG_FUNCTION (this);
			return m_address;
		}

	Mac48Address
		BleNetDevice::GetAddress48 (void) const
		{
			return m_address;
		}

	std::string BleNetDevice::GetIdentityAddress48 (void)
		{
			return this->GetPrivacyManager()->GetIdentityAddress ();
		}

	bool
		BleNetDevice::IsBroadcast (void) const
		{
			NS_LOG_FUNCTION (this);
            return true;
		}

    // Returns the broadcast address supported by this device.
    // Broadcast address can be public or random
	Address
		BleNetDevice::GetBroadcast (void) const
		{
			NS_LOG_FUNCTION (this);
			return Mac48Address ("FF:FF:FF:FF:FF:FF");
		}

    // Returns tur if this device supports multicast
    //  False for BLE
	bool
		BleNetDevice::IsMulticast (void) const
		{
			NS_LOG_FUNCTION (this);
			//return false;
            // In this case, multicast equals broadcast

            return true;
		}

    // Return the multicast address supported by this device.
    //  No multicast supported, but value needs to be returned,
    //  so all 1's.
	Address
		BleNetDevice::GetMulticast (Ipv4Address addr) const
		{
			NS_LOG_FUNCTION (addr);
			Mac48Address ad = Mac48Address ("FF:FF:FF:FF:FF:FF");
			return ad;
		}


	Address BleNetDevice::GetMulticast (Ipv6Address addr) const
	{
		NS_LOG_FUNCTION (addr);
	    Mac48Address ad = Mac48Address ("FF:FF:FF:FF:FF:FF");
		return ad;
	}


	bool
		BleNetDevice::IsPointToPoint (void) const
		{
			NS_LOG_FUNCTION (this);
			return false;
		}

	bool
		BleNetDevice::IsBridge (void) const
		{
			NS_LOG_FUNCTION (this);
			return false;
		}


	Ptr<Node>
		BleNetDevice::GetNode (void) const
		{
			NS_LOG_FUNCTION (this);
			return m_node;
		}

	void
		BleNetDevice::SetNode (Ptr<Node> node)
		{
			NS_LOG_FUNCTION (node);

			m_node = node;
		}

	void
		BleNetDevice::SetPhy (Ptr<BlePhy> phy)
		{
			NS_LOG_FUNCTION (this << phy);
			m_phy = phy;
		}


	Ptr<BlePhy>
		BleNetDevice::GetPhy () const
		{
			NS_LOG_FUNCTION (this);
			return m_phy;
		}


	void
		BleNetDevice::SetChannel (Ptr<Channel> c)
		{
			NS_LOG_FUNCTION (this << c);
          // TAKE CARE: there is also a channel variabl in the phy layer
            if (this->GetPhy() != 0) {
              this->GetPhy()
                ->SetChannel(DynamicCast<SpectrumChannel> (c));
            }
		}


	Ptr<Channel>
		BleNetDevice::GetChannel (void) const
		{
			NS_LOG_FUNCTION (this);
            return this->GetPhy()->GetChannel();
		}

  // Returns true if ARP is needed, false otherwise.
  //  For your information: ARP = Address Resolution Protocol
	bool
		BleNetDevice::NeedsArp (void) const
		{
			NS_LOG_FUNCTION (this);
			return false;
		}

	bool
		BleNetDevice::IsLinkUp (void) const
		{
			NS_LOG_FUNCTION (this);
			return m_linkUp;
		}

	void
		BleNetDevice::AddLinkChangeCallback (Callback<void> callback)
		{
			NS_LOG_FUNCTION (&callback);
			m_linkChangeCallbacks.ConnectWithoutContext (callback);
		}

	void
		BleNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
		{
			NS_LOG_FUNCTION (&cb);
            NS_LOG_LOGIC ("Receive callback set");
			m_rxCallback = cb;
		}

	void
		BleNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
		{
			NS_LOG_FUNCTION (&cb);
            NS_LOG_LOGIC ("Promisc receive callback set");
			m_promiscRxCallback = cb;
		}

	bool
		BleNetDevice::SupportsSendFrom () const
		{
			NS_LOG_FUNCTION (this);
			return true;
		}

	bool
		BleNetDevice::Send (Ptr<Packet> packet,uint16_t protocolNumber)
		{
			NS_LOG_FUNCTION (packet);
			return this->SendFrom (
                packet, m_address, m_address, protocolNumber);
		}

	bool
		BleNetDevice::Send (Ptr<Packet> packet,const Address& dest)
		{
			NS_LOG_FUNCTION (packet << dest);
			return this->SendFrom (packet, m_address, dest, 0 );
		}

	bool
		BleNetDevice::Send (
            Ptr<Packet> packet,const Address& dest, 
            uint16_t protocolNumber)
		{
			NS_LOG_FUNCTION (packet << dest << protocolNumber);
			return this->SendFrom (
                packet, m_address, dest, protocolNumber);
		}

  bool
	BleNetDevice::SendFrom (
        Ptr<Packet> packet, const Address& src, 
        const Address& dest, uint16_t protocolNumber)
	{
		NS_LOG_FUNCTION (packet << src << dest << protocolNumber);
		
      // Headers will be handled now 
      BleMacHeader header = BleMacHeader();
      if (src != this->GetAddress())
      {
        NS_LOG_WARN ("Warning!!! You are trying to send a message from this"
            " device using a source address that does "
            "not belong to this device!!!");
      }
      header.SetSrcAddr(Mac48Address::ConvertFrom(src)); 
      Mac48Address dest48 = Mac48Address::ConvertFrom(dest);
      NS_LOG_INFO (" Destination address for current packet (nd): " << dest48);
      header.SetDestAddr(dest48);
      header.SetProtocol(protocolNumber);
      packet->AddHeader (header);

      NodeTag ntag;
  	  ntag.SetNodeId (this->GetNode()->GetId());
	  packet->AddPacketTag (ntag);
			
      bool sendOk = true;
			// If the device is idle, transmission starts immediately. Otherwise,
			// the transmission will be started by NotifyTransmissionEnd
      NS_LOG_LOGIC ("Enqueueing new packet of length " << packet->GetSize());
      Simulator::ScheduleNow(&BleBBManager::TryAgain,
         this->GetBBManager());
      NS_ASSERT(m_queue !=0);
      NS_ASSERT(packet !=0);
      Ptr<QueueItem> item = Create<QueueItem> (packet);
      NS_ASSERT(item !=0);
      NS_LOG_INFO ("Max size of queue = " << m_queue->GetMaxSize());
      NS_LOG_INFO ("Current size of queue = " << m_queue->GetCurrentSize());
      NS_LOG_INFO ("Size of packet item = " << item->GetSize());
     // std::cout << std::endl;
     // packet->Print(std::cout);
     // std::cout << std::endl;
      if (m_queue->Enqueue (Create<QueueItem> (packet)) == false)
      {
          NS_LOG_LOGIC ("Enqueueing new packet failed");
          m_macTxDropTrace (packet);
          sendOk = false;
      }
        
      if (sendOk)
        m_macTxTrace (packet);
 	  return sendOk;
	}

	void
		BleNetDevice::SetGenericPhyTxStartCallback (GenericPhyTxStartCallback c)
		{
			NS_LOG_FUNCTION (this);
            this->GetLinkController()->SetGenericPhyTxStartCallback(c);
		}

  Ptr<BleBBManager> 
    BleNetDevice::GetBBManager()
    {
      return this->m_bbManager;
    }

  void
    BleNetDevice::SetBBManager(Ptr<BleBBManager> bbManager)
    {
      this->m_bbManager = bbManager;
    }


  Ptr<BleLinkManager> 
    BleNetDevice::GetLinkManager()
    {
      return this->m_linkManager;
    }

  void
    BleNetDevice::SetLinkManager(Ptr<BleLinkManager> linkManager)
    {
      this->m_linkManager = linkManager;
    }


  Ptr<BleLinkController> 
    BleNetDevice::GetLinkController()
    {
      return this->m_linkController;
    }

  void
    BleNetDevice::SetLinkController(Ptr<BleLinkController>
        linkController)
    {
      this->m_linkController = linkController;
    }

   Ptr<BlePrivacyManager> 
    BleNetDevice::GetPrivacyManager()
    {
      return this->m_privacyManager;
    }

  void
    BleNetDevice::SetPrivacyManager(Ptr<BlePrivacyManager> privacyManager)
    {
      this->m_privacyManager = privacyManager;
    } 

  Ptr<DropTailQueue<QueueItem>>
  BleNetDevice::GetQueue (void)
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_queue;
  }

	void
		BleNetDevice::NotifyTransmissionEnd (Ptr<const Packet>)
		{
			NS_LOG_FUNCTION (this);
			NS_ASSERT (m_queue);
		}

    void
		BleNetDevice::NotifyReceptionStart ()
		{
			NS_LOG_FUNCTION (this);
		}

    void
      BleNetDevice::NotifyTXWindowSkipped ()
      {
        NS_LOG_FUNCTION (this);
        m_macTXWindowSkipped (this);
      }

	void
		BleNetDevice::NotifyReceptionEndError (Ptr<Packet> packet)
		{
			NS_LOG_FUNCTION (this);
            m_macRxErrorTrace (packet);
		}

	void
		BleNetDevice::NotifyReceptionEndOk (Ptr<Packet> packet)
		{
			NS_LOG_FUNCTION (this << packet);

            NS_ASSERT(packet != 0);
			BleMacHeader header;
			packet->PeekHeader (header);
			NS_LOG_LOGIC ("packet : Source --> " 
                << header.GetSrcAddr () << " Dest --> " 
                << header.GetDestAddr()
                << "(here: " << m_address << ") protocol: " 
                << header.GetProtocol () );
			m_phy->ChangeState(BlePhy::State::IDLE);

			PacketType packetType;
			if (header.GetDestAddr () == this->GetBroadcast())
			{
				packetType = PACKET_BROADCAST;
			}
			else if ( this->GetPrivacyManager()->ResolveRandMac( this->GetAddress48() ).second == this->GetPrivacyManager()->ResolveRandMac( header.GetDestAddr () ).second )
			{
				packetType = PACKET_HOST;
			}
			else
			{
				packetType = PACKET_OTHERHOST;
			}

			NS_LOG_LOGIC ("packet type = " << packetType );
            Ptr<NetDevice> nd_pointer = Ptr<BleNetDevice>(this);
            Ptr<Packet> packet_copy1 = packet->Copy();
            short unsigned int protocol = header.GetProtocol();
            const Address src_addr = Address(header.GetSrcAddr());
            const Address dest_addr = Address(header.GetDestAddr());
			NS_LOG_LOGIC ("packet size = " << packet_copy1->GetSize() );
			BleMacHeader rmheader;
			packet_copy1->RemoveHeader (rmheader);
            Ptr<const Packet> packet_copy = packet_copy1->Copy();

            NodeTag tag;
		    uint32_t node_id;
		  	if (packet_copy->PeekPacketTag(tag)){
		      //NS_LOG_DEBUG ("Received Packet with SRN = " << tag.Get());
		      node_id = tag.GetNodeId();
			  }
			//NS_LOG_UNCOND(node_id);

            if (packetType == PACKET_BROADCAST )
            {
			  m_macRxBroadcastTrace(packet, this, node_id);
              NS_ASSERT(packet_copy != 0);
              m_rxCallback (nd_pointer, packet_copy, protocol, src_addr);
            }
            else if (packetType != PACKET_OTHERHOST )
			{
			    NS_LOG_LOGIC ("packet : Source --> " 
                    << header.GetSrcAddr () << " Dest --> " 
                    << header.GetDestAddr()
                  << "(here: " << m_address << ") protocol: " 
                  << header.GetProtocol () );
                NS_ASSERT(src_addr.GetLength() > 0);
                NS_ASSERT(dest_addr.GetLength() > 0);
                NS_ASSERT(src_addr.GetLength() == 6);
                NS_ASSERT(dest_addr.GetLength() == 6);
                //NS_ASSERT(header.GetSrcAddr() != 0);
                //NS_ASSERT(header.GetDestAddr() != 0);    
                NS_ASSERT(packet != 0);
				m_macRxTrace(packet, this, node_id);
                NS_ASSERT(packet_copy != 0);
                m_rxCallback (nd_pointer, packet_copy, protocol, src_addr);
                if (protocol>0)//check this(0)... No if condition originally present
		{
		m_promiscRxCallback (nd_pointer, packet_copy, 
                    protocol, src_addr, dest_addr, packetType);
		}
				Simulator::ScheduleNow(&BleBBManager::TryAgain, 
                    this->GetBBManager());
			}
			else // Received packet is not for me
			{
              NS_LOG_INFO ("Arrived packet was not for me");
			}
		}
} // namespace ns3
