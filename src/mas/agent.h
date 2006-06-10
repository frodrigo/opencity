/***************************************************************************
                          agent.h  -  description
         $Id: agent.h,v 1.9 2006/06/05 09:58:06 neoneurone Exp $
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


#ifndef _OPENCITY_AGENT_H_
#define _OPENCITY_AGENT_H_ 1

#include "role.h"
#include "main.h"				///< OpenCity specifics

#include <ostream>
#include <list>

// Forward declaration
class Kernel;
class Environment;
class Message;

typedef unsigned int AgentID_t;


   /*=====================================================================*/
/** This interface shows common methods of all agents 
in a Multi-Agent System
	@author Victor STINNER
*/
class Agent
{
public:
    typedef enum
    {
		AGENT_BORN,
		AGENT_LIVE,
		AGENT_DIE
    } agent_state_t;

	Agent(Kernel& kernel, Environment& env, int x, int y, Role_t role);
	virtual ~Agent();

	void receiveMessage(const Message& msg);
	void sendMessage(Role_t role, const Message& msg);
	void sendMessageToAgent(AgentID_t agent, const Message& msg);
	virtual void processMessage();
	
	virtual void born();
	virtual void live();
	virtual void die();

    unsigned int getX() const;
    unsigned int getY() const;

    Agent* lookForAgent(direction_t dir, unsigned long max_distance);
	
	AgentID_t getId() const;
    Role_t getRole() const;

//========================================================================
/** Move the agent accordingly to the current direction
	@return true if the agent has move, false otherwise
*/
	bool moveDirection();
	bool move(int x, int y);

	bool canMove(int x, int y) const;
	bool canMove(direction_t dir, int &x, int &y) const;
	bool canMove(direction_t dir) const;

//========================================================================
/** Move the agent randomly
	@return true if the agent has move, false otherwise
*/
    bool randomMove(int turn_percent=60);

	virtual void output(std::ostream& os) const;
	friend std::ostream& operator<<(std::ostream& os, const Agent& agent);


   /*=====================================================================*/
   /*                          OPENCITY  SPECIFIC                         */
   /*=====================================================================*/
//========================================================================
/** Returns the OpenCity's GRAPHIC_CODE of the agent
*/
	const OPENCITY_GRAPHIC_CODE
	GetGraphicCode() const;

	void
	SetGraphicCode( const OPENCITY_GRAPHIC_CODE gc );


protected:
    bool doRandomMove(int turn_percent);
	Kernel& m_kernel;
	Environment& m_environment;
    int m_x, m_y;
    direction_t m_direction;
    unsigned int m_move_speed;
    Role_t m_role;
	agent_state_t m_agent_state;
	std::list<Message> m_messages;

   /*=====================================================================*/
   /*                          OPENCITY  SPECIFIC                         */
   /*=====================================================================*/
	OPENCITY_GRAPHIC_CODE m_enumGC;		///< OpenCity's graphic code

private:
	AgentID_t m_id;
};
#endif

































