/***************************************************************************
						agent.cpp  -  description
						-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2006 by Duong-Khang NGUYEN
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

// Useful enumerations
#include "opencity_structure_type.h"

// MAS headers
#include "agent.h"
#include "kernel.h"
#include "environment.h"
#include "message.h"

// OpenCity headers
#include "structure.h"				///< OpenCity's specifics

// Standard headers
#include <cassert>


/*=====================================================================*/
Agent::Agent(Kernel& kernel, Environment &env, int x, int y, MAS_ROLE role):
m_kernel(kernel),
m_environment(env),
m_x(x),
m_y(y),
m_role(role),
m_enumGC( OC_EMPTY ),				// OpenCity's specifics
m_id(kernel.getAgentUniqId())
{
	MAS_DEBUG( "ctor " << *this );

	m_move_speed = 1;
	m_direction = (MAS_DIRECTION)randomInt(0, 3);
}


/*=====================================================================*/
Agent::~Agent()
{
	MAS_DEBUG( "dtor " << *this );
}


/*=====================================================================*/
MAS_ROLE Agent::getRole() const
{
	return m_role;
}


/*=====================================================================*/
void Agent::receiveMessage(const Message& msg)
{
	MAS_DEBUG( *this << " receive " << msg );

	m_messages.push_back(msg);
}


/*=====================================================================*/
void Agent::processMessage()
{
}


/*=====================================================================*/
void Agent::sendMessage(MAS_ROLE role, const Message& msg)
{
	m_kernel.sendMessage(role, msg);
}


/*=====================================================================*/
void Agent::sendMessageToAgent(AgentID_t agent, const Message& msg)
{
	m_kernel.sendMessageToAgent(agent, msg);
}


/*=====================================================================*/
Agent* Agent::lookForAgent(MAS_DIRECTION dir, unsigned long max_distance)
{
	int x = m_x, y = m_y;
	int dx=0, dy=0;
	switch (dir)
	{
		case NORTH: dy = 1; break;
		case SOUTH: dy = -1; break;
		case EAST:  dx = 1; break;
		case WEST:  dx = -1; break;
	}

	uint step;
	for (step=1; step<=max_distance; step++)
	{
		x += dx;
		y += dy;
		if (x < 0 || y < 0 || (int)m_environment.getWidth() <= x || (int)m_environment.getHeight() <= y)
		{
			break;
		}
		if (m_environment.hasAgentAt(this, x, y))
		{
			return m_environment.getAgentAt(x,y);
		}
	}
	return NULL; 
}


/*=====================================================================*/
bool Agent::randomMove(int turn_percent)
{
	bool ok = doRandomMove(turn_percent);
	if (ok)
	{
		ok = moveDirection();
// debug
//		std::cerr << "Agent " << getId() << ", move to direction " << m_direction
//			<< " at x: " << x << " / y: " << y << std::endl;
	}
	return ok;
}


/*=====================================================================*/
bool Agent::doRandomMove(int turn_percent)
{
	bool ok = canMove(m_direction);
	if (ok) {
		if (turn_percent < randomInt(0,99))
			return true;

		// Try left/right
		MAS_DIRECTION dir;
		unsigned char rotate = randomBool();
		if (rotate)
			dir = rotateLeft(m_direction);
		else
			dir = rotateRight(m_direction);
		if (canMove(dir))
		{
			m_direction = dir;
			return true;
		}
		
		// Try right/left (the opposite)
		if (rotate)
			dir = rotateRight(m_direction);
		else
			dir = rotateLeft(m_direction);
		if (canMove(dir))
			m_direction = dir;
		return true;
	}
	else {
		// Try left/right
		unsigned char rotate = randomBool();
		MAS_DIRECTION dir;
		if (rotate)
			dir = rotateLeft(m_direction);
		else
			dir = rotateRight(m_direction);
		if (canMove(dir))
		{
			m_direction = dir;
			return true;
		}

		// Try right/left (the opposite)
		if (rotate)
			dir = rotateRight(m_direction);
		else
			dir = rotateLeft(m_direction);
		if (canMove(dir))
		{
			m_direction = dir;
			return true;
		}

		// try turn backward
		if (rotate)
			m_direction = rotate180(m_direction);
		else
			m_direction = rotate180(m_direction);
		return canMove(m_direction);
	}
}


/*=====================================================================*/
void Agent::born()
{
	m_agent_state = AGENT_BORN;
	m_kernel.registerAgent(this);
	m_environment.registerAgent(this, m_x, m_y);
}


/*=====================================================================*/
void Agent::live()
{
}


/*=====================================================================*/
void Agent::die()
{
	m_agent_state = AGENT_DIE;
	m_kernel.unregisterAgent(this);
	m_environment.unregisterAgent(this);
}


/*=====================================================================*/
AgentID_t Agent::getId() const
{
	return m_id;
}


/*=====================================================================*/
unsigned int Agent::getX() const
{
	return m_x;
}


/*=====================================================================*/
unsigned int Agent::getY() const
{
	return m_y;
}


/*=====================================================================*/
void Agent::output (std::ostream& os) const
{
	os << "Agent " << m_id;
}


/*=====================================================================*/
std::ostream& operator<< (std::ostream& os, const Agent &agent)
{
	agent.output(os);
	return os;
}


/*=====================================================================*/
const OPENCITY_GRAPHIC_CODE
Agent::GetGraphicCode() const
{
	return m_enumGC;
}


/*=====================================================================*/
bool Agent::move(int x, int y)
{
	assert (0 <= x && 0 <= y && x < (int)m_environment.getWidth() && y < (int)m_environment.getHeight());
	if (m_environment.moveAgent(this, x, y))
	{
		m_x = static_cast<unsigned int>(x);
		m_y = static_cast<unsigned int>(y);
		return true;
	} else {
		return false;
	}
}


/*=====================================================================*/
bool
Agent::moveDirection()
{
	int x, y;
	return (canMove(m_direction, x, y) and move(x,y));
}


/*=====================================================================*/
bool
Agent::canMove(int x, int y) const
{
	if ( x < 0
		or y < 0
		or (int)m_environment.getWidth() <= x
		or(int)m_environment.getHeight() <= y )
		return false;

	Structure* building = m_environment.getBuildingXY(x, y);
	if (building == NULL)
		return false;

	bool ok = building->GetCode() == OC_STRUCTURE_ROAD;
	if (ok) {
		ok = !m_environment.hasAgentAt(this, x, y);
	}
	return ok;
}


/*=====================================================================*/
bool
Agent::canMove(MAS_DIRECTION dir) const
{
	int x, y;
	return canMove(dir, x, y);
}


/*=====================================================================*/
bool
Agent::canMove(MAS_DIRECTION dir, int &x, int &y) const
{
	x = static_cast<int>(m_x);
	y = static_cast<int>(m_y);
	switch (dir) {
		case NORTH: y += m_move_speed; break;
		case SOUTH: y -= m_move_speed; break;
		case EAST: x += m_move_speed; break;
		case WEST: x -= m_move_speed; break;
	}
	return canMove(x, y);
}


/*=====================================================================*/
void
Agent::SetGraphicCode( const OPENCITY_GRAPHIC_CODE gc )
{
	m_enumGC = gc;
}



























