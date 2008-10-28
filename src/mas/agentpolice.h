/***************************************************************************
						agentpolice.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
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

#ifndef _OPENCITY_AGENTPOLICE_H_
#define _OPENCITY_AGENTPOLICE_H_ 1

#include "agent.h"
#include <vector>

// Forward declarations
class Destination;

   /*=====================================================================*/
/** Specialized police agent
	@author Victor STINNER
*/
class AgentPolice : public Agent
{
public:
	typedef enum
	{
		POLICE_LOOK,
		POLICE_PURSUE,
		POLICE_LOST_PURSUE,
		POLICE_COLLABORATE
	} police_state_t;

	AgentPolice(Kernel& kernel, Environment& env, int x, int y);
	virtual ~AgentPolice();

	virtual void processMessage();
	virtual void born();
	virtual void live();
	virtual void die();
	virtual void output(std::ostream& os) const;

protected:
	police_state_t m_state;
	unsigned int m_sleep;
	unsigned int m_pursue;
	unsigned int m_lost_pursue;
	
	Agent* SeeBadGuy(MAS_DIRECTION dir);
	void Look();
	void Pursue();
	void Collaborate();
	void SetState(police_state_t state);
	void NoticePursue(Agent *agent);
	Agent* FindBadGuy(bool test_backward=true);
	void LostPursue();
	Agent* IsNearBadGuy();

// For collaboration and path finding
	unsigned int m_index;
	std::vector<Destination> m_path;
	typedef std::vector<Destination>::iterator m_path_it;

private:
   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
	void followPath();
};

#endif































