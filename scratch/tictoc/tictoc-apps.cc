/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Authors: Joe Kopena <tjkopena@cs.drexel.edu>
 *
 * These applications are used in the WiFi Distance Test experiment,
 * described and implemented in test02.cc.  That file should be in the
 * same place as this file.  The applications have two very simple
 * jobs, they just generate and receive packets.  We could use the
 * standard Application classes included in the NS-3 distribution.
 * These have been written just to change the behavior a little, and
 * provide more examples.
 *
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

#include <ostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/stats-module.h"

#include "tictoc-apps.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TictocApps");

/*
  TictocSender
*/
TypeId
TictocSender::GetTypeId (void)
{
  static TypeId tid = TypeId ("TictocSender")
    .SetParent<Application> ()
    .AddConstructor<TictocSender> ()
    .AddAttribute ("Destination", "Target host address.",
                   Ipv4AddressValue ("192.168.0.2"),
                   MakeIpv4AddressAccessor (&TictocSender::m_destAddr),
                   MakeIpv4AddressChecker ())
    .AddAttribute ("Port", "Destination app port.",
                   UintegerValue (1603),
                   MakeUintegerAccessor (&TictocSender::m_destPort),
                   MakeUintegerChecker<uint32_t>())
  ;
  return tid;
}


TictocSender::TictocSender()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
}

TictocSender::~TictocSender()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TictocSender::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_socket = 0;
  // chain up
  Application::DoDispose ();
}

void TictocSender::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_socket == 0) {
      Ptr<SocketFactory> socketFactory = GetNode ()->GetObject<SocketFactory>
          (UdpSocketFactory::GetTypeId ());
      m_socket = socketFactory->CreateSocket ();
      m_socket->Bind ();
    }
  Ptr<Packet> packet = Create<Packet>(100);
  m_socket->SendTo (packet, 0, InetSocketAddress (m_destAddr, m_destPort));
}

void
TictocSender::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
}

/*
  TictocReceiver
*/
TypeId
TictocReceiver::GetTypeId (void)
{
  static TypeId tid = TypeId ("TictocReceiver")
    .SetParent<Application> ()
    .AddConstructor<TictocReceiver> ()
    .AddAttribute ("Port", "Listening port.",
                   UintegerValue (1603),
                   MakeUintegerAccessor (&TictocReceiver::m_port),
                   MakeUintegerChecker<uint32_t>())
    ;
  return tid;
}

TictocReceiver::TictocReceiver()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
}

TictocReceiver::~TictocReceiver()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TictocReceiver::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
  Application::DoDispose ();
}

void
TictocReceiver::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_socket == 0) {
    Ptr<SocketFactory> socketFactory = GetNode ()->GetObject<SocketFactory>
      (UdpSocketFactory::GetTypeId ());
    m_socket = socketFactory->CreateSocket ();
    InetSocketAddress local = 
      InetSocketAddress (Ipv4Address::GetAny (), m_port);
    m_socket->Bind (local);
  }
  //end Receiver::StartApplication
}

void
TictocReceiver::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
  // end Receiver::StopApplication
}
