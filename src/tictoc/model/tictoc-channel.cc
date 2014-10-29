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

#include "tictoc-channel.h"
#include "tictoc-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("TictocChannel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TictocChannel);

TypeId 
TictocChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TictocChannel")
    .SetParent<Channel> ()
    .AddConstructor<TictocChannel> ()
    ;
  return tid;
}
  
TictocChannel::TictocChannel ()
{
  NS_LOG_FUNCTION (this);
}


uint32_t
TictocChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION (this);
  return m_devices.size ();
}

Ptr<NetDevice>
TictocChannel::GetDevice (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return m_devices[i];
}

void
TictocChannel::Add (Ptr<TictocNetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  m_devices.push_back (device);
}

void
TictocChannel::Send (Ptr<Packet> p, uint16_t protocol,
		     Mac48Address to, Mac48Address from,
		     Ptr<TictocNetDevice> sender)
{
  NS_LOG_FUNCTION (this << p << protocol << to << from << sender);
  for (std::vector<Ptr<TictocNetDevice> >::const_iterator i = m_devices.begin (); i != m_devices.end (); ++i)
    {
      Ptr<TictocNetDevice> tmp = *i;
      if (tmp == sender)
        {
          continue;
        }
      Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (1),
                                      &TictocNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
    }
}

} // namespace ns3
