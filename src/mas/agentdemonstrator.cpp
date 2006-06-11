/***************************************************************************
                    agentdemonstrator.cpp  -  description
         $Id$
                             -------------------
    begin                : nov 29th 2005
    copyright            : (C) 2005 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
    author               : Victor STINNER
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/


#include "agentdemonstrator.h"
#include "kernel.h"
#include "message.h"

#define MAX_TTL 15

   /*=====================================================================*/
AgentDemonstrator::AgentDemonstrator(Kernel& kernel, Environment& env, int x, int y):
Agent(kernel, env, x, y, ROLE_DEMONSTRATOR)
{
	this->born();		// Warning: see superclass

// OpenCity's specifics
	this->SetGraphicCode( OC_VEHICLE_PICKUP );
}


   /*=====================================================================*/
AgentDemonstrator::~AgentDemonstrator()
{
	if (m_agent_state != AGENT_DIE)
		this->die();		// Warning: see superclass
}


   /*=====================================================================*/
void AgentDemonstrator::processMessage()
{
	static Message msg;

// Process all the messages
	while (!m_messages.empty()) {
		msg = m_messages.front();
#ifndef OC_MAS_NDEBUG
	std::cout << *this << " processes " << msg << std::endl;
#endif
		if (msg.getType() == Message::MSG_AGENT_DIE)
			m_agent_state = AGENT_DIE;
		m_messages.pop_front();
	}
}


   /*=====================================================================*/
void AgentDemonstrator::born()
{
	m_ttl = MAX_TTL;			// Set m_ttl to maximum time to live
	processMessage();
	Agent::born();				// This set m_agent_state to AGENT_BORN
	m_kernel.registerRole(this, ROLE_DEMONSTRATOR);
	m_agent_state = AGENT_LIVE;
}


   /*=====================================================================*/
void AgentDemonstrator::live()
{
// Process all the received messages
	processMessage();

// Do we need this ?
//	Agent::live();

	if (m_agent_state != AGENT_LIVE)
		return;

    // Little sleep()
    unsigned int m_sleep = 3;
    if ((m_kernel.getStep() % m_sleep) != 0)
        return;

// IF we can not move in more than ttl steps THEN die
    if (!randomMove()) {
		m_ttl--;		// Decrease Time To Live
//debug cout << "Agent: " << getId() << " / TTL: " << m_ttl << endl;
		if (!m_ttl) {
			m_kernel.killAgent(this);
		}
	}
	else {
		m_ttl = MAX_TTL;
	}
}


   /*=====================================================================*/
void AgentDemonstrator::die()
{
	processMessage();
	Agent::die();
	m_kernel.unregisterRole(this, getRole());
}


   /*=====================================================================*/
void AgentDemonstrator::output (std::ostream& os) const
{
	os << "AgentDemonstrator " << getId();
}

































