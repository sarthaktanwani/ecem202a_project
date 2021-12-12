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
 *  Based on lora-application from the lora ns-3 module,
 *      designed by Brecht Reynders.
 *          This module can be found here:
 *https://github.com/networkedsystems/lora-ns3/blob/master/model/lora-mac-header.h
 *
 */

#ifndef BLE_APPLICATION_H
#define BLE_APPLICATION_H

#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/callback.h"
#include "ns3/application.h"
#include <ns3/mac48-address.h>
#include <ns3/random-variable-stream.h>
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/includes.h"

namespace ns3 {

	class Node;
	class Socket;

	/**
	 * \ingroup ble
	 * \brief Ble default application.
	 *
	 * This application generates empty packets every X seconds.  
	 * This application mimics a sensor. 
	 */
	class BleApplication : public Application
	{
		public:
			/**
			 * \brief Get the type ID.
			 * \return the object TypeId
			 */
			static TypeId GetTypeId (void);
			BleApplication ();
			virtual ~BleApplication ();

            void HandleBle (Ptr<Socket> socket);
            void SetNetDevice (Ptr<NetDevice> device);

		private:
			/**
			 * This function is the function to schedule a sensing event. 
			 * With every sensing event, we generate the packet.
			 */
			void Sense (void);

            Mac48Address m_destination;
            Ptr<NetDevice> m_device;

			EventId m_SenseEvent;     
              //!< The event that will fire at m_stopTime to end the application
			Ptr<Socket> m_socket;		
              //!< The socket for this application. 
              // It sends directly to the netdevice (no routing!)
			uint8_t m_dataSize;		//!< The size of an empty message
			uint8_t m_port;			
              //!< The port to use (this is equal to the port MAC field in Ble)
			Time m_interPacketTime;	//!< The time between two packets
            Time m_timeOffset; //!< Time before first packet is send
            Time m_app_stop_time; //!< Time to stop this app instance
            Time m_app_start_time; //!< Time to start this app instance
            Ptr<UniformRandomVariable> m_rval; //!< uniform random variable generator
		protected:
			virtual void DoDispose (void);
			virtual void DoInitialize (void);
			/**
			 * \brief Application specific startup code
			 *
			 * The StartApplication method is called at the start time specified by Start
			 * This method should be overridden by all or most application
			 * subclasses.
			 */
			void StartApplication (void);

			/**
			 * \brief Application specific shutdown code
			 *
			 * The StopApplication method is called at the stop time specified by Stop
			 * This method should be overridden by all or most application
			 * subclasses.
			 */
			void StopApplication (void);

			void StartThisApplication(void);

	};

} // namespace ns3

#endif /* BLE_APPLICATION_H */
