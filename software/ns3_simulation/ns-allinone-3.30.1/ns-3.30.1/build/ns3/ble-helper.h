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


#ifndef BLE_HELPER_H
#define BLE_HELPER_H

#include "ns3/ble-module.h"
#include <ns3/node-container.h>
#include <ns3/application-container.h>
#include <ns3/trace-helper.h>
#include <ns3/callback.h>
#include <ns3/ble-phy.h>
#include <ns3/generic-phy.h>

#include "ns3/attribute.h"
#include "ns3/object-factory.h"

namespace ns3 {

  class SpectrumChannel;
  class MobilityModel;
  class RandomVariableStream;
  /**
   * \ingroup ble
   *
   * \brief helps to manage and create BLE NetDevice objects
   *
   * This class can help to create BLE NetDevice objects
   * and to configure their attributes during creation. It also contains
   * additional helper functions used by client code.
   *
   * Only one channel is created, and all devices attached to it. If 
   * multiple channels are needed, multiple helper objects must be used,
   * or else the channel object must be replaced.
   */

  class BleHelper: public PcapHelperForDevice,
  public AsciiTraceHelperForDevice
  {
  public:

    /**
     * \brief Generate constant traffic from dev
     * \internal
     *
     */

    ApplicationContainer GenerateTraffic(Ptr <RandomVariableStream> var, 
        NodeContainer nodes, int packet_size, double start, 
        double duration, double interval);
    Ptr<Application> GenerateTraffic(Ptr <RandomVariableStream> var, 
        Ptr<Node> node, int packet_size, double start, double duration, 
        double interval, Ptr<Node> destination);

    ApplicationContainer GenerateBroadcastTraffic(Ptr <RandomVariableStream> var, 
        NodeContainer nodes, int packet_size, double start, 
        double duration, double interval, int mobility_profile, std::vector<int> dev_class);
    Ptr<Application> GenerateBroadcastTraffic(Ptr<RandomVariableStream> var, 
        Ptr<Node> node, int packet_size, double start, 
        double duration, double interval, double offset, int mobility_profile);

    /**
     * \brief Create a BLE helper in an empty state.
     */
    BleHelper (void);

    /**
     * \brief BLE helper destructor
     */
    virtual ~BleHelper (void);

    /**
     * \brief Set the channel associated to this helper
     * \param channel the channel
     */
    void SetChannel (Ptr<SpectrumChannel> channel);

    /**
     * \brief Set the channel associated to this helper
     * \param channelName the channel name
     */
    void SetChannel (std::string channelName);

    void SetChannelAttribute (std::string n1, const AttributeValue & v1);

    /**
     * \brief Get the channel associated to this helper
     * \returns the channel
     */
    Ptr<SpectrumChannel> GetChannel (void);

    /**
     * \brief Add mobility model to a physical device
     * \param phy the physical device
     * \param m the mobility model
     */
    void AddMobility (Ptr<BlePhy> phy, Ptr<MobilityModel> m);

    /**
     * \param c a set of nodes
     * \parm only_transmit is true if we want the node to act just as a sender not receiver
     * \returns A container holding the added net devices.
     */
    NetDeviceContainer Install (NodeContainer c, bool only_transmit);

    void AddCallbacks (std::string traceSource, CallbackBase callback);
    
    /**
     * Helper to enable all Ble log components with one statement
     */
    void EnableLogComponents (void);

    /**
     * Assign a fixed random variable stream number to the random variables
     * used by this model. Return the number of streams that have been
     * assigned. The Install() method should have previously been
     * called by the user.
     *
     * \param c NetDeviceContainer of the set of net devices for which the 
     *          CsmaNetDevice should be modified to use a fixed stream
     * \param stream first streaem index to use
     * \return the number of stream indices assigned by this helper
     */
    int64_t AssignStreams (NetDeviceContainer c, int64_t stream);


    /*
     * Creates all possible links between the devices in the container
     * (fully connected mesh)
     */
    void CreateAllLinks (NetDeviceContainer c, 
        bool scheduled, uint32_t nbConnInterval);

    /*
     * Setups a broadcast link
     */
    void CreateBroadcastLink (NetDeviceContainer c, 
        bool scheduled, uint32_t nbConnInterval, bool collAvoid);

/**
   * \param type the type of the model to set
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
	 * Install another network application 
   */
  void InstallNetworkApplication (std::string type,
        std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
        std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
        std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
        std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
        std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
        std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
        std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
        std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

private:
	// Disable implicit constructors
  /**
   * \brief Copy constructor - defined and not implemented.
   */
	BleHelper (BleHelper const &);
  /**
   * \brief Copy constructor - defined and not implemented.
   * \returns
   */
	BleHelper& operator= (BleHelper const &);
	/**
	 * \brief Enable pcap output on the indicated net device.
	 *
	 * NetDevice-specific implementation mechanism for hooking the trace and
	 * writing to the trace file.
	 *
	 * \param prefix Filename prefix to use for pcap files.
	 * \param nd Net device for which you want to enable tracing.
   * \param promiscuous If true capture all possible packets 
   *            available at the device.
   * \param explicitFilename Treat the prefix as an explicit filename if true
   */
  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, 
      bool promiscuous, bool explicitFilename);


	/**
	 * \brief Enable ascii trace output on the indicated net device.
	 *
	 * NetDevice-specific implementation mechanism for hooking the trace and
	 * writing to the trace file.
	 *
	 * \param stream The output stream object to use when logging ascii traces.
	 * \param prefix Filename prefix to use for ascii trace files.
	 * \param nd Net device for which you want to enable tracing.
   * \param explicitFilename Treat the prefix as an explicit filename if true
	 */
	virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
			std::string prefix,
			Ptr<NetDevice> nd,
			bool explicitFilename);

    void ConstructAllChannels ();

  Ptr<SpectrumChannel> m_channel; //!< channel to be used for the devices
	
  typedef std::tuple<std::string,CallbackBase> callbacktuple;
  std::list<callbacktuple > m_callbacks;
  
  std::list<ObjectFactory> m_netApp; 
        //!< These are the applications installed on the network server
	Ptr<const SpectrumModel> m_spectrumModel;
  std::vector<Ptr<SpectrumChannel>> m_allChannels;
  

  ObjectFactory m_queueFactory;
  ObjectFactory m_deviceFactory;
  ObjectFactory m_channelFactory;

};

}

#endif /* BLE_HELPER_H */

