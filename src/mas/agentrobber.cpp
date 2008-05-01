/***************************************************************************
					agentrobber.cpp  -  description
						-------------------
	begin                : nov 29th, 2005
	copyright            : (C) 2005-2008 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	author               : Victor STINNER

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


#include "agentrobber.h"
#include "kernel.h"
#include "message.h"


   /*=====================================================================*/
AgentRobber::AgentRobber
(
	Kernel& kernel,
	Environment& env,
	int x, int y
):
Agent(kernel, env, x, y, ROLE_ROBBER)
{
	this->born();		// Warning: see superclass

// OpenCity's specifics
	this->SetGraphicCode( OC_VEHICLE_ROBBER );
}


   /*=====================================================================*/
AgentRobber::~AgentRobber()
{
	if (m_agent_state != AGENT_DIE)
		this->die();		// Warning: see superclass
}


   /*=====================================================================*/
void
AgentRobber::processMessage()
{
}


   /*=====================================================================*/
void
AgentRobber::born()
{
	Agent::born();
	m_kernel.registerRole(this, ROLE_ROBBER);
}


   /*=====================================================================*/
void
AgentRobber::live()
{
	static bool first = true;

	if (first) {
		sendMessage( ROLE_POLICE, Message( MSG_NEW_ROBBER ) << 10 << 5);
		first = false;
	}
}


   /*=====================================================================*/
void
AgentRobber::die()
{
	Agent::die();
	m_kernel.unregisterRole(this, this->getRole());
}


   /*=====================================================================*/
void
AgentRobber::output (std::ostream& os) const
{
	os << "AgentRobber " << getId();
}

































