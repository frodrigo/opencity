/***************************************************************************
                        CBoolean.cpp  -  description
							-------------------
	begin                : December 29th, 2008
	copyright            : (C) 2008 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

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

// Framework headers
#include "CBoolean.h"			// System::Boolean class
#include "CString.h"			// System::String class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Boolean::Boolean() {}


Boolean::Boolean(bool value) : mbValue(value) {}


Boolean::~Boolean() {}


String Boolean::ToString() const
{
	return mbValue ? String("true") : String("false");
}


   /*=====================================================================*/
Boolean& Boolean::operator=(const bool& value)
{
	this->mbValue = value;
	return *this;
}


bool Boolean::operator==(const Boolean& value)
{
	return (bool)(this->mbValue == value.mbValue);
}


bool Boolean::operator!=(const Boolean& value)
{
	return (bool)(this->mbValue != value.mbValue);
}


   /*=====================================================================*/
bool Boolean::operator==(const bool& value)
{
	return (bool)(this->mbValue == value);
}


bool Boolean::operator!=(const bool& value)
{
	return (bool)(this->mbValue != value);
}


   /*=====================================================================*/


SPF_NAMESPACE_END
