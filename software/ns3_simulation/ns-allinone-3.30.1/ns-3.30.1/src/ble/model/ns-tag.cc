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

#include "ns3/ns-tag.h"

namespace ns3 {
 
 TypeId 
 NodeTag::GetTypeId (void)
 {
   static TypeId tid = TypeId ("ns3::NodeTag")
     .SetParent<Tag> ()
     .AddConstructor<NodeTag> ()
     .AddAttribute ("NodeID",
                    "The node ID of the sender",
                    EmptyAttributeValue (),
                    MakeUintegerAccessor (&NodeTag::GetNodeId),
                    MakeUintegerChecker<uint32_t> ())
   ;
   return tid;
 }
 TypeId 
 NodeTag::GetInstanceTypeId (void) const
 {
   return GetTypeId ();
 }
 uint32_t 
 NodeTag::GetSerializedSize (void) const
 {
   return 4;
 }
 void 
 NodeTag::Serialize (TagBuffer i) const
 {
   i.WriteU32 (m_node_id);
 }
 void 
 NodeTag::Deserialize (TagBuffer i)
 {
   m_node_id = i.ReadU32 ();
 }
 void 
 NodeTag::Print (std::ostream &os) const
 {
   os << "v=" << (uint32_t)m_node_id;
 }
 void 
 NodeTag::SetNodeId (uint32_t value)
 {
   m_node_id = value;
 }
 uint32_t 
 NodeTag::GetNodeId (void) const
 {
   return m_node_id;
 }

 } // namespace ns3