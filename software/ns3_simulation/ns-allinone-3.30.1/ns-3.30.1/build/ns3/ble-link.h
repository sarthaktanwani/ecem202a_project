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


#ifndef BLE_LINK_H
#define BLE_LINK_H

// Includes
#include <ns3/callback.h>
#include <ns3/traced-callback.h>
#include <ns3/object.h>
#include <ns3/ptr.h>
#include <ns3/pointer.h>
#include <ns3/mac48-address.h>

#include <ns3/spectrum-channel.h>

//#include <ns3/ble-bb-manager.h>

namespace ns3 {

  // Classes

  class BleBBManager;
/** 
 * \ingroup ble
 * \brief Implementation for the Link of the BLE protocol
 * This implements the following features:
 *
 */
  class BleLink : public Object
  {
    public:

      // Although not real 'links', a node that is broadcasting or
      // scanning can also be assigned a LinkType
      enum LinkType
      {
        UNCONNECTED,
        POINT_TO_POINT,
        BROADCAST,
        MULTICAST, // Not Supported in BLE
        SCANNER
      };

      BleLink ();
      ~BleLink ();

      static TypeId GetTypeId (void);

      LinkType GetLinkType();
      void SetLinkType(LinkType linkType);

      void AddSlave (Ptr<BleBBManager> bleBBManager);
      void SetMaster(Ptr<BleBBManager> bleBBManager);
      Ptr<BleBBManager> GetMaster();

      std::list<Ptr<BleBBManager>> GetLinkedDevices();

      void SetChannel (Ptr<SpectrumChannel> c);
      Ptr<SpectrumChannel> GetChannel();
      
      Ptr<BleBBManager> GetLinkedDevice (Mac48Address addr);

    private:
      LinkType currentLinkType;
      std::list<Ptr<BleBBManager>> m_slaves; 
      Ptr<BleBBManager> m_master;

      Ptr<SpectrumChannel> m_channel;

  };

}

#endif /* BLE_LINK_H */

