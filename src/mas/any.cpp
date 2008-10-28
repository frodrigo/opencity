/***************************************************************************
						any.cpp  -  description
							-------------------
	begin                : nov 29th, 2005
	copyright            : (C) 2005-2008 by Duong Khang NGUYEN
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


#include "any.h"
#include "agent.h"				///< For debug macros

#include <cassert>
#include <iostream>


   /*=====================================================================*/
Any::Any( int value ):
m_type(ANY_INT),
m_int_value(value)
{}


   /*=====================================================================*/
Any::Any( unsigned int value ):
m_type(ANY_UINT),
m_uint_value(value)
{}


   /*=====================================================================*/
Any::Any( double value ):
m_type(ANY_DOUBLE),
m_double_value(value)
{}


   /*=====================================================================*/
Any::Any( const std::string& value ):
m_type(ANY_STRING),
m_string_value(value)
{}


   /*=====================================================================*/
MAS_ANY_TYPE
Any::getType() const
{
	return m_type;
}


   /*=====================================================================*/
int
Any::getInt() const
{
	assert (m_type == ANY_INT);
	return m_int_value;
}


   /*=====================================================================*/
unsigned int
Any::getUInt() const
{
	assert (m_type == ANY_UINT);
	return m_uint_value;
}


   /*=====================================================================*/
double
Any::getDouble() const
{
	assert (m_type == ANY_DOUBLE);
	return m_double_value;
}


   /*=====================================================================*/
const std::string&
Any::getString() const
{
	assert (m_type == ANY_STRING);
	return m_string_value;
}


   /*=====================================================================*/
std::ostream&
operator<< (std::ostream &os, const Any &any)
{
	switch (any.getType())
	{
		case ANY_INT:
			MAS_DEBUG( "(int)" );
			os << any.getInt();
			break;
		case ANY_UINT:
			MAS_DEBUG( "(uint)" );
			os << any.getUInt();
			break;
		case ANY_DOUBLE:
			MAS_DEBUG( "(double)" );
			os << any.getDouble();
			break;
		case ANY_STRING:
			MAS_DEBUG( "(str)" );
			os << '"' << any.getString() << '"';
			break;
	}
	return os;
}
































