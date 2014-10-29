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
#ifndef TICTOC_CHANNEL_H
#define TICTOC_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include <vector>

namespace ns3 {

class TictocNetDevice;

class TictocChannel : public Channel
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TictocChannel ();


  /**
   * A packet is sent by a net device.  A receive event will be 
   * scheduled for all net device connected to the channel other 
   * than the net device who sent the packet
   *
   * \param p packet to be sent
   * \param protocol protocol number
   * \param to address to send packet to
   * \param from address the packet is coming from
   * \param sender netdevice who sent the packet
   *
   */
  virtual void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
                     Ptr<TictocNetDevice> sender);

  /**
   * Attached a net device to the channel.
   *
   * \param device the device to attach to the channel
   */ 
  virtual void Add (Ptr<TictocNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;


private:
  std::vector<Ptr<TictocNetDevice> > m_devices; //!< devices connected by the channel
};

} // namespace ns3

#endif /* TICTOC_CHANNEL_H */
