/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 KULeuven 
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
 * Author: Stijn Geysen <stijn.geysen@student.kuleuven.be>
 * Author: Abhishek Mishra <abhishek.mishra@inria.fr>
 */


#include "ble-bb-manager.h"
#include <ns3/ble-net-device.h>
#include <ns3/ble-phy.h>
#include <ns3/ble-link-controller.h>
#include <ns3/ble-link.h>
#include <ns3/ble-mac-header.h>
#include <ns3/simulator.h>
#include <ns3/drop-tail-queue.h>
#include <ns3/queue-item.h>
#include "ns3/log.h"
#include "ns3/uinteger.h"

#include <ns3/multi-model-spectrum-channel.h>

namespace ns3 {

  NS_LOG_COMPONENT_DEFINE ("BleBBManager");
  
  NS_OBJECT_ENSURE_REGISTERED (BleBBManager);

  TypeId
    BleBBManager::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::BleBBManager")
        .SetParent<Object> ()
        .AddConstructor<BleBBManager> ()
        .AddAttribute ("BleNetDevice",
            "The Net Device of this device.",
            PointerValue (),
            MakePointerAccessor (&BleBBManager::m_netDevice),
            MakePointerChecker<Object> ())
        // Add attributes and tracesources
        ;
      return tid;
    }

  BleBBManager::BleBBManager ()
  {
    NS_LOG_FUNCTION (this);
  }

  BleBBManager::~BleBBManager ()
  {
    NS_LOG_FUNCTION (this);
  }

  void
    BleBBManager::DoDispose ()
    {
      NS_LOG_FUNCTION (this);
    }

  BleBBManager::BleBBManager (Ptr<BleNetDevice> bleNetDevice)
  {
    NS_LOG_FUNCTION (this);

    m_netDevice = bleNetDevice;
  }

/**********************
 * GETTERS AND SETTER *
 **********************/

  BlePhy::State
    BleBBManager::GetPhyState()
    {
      NS_LOG_FUNCTION (this);
      return this->GetPhy()->GetState();
    }

  Ptr<BlePhy>
    BleBBManager::GetPhy () 
    {
      NS_LOG_FUNCTION (this);
      return this->GetNetDevice()->GetPhy();
    }

  Ptr<DropTailQueue<QueueItem>>
    BleBBManager::GetQueue (void)
    {
      NS_LOG_FUNCTION (this);
      return this->GetNetDevice()->GetQueue();
    }

  Ptr<BleLinkController>
    BleBBManager::GetLinkController()
    {
      NS_LOG_FUNCTION (this);
      return this->GetNetDevice()->GetLinkController();
    }

  Ptr<Packet>
    BleBBManager::GetCurrentPacket()
    {
      return this->GetLinkController()->GetCurrentPacket();
    }

  void
    BleBBManager::SetCurrentPacket(Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION (this);
      this->GetLinkController()->SetCurrentPacket(packet);
    }

  void
    BleBBManager::SetActiveLinkManager (Ptr<BleLinkManager> lm)
    {
      if (lm == 0)
        NS_ASSERT (this->GetPhyState() == BlePhy::State::IDLE);
      else
        NS_ASSERT (this->m_activeLinkManager == 0);
      this->m_activeLinkManager = lm;
    }

  Ptr<BleLinkManager>
    BleBBManager::GetActiveLinkManager ()
    {
      return m_activeLinkManager;
    }

 /******************************
  * END OF GETTERS AND SETTERS *
  ******************************/

  void
    BleBBManager::AddLinkManager(Ptr<BleLinkManager> linkManager)
    {
      NS_LOG_FUNCTION (this << linkManager);
      NS_ASSERT (linkManager->GetAssociatedLink() != 0);
      if (linkManager->GetAssociatedLink()->GetMaster() == 0) 
      {
        // Link has no master set, this BBManager becomes master
        linkManager->GetAssociatedLink()->
          SetMaster(Ptr<BleBBManager>(this));
      }

      if (! LinkManagerExists(linkManager))
      {
        m_linkManagers.push_back(linkManager);
      }
      else
      {
        NS_LOG_WARN ("LinkManager already exists in this baseband manager");
      }
    }
  
  Ptr<BleLink> 
    BleBBManager::CreateLinkScheduledMultipleNodes(
        std::list<Ptr<BleBBManager>> otherBBManagers, bool scheduled, 
        uint32_t nbTxWindowOffset, uint32_t nbConnectionInterval, 
        bool collAvoid)
    {
      NS_LOG_FUNCTION (this);
      // Channel map setup

      // Multiple nodes ==> advertising channels 37, 38, 39
      std::vector<uint8_t> chmap;
      for (int i=37; i< 40; i++)
      {
        chmap.push_back(i);
      }
      uint8_t hopIncr = 1;

      // Manage my LinkManager
      Ptr<BleLinkManager> myLinkManager = CreateObject<BleLinkManager> ();
      NS_ASSERT (myLinkManager->GetQueue() != 0);

      myLinkManager->SetBBManager(Ptr<BleBBManager> (this));
      myLinkManager->SetUsedChannels((chmap));
      myLinkManager->SetHopIncrement (hopIncr);

      
      std::vector<Ptr<BleLinkManager>> otherLinkManagers;
      // Manage the other LinkManagers
      for (auto bbm : otherBBManagers)
      {
        Ptr<BleLinkManager> otherLinkManager = CreateObject<BleLinkManager> ();
        otherLinkManager->SetBBManager(bbm);
        otherLinkManager->SetUsedChannels((chmap));
        otherLinkManager->SetHopIncrement (hopIncr);
        otherLinkManagers.push_back (otherLinkManager);
      }
      
      myLinkManager->SetupLink(otherLinkManagers, scheduled, nbTxWindowOffset, 
          nbConnectionInterval, collAvoid); 
      
      this->AddLinkManager(myLinkManager);
      uint32_t it = 0;
      for (auto bbm : otherBBManagers)
      {
        bbm->AddLinkManager(otherLinkManagers.at(it));
        it++;
      }
      return myLinkManager->GetAssociatedLink();
    }

  Ptr<BleLink> 
    BleBBManager::CreateLinkScheduled(Ptr<BleBBManager> otherBBManager,
        BleLinkManager::Role myRole, bool scheduled, uint32_t nbTxWindowOffset, 
        uint32_t nbConnectionInterval)
    {
      NS_LOG_FUNCTION (this);

      // Create a link manager and let the link manager
      // set up the link
      Ptr<BleLinkManager> myLinkManager = CreateObject<BleLinkManager> ();
      Ptr<BleLinkManager> otherLinkManager = CreateObject<BleLinkManager> ();

      NS_ASSERT (myLinkManager->GetQueue() != 0);
 
      myLinkManager->SetBBManager(Ptr<BleBBManager> (this));
      otherLinkManager->SetBBManager(otherBBManager);

      Ptr<UniformRandomVariable> randT = CreateObject<UniformRandomVariable> ();
      int mapSize = 15;
      std::vector<uint8_t> chmap;
      for (int i=0; i< mapSize; i++)
      {
        chmap.push_back(randT->GetInteger(0,36));
      }
      //std::vector<uint8_t> chmap = {1,4,7,9,11}; 
      uint8_t hopIncr = 2;
      
      myLinkManager->SetUsedChannels((chmap));
      otherLinkManager->SetUsedChannels((chmap));
      myLinkManager->SetHopIncrement (hopIncr);
      otherLinkManager->SetHopIncrement (hopIncr);

      myLinkManager->SetupLink(myRole, otherLinkManager, scheduled, 
          nbTxWindowOffset, nbConnectionInterval); 

      this->AddLinkManager(myLinkManager);
      otherBBManager->AddLinkManager(otherLinkManager);
      
      return myLinkManager->GetAssociatedLink();
    }

 
  Ptr<BleLink> 
    BleBBManager::CreateLink(Ptr<BleBBManager> otherBBManager,
        BleLinkManager::Role myRole)
    {
      NS_LOG_FUNCTION (this);

      // Create a link manager and let the link manager
      // set up the link
      Ptr<BleLinkManager> myLinkManager = CreateObject<BleLinkManager> ();
      Ptr<BleLinkManager> otherLinkManager = CreateObject<BleLinkManager> ();

      NS_ASSERT (myLinkManager->GetQueue() != 0);
 
      myLinkManager->SetBBManager(Ptr<BleBBManager> (this));
      otherLinkManager->SetBBManager(otherBBManager);

      Ptr<UniformRandomVariable> randT = CreateObject<UniformRandomVariable> ();
      int mapSize = 15;
      std::vector<uint8_t> chmap;
      for (int i=0; i< mapSize; i++)
      {
        chmap.push_back(randT->GetInteger(1,37));
      }
      //std::vector<uint8_t> chmap = {1,4,7,9,11}; 
      uint8_t hopIncr = 2;
      
      myLinkManager->SetUsedChannels((chmap));
      otherLinkManager->SetUsedChannels((chmap));
      myLinkManager->SetHopIncrement (hopIncr);
      otherLinkManager->SetHopIncrement (hopIncr);

      myLinkManager->SetupLink(myRole, otherLinkManager, false, 0, 0); 

      this->AddLinkManager(myLinkManager);
      otherBBManager->AddLinkManager(otherLinkManager);
      
      return myLinkManager->GetAssociatedLink();
    }

  bool
    BleBBManager::LinkManagerExists (Ptr<BleLinkManager> linkManager)
    {
      NS_LOG_FUNCTION (this);
      std::list<Ptr<BleLinkManager>>::iterator it;
      for (it = m_linkManagers.begin(); it != m_linkManagers.end(); ++it)
      {
        if (*it == linkManager) 
        {
          return true;
        }
      }
      return false;
    }

  bool
    BleBBManager::LinkExists (Ptr<BleLink> link)
    {
      NS_LOG_FUNCTION (this);
      std::list<Ptr<BleLinkManager>>::iterator it;
      for (it = m_linkManagers.begin(); it != m_linkManagers.end(); ++it)
      {
        Ptr<BleLinkManager> lm = *it;
        NS_ASSERT(lm->GetAssociatedLink() != 0);
        NS_ASSERT(link != 0);
        if (lm->GetAssociatedLink() == link) 
        {
          return true;
        }
      }
      return false;
    }


  Ptr<BleNetDevice>
    BleBBManager::GetNetDevice()
    {
      return m_netDevice;
    }

  void
    BleBBManager::SetNetDevice(Ptr<BleNetDevice> netDevice)
    {
      NS_LOG_FUNCTION (this);
      m_netDevice = netDevice;
    }

  void
    BleBBManager::SetPhy (Ptr<BlePhy> phy)
    {
      NS_LOG_FUNCTION (this);
      this->GetNetDevice()->SetPhy(phy);
    }

  bool
    BleBBManager::LinkExists (Mac48Address address)
    {
      NS_LOG_FUNCTION (this);
      std::list<Ptr<BleLinkManager>>::iterator it;
      for (it = m_linkManagers.begin(); it != m_linkManagers.end(); ++it)
      {
        std::list<Ptr<BleBBManager>>::iterator it2;
        Ptr<BleLinkManager> lm = *it;
        Ptr<BleLink> temp = lm->GetAssociatedLink();
        if (temp->GetLinkType() == BleLink::LinkType::BROADCAST 
            && address == Mac48Address("FF:FF:FF:FF:FF:FF"))
          return true;
        std::list<Ptr<BleBBManager>> all_devices = temp->GetLinkedDevices();
        for (it2 = all_devices.begin(); it2 != all_devices.end(); ++it2)
        {
          Ptr<BleBBManager> bbm = *it2;
          NS_ASSERT(bbm != 0);
          Ptr<BleNetDevice> nd = bbm->GetNetDevice ();
          NS_ASSERT(nd != 0);
          if (this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( nd->GetAddress48() ).second == this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( address ).second
              && temp->GetLinkType() != BleLink::LinkType::BROADCAST)
          {
            return true;
          }
        }
      }
      return false;
    }
  
  
  Ptr<BleLinkManager>
    BleBBManager::GetLinkManager (Mac48Address address)
    {
      NS_LOG_FUNCTION (this);
      std::list<Ptr<BleLinkManager>>::iterator it;
      for (it = m_linkManagers.begin(); it != m_linkManagers.end(); ++it)
      {
        std::list<Ptr<BleBBManager>>::iterator it2;
        Ptr<BleLinkManager> lm = *it;
        Ptr<BleLink> temp = lm->GetAssociatedLink();
        if (temp->GetLinkType() == BleLink::LinkType::BROADCAST 
            && address == Mac48Address("FF:FF:FF:FF:FF:FF"))
          return lm;
        std::list<Ptr<BleBBManager>> all_devices = temp->GetLinkedDevices();
        for (it2 = all_devices.begin(); it2 != all_devices.end(); ++it2)
        {
          Ptr<BleBBManager> bbm = *it2;
          NS_ASSERT(bbm != 0);
          Ptr<BleNetDevice> nd = bbm->GetNetDevice ();
          NS_ASSERT(nd != 0);
          if (this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( nd->GetAddress48() ).second == this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( address ).second
              && temp->GetLinkType() != BleLink::LinkType::BROADCAST) 
          {
            return lm;
          }
        }
      }
      NS_LOG_WARN ("There is no link to a device with address " << address);
      Ptr<BleLinkManager> lm = CreateObject<BleLinkManager> ();
      return lm;
    }
  

  Ptr<BleLink>
    BleBBManager::GetLink (Mac48Address address)
    {
      NS_LOG_FUNCTION (this);
      std::list<Ptr<BleLinkManager>>::iterator it;
      for (it = m_linkManagers.begin(); it != m_linkManagers.end(); ++it)
      {
        std::list<Ptr<BleBBManager>>::iterator it2;
        Ptr<BleLinkManager> lm = *it;
        Ptr<BleLink> temp = lm->GetAssociatedLink();
        if (temp->GetLinkType() == BleLink::LinkType::BROADCAST 
            && address == Mac48Address("FF:FF:FF:FF:FF:FF"))
          return temp;
        std::list<Ptr<BleBBManager>> all_devices = temp->GetLinkedDevices();
        for (it2 = all_devices.begin(); it2 != all_devices.end(); ++it2)
        {
          Ptr<BleBBManager> bbm = *it2;
          NS_ASSERT(bbm != 0);
          Ptr<BleNetDevice> nd = bbm->GetNetDevice ();
          NS_ASSERT(nd != 0);
          if (this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( nd->GetAddress48() ).second == this->GetNetDevice()->GetPrivacyManager()->ResolveRandMac( address ).second 
              && temp->GetLinkType() != BleLink::LinkType::BROADCAST)
          {
            return temp;
          }
        }
      }
      NS_LOG_WARN ("There is no link to a device with address " << address);
      Ptr<BleLink> link = CreateObject<BleLink> ();
      return link;
    }
  
  
  uint32_t
    BleBBManager::CountLinks()
    {
      return m_linkManagers.size();
    }

   void
    BleBBManager::TryAgain()
    {
      HandlePacket();
    }

   void
     BleBBManager::HandlePacket()
     {
       NS_LOG_FUNCTION (this);
       while (this->GetNetDevice()->GetQueue()->IsEmpty () == false)
       {
         NS_LOG_INFO ("Queue is not empty");
         Ptr<QueueItem> item = this->GetNetDevice()->GetQueue()->Dequeue ();
         NS_ASSERT (item);
         Ptr<Packet> packet = item->GetPacket ();
        
         // Check dest address in header and get a link to the address
         BleMacHeader macheader;
         packet->PeekHeader(macheader);
         Mac48Address destAddr = macheader.GetDestAddr();
         NS_LOG_INFO ("Destination addr of current packet: " << destAddr); 
         bool linkExists = LinkExists (destAddr);
         if (!linkExists)
         {
           NS_LOG_ERROR (" No link exists to destination address " << destAddr);
          // (if time allows: implement:) setup a link to the destination address
          NS_ASSERT(linkExists);
         }
         else
         {
           NS_LOG_INFO (" Link to destination of current packet exists ");
           Ptr<BleLinkManager> activeLinkManager = GetLinkManager (destAddr);
           activeLinkManager->GetQueue ()->Enqueue (item);
           
         }
       } // Queue was not empty
       NS_LOG_INFO( "Queue is empty");
     }

 }

