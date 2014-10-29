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
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/error-model.h"
#include "tictoc-net-device.h"

NS_LOG_COMPONENT_DEFINE ("TictocNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TictocNetDevice);

TypeId 
TictocNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TictocNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<TictocNetDevice> ()
    .AddAttribute ("ReceiveErrorModel",
                   "The receiver error model used to simulate packet loss",
                   PointerValue (),
                   MakePointerAccessor (&TictocNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddTraceSource ("PhyRxDrop",
                     "Trace source indicating a packet has been dropped by the device during reception",
                     MakeTraceSourceAccessor (&TictocNetDevice::m_phyRxDropTrace))
  ;
  return tid;
}

TictocNetDevice::TictocNetDevice ()
  : m_channel (0),
    m_node (0),
    m_mtu (0xffff),
    m_ifIndex (0)
{
  NS_LOG_FUNCTION (this);
}

void
TictocNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol,
                          Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << protocol << to << from);
  NetDevice::PacketType packetType;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (packet) )
    {
      m_phyRxDropTrace (packet);
      return;
    }

  if (to == m_address)
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsBroadcast ())
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsGroup ())
    {
      packetType = NetDevice::PACKET_MULTICAST;
    }
  else 
    {
      packetType = NetDevice::PACKET_OTHERHOST;
    }
  //  m_rxCallback (this, packet, protocol, from);
  if (!m_promiscCallback.IsNull ())
    {
      NS_LOG_DEBUG (packetType);
      m_promiscCallback (this, packet, protocol, from, to, packetType);
    }
  
  m_channel->Send (packet, protocol, Mac48Address ("ff:ff:ff:ff:ff:ff"), m_address, this);
}
void 
TictocNetDevice::SetChannel (Ptr<TictocChannel> channel)
{
  NS_LOG_FUNCTION (this << channel);
  m_channel = channel;
}

void
TictocNetDevice::SetReceiveErrorModel (Ptr<ErrorModel> em)
{
  NS_LOG_FUNCTION (this << em);
  m_receiveErrorModel = em;
}
void 
TictocNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}
uint32_t 
TictocNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}
Ptr<Channel>
TictocNetDevice::GetChannel (void) const
{
  return m_channel;
}
void
TictocNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = Mac48Address::ConvertFrom (address);
}
Address 
TictocNetDevice::GetAddress (void) const
{
  //
  // Implicit conversion from Mac48Address to Address
  //
  NS_LOG_FUNCTION (this);
  return m_address;
}
bool 
TictocNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  m_mtu = mtu;
  return true;
}
uint16_t 
TictocNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mtu;
}
bool 
TictocNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}
void 
TictocNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
 NS_LOG_FUNCTION (this << &callback);
}
bool 
TictocNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}
Address
TictocNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
TictocNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}
Address 
TictocNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this << multicastGroup);
  return Mac48Address::GetMulticast (multicastGroup);
}

Address TictocNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

bool 
TictocNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool 
TictocNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool 
TictocNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  m_channel->Send (packet, protocolNumber, to, m_address, this);
  return true;
}
bool 
TictocNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  Mac48Address from = Mac48Address::ConvertFrom (source);
  m_channel->Send (packet, protocolNumber, to, from, this);
  return true;
}

Ptr<Node> 
TictocNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}
void 
TictocNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}
bool 
TictocNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}
void 
TictocNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_rxCallback = cb;
}

void
TictocNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_node = 0;
  m_receiveErrorModel = 0;
  NetDevice::DoDispose ();
}


void
TictocNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_promiscCallback = cb;
}

bool
TictocNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

} // namespace ns3
