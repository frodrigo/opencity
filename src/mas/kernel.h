/***************************************************************************
						kernel.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005 by Duong-Khang NGUYEN
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


#ifndef _OPENCITY_KERNEL_H_
#define _OPENCITY_KERNEL_H_ 1

#include "agent.h"

#include <map>
#include <list>

// Forward declaration
class Message;


   /*=====================================================================*/
/** This is the kernel of the Multi-Agent System.
	@author Victor STINNER
*/
class Kernel
{
public:
	Kernel();
	~Kernel();

	unsigned long getStep() const;

//========================================================================
/** The main method of the kernel. It must be called in order to execute
the agents and other things
*/
	void live();

	AgentID_t getAgentUniqId();

//========================================================================
/** Return the pointer to the singleton of the kernel.
	@return The pointer to the only one kernel of the application,
otherwise null
*/
	static Kernel* getSingleton();

	void registerAgent(Agent* agent);
	void registerRole(Agent *agent, Role_t role);
	void unregisterAgent(Agent* agent);
	void unregisterRole(Agent *agent, Role_t role);

//========================================================================
/** First, remove the agent either from the yellow or the white pages.
Then, physically destroy it.
	@param agent The pointer to the agent to destroy
*/
	void killAgent(Agent* agent);

	void sendMessage(Role_t role, const Message& msg);
	void sendMessageToAgent(AgentID_t agent, const Message& msg);

	
private:
	static Kernel* m_singleton;				///< Singleton

	unsigned long m_step;
	std::map<AgentID_t, Agent*> m_white_pages;
	typedef std::map<AgentID_t, Agent*>::iterator m_white_pages_it;
	std::map<Role_t, std::list<Agent*> > m_yellow_pages;
	typedef std::map<Role_t, std::list<Agent*> >::iterator m_yellow_pages_it;
	typedef std::list<Agent*>::iterator m_agent_set_it;
	std::list<Agent*> m_agent_dying;
	typedef std::list<Agent*>::iterator m_agent_dying_it;
	AgentID_t m_agent_uniq_id;


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
	Agent& getAgentById(AgentID_t agent);
};


#endif

































