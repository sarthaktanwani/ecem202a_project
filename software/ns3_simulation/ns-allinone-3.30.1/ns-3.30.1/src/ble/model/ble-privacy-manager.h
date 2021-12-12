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
 */

#ifndef BLE_PRIVACY_MANAGER_H
#define BLE_PRIVACY_MANAGER_H

#include <cstring>
#include <ns3/core-module.h>
#include <ns3/trace-helper.h>
#include <ns3/address.h>
#include <ns3/net-device.h>
#include <ns3/callback.h>
#include <ns3/packet.h>
#include <ns3/traced-callback.h>
#include <ns3/nstime.h>
#include <ns3/ptr.h>
#include <ns3/mac16-address.h>
#include <ns3/mac48-address.h>
#include <ns3/generic-phy.h>
#include <ns3/random-variable-stream.h>
#include <ns3/event-id.h>
#include <ns3/backoff.h>
#include <vector>

#include <ns3/constants.h>


namespace ns3 {

  //class BleNetDevice;

class BlePrivacyManager: public Object
{
	/**
		* See also in higher classes
		*/
public:
  static TypeId GetTypeId (void);

  BlePrivacyManager ();
  ~BlePrivacyManager () {}
  void DoDispose (void);

  Ptr<BleNetDevice> GetNetDevice ();
  void SetNetDevice (Ptr<BleNetDevice> netDevice);

  void UpdateAddress(uint32_t node_id);//call the updation of the device Mac Address
  void GenerateRandomMac();//Generates the random Mac addresses

  void SetIdentityAddress();//set identity address for this device
  std::string GetIdentityAddress (void);

  void SetDeviceClass( int d_class);
  int GetDeviceClass() const { return m_device_class; }
  void SetBleStandard( int b_std);
  void SetRandInterval(int in_intvl=15 * 60);
  void SetDswap();
  void SetStream( Ptr<OutputStreamWrapper> mac_stream );
  void SetMacMap( std::map<std::string, uint32_t> &mac_map );

  void SetIRK(void); //set the IRK for this device
  std::string GetIRK(void); //get the IRK for this device
  void AddIRK(std::string in_key, std::string in_dev); // store a IRK and Identity address of a paired device
  std::string GenPrand(); // generate a 24 bit prand for the rand MAC generation

  std::pair<bool, std::string> ResolveRandMac(Mac48Address sender_address); // Resolve the sender of the packet which has randomized MAC
  							// returns a pair of [(true & identity address of sender), if resolution was successful; else (false & empty string)]

  uint32_t m_node_id;
  Mac48Address m_identity_address; //!< Public static device address
  std::string m_str_identity_address; // !< Public static device address in string format(just present for the ease of parsing!)
  std::string m_IRK; //!< identity resolving key(IRK) of this device
  std::vector< std::pair<std::string, std::string> > m_stored_IRKs; //!< stored IRK and identity address pairs of other paired devices

  int m_device_class;//class of device: Apple, Samsung e.t.c
  int m_ble_standard;//the ble standard used for the randomization
  int m_randomization_interval;//the interval at which the device gets a new device address in seconds
  double m_dswap; //time taken by a node to change the randomized MAC address
  Ptr<OutputStreamWrapper> m_mac_stream; //stream to keep track of MAC Randomization
  bool first_mac_rand_in_sniff;
  std::map<std::string, uint32_t> m_mac_map;//map from anon mac to node id

  Ptr<BleNetDevice> m_netDevice;

};

}// namespace ns3

#endif /* BLE_NET_DEVICE_H */