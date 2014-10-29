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

#include <ctime>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/tictoc-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"

#include "tictoc-apps.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("TictocSimulator");


/**********************************************************
                   Trace data
 ***********************************************************/

//----------------------------------------------
//-- main
//----------------------------------------------
int main (int argc, char *argv[]) {

  int nodeAmount = 2;

  //------------------------------------------------------------
  //-- Create nodes and network stacks
  //--------------------------------------------
  NS_LOG_INFO ("Creating nodes.");
  NodeContainer nodes;
  nodes.Create (nodeAmount);

  //------------------------------------------------------------
  //-- Create Net device
  //--------------------------------------------

  NS_LOG_INFO ("Create traffic source & sink.");
  Ptr<TictocNetDevice> netDevices [nodeAmount];
  Ptr<TictocChannel> channels [nodeAmount];
  NetDeviceContainer netDeviceContainer;
  
  for(int i = 0; i < nodeAmount; i++)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      netDevices [i] = CreateObject<TictocNetDevice>();
      channels   [i] = CreateObject<TictocChannel>();
      netDeviceContainer.Add(netDevices [i]);
      node->AddDevice (netDevices [i]);
      netDevices [i]->SetChannel (channels [i]);
      netDevices [i]->SetNode (node);
      netDevices [i]->SetAddress (Mac48Address::Allocate ());
    }
  for (int i = 0; i < nodeAmount; i++)
    {
      for(int j = 0; j < nodeAmount; j++){
        channels [i]->Add (netDevices [j]);
      }
    }
  InternetStackHelper internet;
  internet.Install (nodes);
  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("192.168.0.0", "255.255.255.0");
  ipAddrs.Assign (netDeviceContainer);

  //------------------------------------------------------------
  //-- Create Applications
  //--------------------------------------------
  NS_LOG_INFO ("Create traffic source & sink.");
  Ptr<TictocSender>   sender   = CreateObject<TictocSender>();
  sender  ->SetStartTime (Seconds (1));
  NodeList::GetNode (0)->AddApplication (sender);

  //------------------------------------------------------------
  //-- Run the simulation
  //--------------------------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  
  // end main
}
