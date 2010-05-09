/***************************************************************************
                          any.cpp  -  description
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


#include "any.h"
#include "agent.h"				///< For debug macros

#include <cassert>
#include <iostream>


   /*=====================================================================*/
Any::Any( int value ):
_type(ANY_INT),
_intValue(value)
{}


   /*=====================================================================*/
Any::Any( unsigned int value ):
_type(ANY_UINT),
_uintValue(value)
{}


   /*=====================================================================*/
Any::Any( double value ):
_type(ANY_DOUBLE),
_doubleValue(value)
{}


   /*=====================================================================*/
Any::Any( const std::string& value ):
_type(ANY_STRING),
_stringValue(value)
{}


   /*=====================================================================*/
MAS_ANY_TYPE
Any::getType() const
{
	return _type;
}


   /*=====================================================================*/
int
Any::getInt() const
{
	assert (_type == ANY_INT);
	return _intValue;
}


   /*=====================================================================*/
unsigned int
Any::getUInt() const
{
	assert (_type == ANY_UINT);
	return _uintValue;
}


   /*=====================================================================*/
double
Any::getDouble() const
{
	assert (_type == ANY_DOUBLE);
	return _doubleValue;
}


   /*=====================================================================*/
const std::string&
Any::getString() const
{
	assert (_type == ANY_STRING);
	return _stringValue;
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
