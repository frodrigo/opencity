/***************************************************************************
						kernel.cpp  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	author               : Duong-Khang NGUYEN and Victor STINNER

	$Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/


#include "kernel.h"
#include "agent.h"
#include "message.h"

#include <algorithm>
#include <cassert>
#include <iostream>


Kernel* Kernel::m_singleton = NULL;				///< Singleton


   /*=====================================================================*/
Kernel::Kernel():
m_step(0),
m_agent_uniq_id(1)
{
	m_singleton = this;
}

	
   /*=====================================================================*/
Kernel::~Kernel()
{
// FIXME: quick and dirty, the yellow page is not handled yet
	m_white_pages_it it=m_white_pages.begin(), end=m_white_pages.end();
	for (; it != end; ++it) {
		it->second->die();
		delete it->second;
	}
	m_white_pages.clear();
	m_singleton = NULL;
}


   /*=====================================================================*/
unsigned long Kernel::getStep() const
{
	return m_step;
}


   /*=====================================================================*/
void Kernel::live()
{
// debug
	++m_step;
/*	if (!(++m_step % 10))
		std::cout << "Kernel is at step: " << m_step << std::endl; */

// Die and Kill
	m_agent_dying_it a_it=m_agent_dying.begin(), a_end=m_agent_dying.end();
	for (; a_it != a_end; ++a_it) {
		MAS_DEBUG( "Agent: " << (*a_it)->getId() << " has been killed " );
		(*a_it)->die();
		delete *a_it;
	}
	if (!m_agent_dying.empty())
		m_agent_dying.clear();

// Live
	m_white_pages_it it=m_white_pages.begin(), end=m_white_pages.end();
	for (; it != end; ++it)
		(*it->second).live();
}


   /*=====================================================================*/
Kernel*
Kernel::getSingleton()
{
	return m_singleton;
}


   /*=====================================================================*/
AgentID_t Kernel::getAgentUniqId()
{
	return m_agent_uniq_id++;
}


   /*=====================================================================*/
void Kernel::registerAgent(Agent *agent)
{
	m_white_pages[agent->getId()] = agent;
}


   /*=====================================================================*/
void
Kernel::unregisterAgent(Agent *agent)
{
	m_white_pages.erase(agent->getId());
}


   /*=====================================================================*/
void Kernel::registerRole(Agent* agent, Role_t role)
{
	m_yellow_pages[role].push_back(agent);
}


   /*=====================================================================*/
void
Kernel::unregisterRole(Agent* agent, Role_t role)
{
	m_agent_set_it iter = find( m_yellow_pages[role].begin(), m_yellow_pages[role].end(), agent);
	if (iter != m_yellow_pages[role].end())
		m_yellow_pages[role].erase(iter);
}


   /*=====================================================================*/
void
Kernel::killAgent(Agent* agent)
{
	Message msg = Message( Message::MSG_AGENT_DIE );

	this->sendMessageToAgent(agent->getId(), msg);
	m_agent_dying.push_back(agent);
}


   /*=====================================================================*/
void Kernel::sendMessage(Role_t role, const Message &msg)
{
	m_yellow_pages_it set_it=m_yellow_pages.find(role);
	if (set_it == m_yellow_pages.end()) return;
	m_agent_set_it
		it = (*set_it).second.begin(),
		end = (*set_it).second.end();
	for (; it != end; ++it)
	{
		(**it).receiveMessage(msg);
	}
}


   /*=====================================================================*/
Agent& Kernel::getAgentById(AgentID_t agent)
{
	assert (m_white_pages.find(agent) != m_white_pages.end());
	return *m_white_pages[agent];
}


   /*=====================================================================*/
void Kernel::sendMessageToAgent(AgentID_t agent, const Message &msg)
{
	getAgentById(agent).receiveMessage(msg);
}

































