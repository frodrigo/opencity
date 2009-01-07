/***************************************************************************
                        CConsole.cpp  -  description
							-------------------
	begin                : December 31th, 2008
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

// Do not include "extern Console Terminal"
#define _OPENCITY_FRAMEWORK_SYSTEM_CCONSOLE_CPP_ 1

// Framework headers
#include "CConsole.h"			// System::Console class
#include "CString.h"			// System::String class

// Standard C++ headers
#include <iostream>				// cout, cerr, cin


SPF_NAMESPACE_BEGIN(System)


Console Terminal;


   /*=====================================================================*/
Console::Console() {}


Console::~Console() {}


   /*=====================================================================*/
void Console::Write(const Object& value)
{
	std::cout << value.ToString();
}


void Console::Write(const String& value)
{
	std::cout << value;
}


   /*=====================================================================*/
String Console::ToString() const
{
	return String("System::Console");
}


   /*=====================================================================*/
Console& Console::operator<<(const Object& value)
{
	std::cout << value.ToString();
	return *this;
}


   /*=====================================================================*/
Console& Console::operator<<(const char* const s)
{
	std::cout << s;
	return *this;
}


Console& Console::operator<<(const int i)
{
	std::cout << i;
	return *this;
}


SPF_NAMESPACE_END
