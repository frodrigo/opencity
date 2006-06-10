/***************************************************************************
                          message.cpp  -  description
         $Id: message.cpp,v 1.4 2006/01/11 14:59:27 neoneurone Exp $
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


#include "message.h"
#include "agent.h"

#include <cassert>

#ifdef DEBUG
#  include <iostream>
#  define DBG(os, something) os << something
#else
#  define DBG(os, something)
#endif



   /*=====================================================================*/
Message::Message():
m_sender(NULL),
m_type(MSG_UNDEFINED)
{}


   /*=====================================================================*/
Message::Message(Message_t type, Agent *sender):
m_sender(sender),
m_type(type)
{}


   /*=====================================================================*/
Message::Message_t Message::getType() const
{
	return m_type;
}


   /*=====================================================================*/
Agent* Message::getSender() const
{
	return m_sender;
}


   /*=====================================================================*/
Message&
Message::setSender(Agent* sender)
{
	m_sender = sender;
    return *this;
}


   /*=====================================================================*/
unsigned int Message::size() const
{
	return arguments.size();
}


   /*=====================================================================*/
Message& Message::operator<< (int value)
{
	arguments.push_back(Any(value)); return *this;
}


   /*=====================================================================*/
Message& Message::operator<< (unsigned int value)
{
	arguments.push_back(Any(value)); return *this;
}


   /*=====================================================================*/
Message& Message::operator<< (double value)
{
	arguments.push_back(Any(value)); return *this;
}


   /*=====================================================================*/
Message& Message::operator<< (const std::string &value)
{
	arguments.push_back(Any(value)); return *this;
}


   /*=====================================================================*/
Any& Message::operator[] (unsigned int index)
{
	return arguments.at(index);
}


   /*=====================================================================*/
const Any& Message::operator[] (unsigned int index) const
{
	return arguments.at(index);
}


   /*=====================================================================*/
std::ostream& operator<< (std::ostream& os, const Message &msg)
{
	std::vector<Any>::const_iterator
		it=msg.arguments.begin(),
		end=msg.arguments.end();
	bool separator=false;
	os << "Message <type=" << msg.getType() << ", args={";
	for (; it != end; ++it)
	{
		if (separator)
			os << ", ";
		else
			separator = true;
		os << *it;
	}
	os << "}>";
	return os;
}


































