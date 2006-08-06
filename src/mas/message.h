/***************************************************************************
						message.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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


#ifndef _OPENCITY_MESSAGE_H_
#define _OPENCITY_MESSAGE_H_ 1

#include "any.h"

#include <vector>
#include <list>
#include <string>
#include <ostream>

// Forward declarations
class Agent;

   /*=====================================================================*/
/** The agents send "messages" to each other
	@author Victor STINNER
*/
class Message
{
public:
	typedef enum {
		MSG_UNDEFINED = 0,
		MSG_NEW_DEMONSTRATOR,
		MSG_NEW_ROBBER,
		MSG_KILL_AGENT,
		MSG_AGENT_DIE
	} Message_t;

	Message();
	Message(Message_t type, Agent *sender=NULL);
	Message& operator<< (int value);
	Message& operator<< (unsigned int value);
	Message& operator<< (double value);
	Message& operator<< (const std::string& value);
	
	Message_t getType() const;
	Agent* getSender() const;
	Message& setSender(Agent* sender);
	unsigned int size() const;
	Any& operator[] (unsigned int index);
	const Any& operator[] (unsigned int index) const;

	friend std::ostream& operator<<(std::ostream& os, const Message& msg);

private:
	Agent* m_sender;
	Message_t m_type;
	std::vector<Any> arguments;
};

#endif

































