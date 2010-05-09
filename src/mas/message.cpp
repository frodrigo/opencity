/***************************************************************************
                        message.cpp  -  description
							-------------------
	begin                : nov 29th, 2005
	copyright            : (C) 2005-2010 by Duong Khang NGUYEN
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


#include "message.h"
#include "agent.h"

#include <cassert>


   /*=====================================================================*/
Message::Message():
_sender(NULL),
_type(MSG_UNDEFINED)
{}


   /*=====================================================================*/
Message::Message( MAS_MESSAGE_TYPE type, Agent *sender ):
_sender(sender),
_type(type)
{}


   /*=====================================================================*/
MAS_MESSAGE_TYPE
Message::getType() const
{
	return _type;
}


   /*=====================================================================*/
Agent* Message::getSender() const
{
	return _sender;
}


   /*=====================================================================*/
Message&
Message::setSender(Agent* sender)
{
	_sender = sender;
    return *this;
}


   /*=====================================================================*/
unsigned int Message::size() const
{
	return _arguments.size();
}


   /*=====================================================================*/
Message&
Message::operator<< (int value)
{
	_arguments.push_back(Any(value));
	return *this;
}


   /*=====================================================================*/
Message&
Message::operator<< (unsigned int value)
{
	_arguments.push_back(Any(value));
	return *this;
}


   /*=====================================================================*/
Message&
Message::operator<< (double value)
{
	_arguments.push_back(Any(value));
	return *this;
}


   /*=====================================================================*/
Message&
Message::operator<< (const std::string &value)
{
	_arguments.push_back(Any(value));
	return *this;
}


   /*=====================================================================*/
Any&
Message::operator[] (unsigned int index)
{
	return _arguments.at(index);
}


   /*=====================================================================*/
const Any&
Message::operator[] (unsigned int index) const
{
	return _arguments.at(index);
}


   /*=====================================================================*/
std::ostream&
operator<< (std::ostream& os, const Message &msg)
{
	std::vector<Any>::const_iterator
		it = msg._arguments.begin(),
		end = msg._arguments.end();
	bool separator = false;

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
