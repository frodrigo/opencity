/***************************************************************************
                        CString.cpp  -  description
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

// Do not include "extern const String Empty"
#define _SPF_SYSTEM_STRING_CPP_ 1

// Framework headers
#include "CNullValue.h"			// System::NullValue class
#include "CString.h"
#include "CConsole.h"

// Standard C++ headers
#include <sstream>				// stringstream

SPF_NAMESPACE_BEGIN(System)


const String String::Empty;		// Default String.Empty value


   /*=====================================================================*/
String::String() {}


String::String(const char* const& value) : msString(value) {}


String::String(const unsigned char* const& value) : msString((const char* const&)value) {}


String::String(std::string value) : msString(value) {}


String::~String() {}


   /*=====================================================================*/
int String::GetLength() const
{
	return msString.size();
}


String String::ToString() const
{
	return *this;
}


   /*=====================================================================*/
String::operator const char*() const
{
	return msString.c_str();
}


String& String::operator=(const char* const value)
{
	msString = value;
	return *this;
}


String& String::operator=(const NullValue& null)
{
	mbIsNull = null.IsNull();
	return *this;
}


const String String::operator+(const char* const value) const
{
	return String(msString + value);
}


const String String::operator+(const String& value) const
{
	return String(msString + value.msString);
}


bool String::operator==(const char* const value) const
{
	return (msString == value);
}


bool String::operator==(const String& value) const
{
	return (msString == value.msString);
}


String& String::operator<<(const unsigned char value)
{
	msString += value;
	return *this;
}


String& String::operator<<(const int value)
{
	std::stringstream loStream;
	loStream << value;

	msString += loStream.str();
	return *this;
}


String& String::operator<<(const char* const value)
{
	msString += value;
	return *this;
}


   /*=====================================================================*/
std::ostream& operator<<(std::ostream& os, const String& value)
{
	return (os << value.msString);
}


SPF_NAMESPACE_END
