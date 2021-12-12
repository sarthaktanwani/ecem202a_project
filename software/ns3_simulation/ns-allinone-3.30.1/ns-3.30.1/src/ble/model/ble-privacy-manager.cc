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
#include <ns3/core-module.h>
//#include <ns3/ble-privacy-manager.h>
#include "ns3/ble-module.h"
//#include "ns3/ns-tag.h"
#include "ns3/includes.h"
#include <ns3/random-variable-stream.h>
#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

TypeId
BlePrivacyManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BlePrivacyManager")
    .SetParent<NetDevice> ()
    .AddConstructor<BlePrivacyManager> ()
    ;
  return tid;
}

BlePrivacyManager::BlePrivacyManager ()
{
  m_randomization_interval=0;
  m_dswap=0.0;
  first_mac_rand_in_sniff=true;
  SetIRK();
  SetIdentityAddress();
}
  
/*BlePrivacyManager::~BlePrivacyManager ()
{
}*/

void
BlePrivacyManager::DoDispose ()
{
  //NS_LOG_FUNCTION (this);
}

Ptr<BleNetDevice>
BlePrivacyManager::GetNetDevice()
{
  return m_netDevice;
}

void
BlePrivacyManager::SetNetDevice(Ptr<BleNetDevice> netDevice)
{
  //NS_LOG_FUNCTION (this);
  m_netDevice = netDevice;
}

std::string BlePrivacyManager::GetIdentityAddress (void)
{
  return m_str_identity_address;
}

void BlePrivacyManager::SetDswap() 
{
  Ptr<NormalRandomVariable> x = CreateObject<NormalRandomVariable> ();
  m_dswap = x->GetValue(2.7, 0.5);
}

void BlePrivacyManager::SetStream( Ptr<OutputStreamWrapper> mac_stream )
{
  m_mac_stream = mac_stream;
}

void BlePrivacyManager::SetMacMap( std::map<std::string, uint32_t> &mac_map )
{
  m_mac_map = mac_map;
}

void BlePrivacyManager::SetIRK(void)
    {
      auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };

        std::string key_str(16,0);
        std::generate_n( key_str.begin(), 16, randchar );//thanks to stack overflow @ Ates Goral
        //NS_LOG_UNCOND(key_str);
      std::string key(key_str);
      this->m_IRK = key;
    }

  std::string BlePrivacyManager::GenPrand()
    {
      auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };

        std::string prand_str(3,0);
        std::generate_n( prand_str.begin(), 3, randchar ); //TODO - add tariling 10 bits
      return prand_str;
    }

  std::string BlePrivacyManager::GetIRK(void)
    {
      return m_IRK;
    }

  void BlePrivacyManager::AddIRK(std::string in_key, std::string in_dev)
    {
      m_stored_IRKs.emplace_back(in_key, in_dev);
    }

  void BlePrivacyManager::SetIdentityAddress()
      {
        std::string val_prand_iden1 = GenPrand();
      std::string val_iden = val_prand_iden1 + GenPrand();

          val_iden = hexlify(val_iden);
    
          int itp=0;
          while (itp<5)
          { 
            val_iden.insert(itp*3+2,1,':');
            itp++;
          }
          //NS_LOG_UNCOND(rand_mac);
          m_str_identity_address = val_iden;
          char const * char_array = val_iden.c_str();

          m_identity_address = Mac48Address (char_array);
    }

   std::pair<bool, std::string> BlePrivacyManager::ResolveRandMac(Mac48Address sender_address)
    {

      uint8_t buffer[6];
        sender_address.CopyTo(buffer);
        std::string this_prand = "";
        std::string this_hash = "";
        for(int j=0; j<6;j++)
        {
          if (j<3)
            this_hash+=makehex(buffer[j]);
          else
            this_prand+=makehex(buffer[j]);
        }
      
      bool resolved=false;
      std::string identity_address="";
      std::string plaintext(zfill(unhexlify(this_prand), 16, '0'));
      std::string ciphertext;
      std::string local_hash;

        for(unsigned int j=0; j< m_stored_IRKs.size(); j++)
        {
        AES instance(m_stored_IRKs[j].first);
            ciphertext = instance.encrypt(plaintext);

            local_hash = ciphertext.substr(ciphertext.size()-3, ciphertext.size());
            local_hash = hexlify(local_hash);

            if (local_hash==this_hash)
            {
              resolved=true;
              identity_address=m_stored_IRKs[j].second;
            }
            //NS_LOG_UNCOND(this_hash << "\t" << this_prand << "\t" << sender_address << "\t" << local_hash);
        }
        
        return std::make_pair(resolved, identity_address);
    }

  void 
    BlePrivacyManager::GenerateRandomMac()
    {

      std::string val_prand = GenPrand();
      std::string plaintext(zfill(val_prand, 16, '0'));

      AES instance(m_IRK);
          std::string ciphertext = instance.encrypt(plaintext);//ciphertext output is 32 bytes long

          std::string rand_mac = ciphertext.substr(ciphertext.size()-3, ciphertext.size()) + val_prand;
          rand_mac = hexlify(rand_mac);
    
          int itp=0;
          while (itp<5)
          { 
            rand_mac.insert(itp*3+2,1,':');
            itp++;
          }
          //NS_LOG_UNCOND(rand_mac << "\t" << hexlify(val_prand));
          char const * char_array = rand_mac.c_str();

          this->GetNetDevice ()->SetAddress(Mac48Address (char_array));

          //NS_LOG_UNCOND(m_randomization_interval);

          if(m_mac_stream)
          {
            *m_mac_stream->GetStream () << rand_mac.c_str() << "\t" << m_node_id << std::endl;
            m_mac_map.insert(std::pair<std::string, uint32_t>(rand_mac, m_node_id));
          }

          if (first_mac_rand_in_sniff)
          {
            Ptr<UniformRandomVariable> xval = CreateObject<UniformRandomVariable> ();
            first_mac_rand_in_sniff=false;
            Simulator:: Schedule (Seconds(xval->GetValue(0, m_randomization_interval) + m_dswap), &BlePrivacyManager::GenerateRandomMac, this);
          }
          else
          {
            Simulator:: Schedule (Seconds(m_randomization_interval + m_dswap), &BlePrivacyManager::GenerateRandomMac, this);
            ResolveRandMac(this->GetNetDevice ()->GetAddress48());
          }
    }

  void 
    BlePrivacyManager::UpdateAddress(uint32_t node_id)
    {
      m_node_id=node_id;
      if (m_randomization_interval==0)
        SetRandInterval();
      GenerateRandomMac();
      SetDswap();
      //Simulator:: Schedule (Seconds(m_randomization_interval), &BlePrivacyManager::GenerateRandomMac, this);
    }

  void 
    BlePrivacyManager::SetDeviceClass( int d_class)
    {
      /*  
      We define a set of class of devices: (Could be expanded in future)
      
      0 - Frequent Emitters
      1 - Moderate  Emitters
      2 - Semi-Moderate  Emitters
      3 - Low  Emitters

      */
      m_device_class=d_class;
    }

    void 
      BlePrivacyManager::SetBleStandard( int b_std)
      {
        /*  
          We define a set of supported BLE standards for MAC anonymization: (Could be expanded in future)
          
          0 - BLE 4.0
          1 - BLE 5.2
      
        */
        m_ble_standard=b_std;
      }

    void 
      BlePrivacyManager::SetRandInterval(int in_intvl)
      {
        /*  
          We define random interval for specified device class and BLE standard: (Could be expanded in future)
        */
        if (m_ble_standard==0)//m_device_class==0 && 
        {
          m_randomization_interval=in_intvl;
        }
        else if (m_ble_standard==1)//m_device_class==0 && 
        {
          m_randomization_interval=in_intvl;
        }
        else
        {
          m_randomization_interval=in_intvl;
        }
      }

}// namespace ns3