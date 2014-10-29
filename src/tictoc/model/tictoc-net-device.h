/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

/* -*- Mode:C++; -*- */
/*
 * Copyright (c) 2014 Yusuke Sugiyama
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
 * Foundation, Inc., Saruwatari Lab, Shizuoka University, Japan
 *
 * Author: Yusuke Sugiyama <sugiyama@aurum.cs.inf.shizuoka.ac.jp>
 */
#ifndef TICTOC_NET_DEVICE_H
#define TICTOC_NET_DEVICE_H

#include <stdint.h>
#include <string>
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "tictoc-channel.h"

namespace ns3 {

class TictocChanel;
class Node;
class ErrorModel;

/**
 * \ingroup netdevice
 *
 * This device does not have a helper and assumes 48-bit mac addressing;
 * the default address assigned to each device is zero, so you must 
 * assign a real address to use it.  There is also the possibility to
 * add an ErrorModel if you want to force losses on the device.
 * 
 * \brief simple net device for simple things and testing
 */
class TictocNetDevice : public NetDevice
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TictocNetDevice ();
  /**
   * Receive a packet from a connected TictocChannel.  The 
   * TictocNetDevice receives packets from its connected channel
   * and then forwards them by calling its rx callback method
   *
   * \param packet Packet received on the channel
   * \param protocol protocol number
   * \param to address packet should be sent to
   * \param from address packet was sent from
   */
  void Receive (Ptr<Packet> packet, uint16_t protocol, Mac48Address to, Mac48Address from);
  
  /**
   *
   * Attach a channel to this net device.  This will be the 
   * channel the net device sends on
   * 
   * \param channel channel to assign to this net device
   *
   */
  void SetChannel (Ptr<TictocChannel> channel);
  /**
   * Attach a receive ErrorModel to the TictocNetDevice.
   *
   * The TictocNetDevice may optionally include an ErrorModel in
   * the packet receive chain.
   *
   * \see ErrorModel
   * \param em Ptr to the ErrorModel.
   */
  void SetReceiveErrorModel (Ptr<ErrorModel> em);

  // inherited from NetDevice base class.
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

protected:
  virtual void DoDispose (void);
private:
  Ptr<TictocChannel> m_channel; //!< the channel the device is connected to
  NetDevice::ReceiveCallback m_rxCallback; //!< Receive callback
  NetDevice::PromiscReceiveCallback m_promiscCallback; //!< Promiscuous receive callback
  Ptr<Node> m_node; //!< Node this netDevice is associated to
  uint16_t m_mtu;   //!< MTU
  uint32_t m_ifIndex; //!< Interface index
  Mac48Address m_address; //!< MAC address
  Ptr<ErrorModel> m_receiveErrorModel; //!< Receive error model
  TracedCallback<Ptr<const Packet> > m_phyRxDropTrace;
};

} // namespace ns3

#endif /* TICTOC_NET_DEVICE_H */
