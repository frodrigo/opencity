/***************************************************************************
                        CEnum.cpp  -  description
							-------------------
	begin                : January 2nd, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
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
#include "CEnum.h"			// System::Enum class
#include "CString.h"		// System::String class

//#include <iostream>			// Debugging

SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Enum::Enum():
miValue(0)
{}


Enum::Enum(const Enum& original):
miValue(original.miValue)
{}


Enum::Enum(int value):
miValue(value)
{}


Enum::~Enum() {}


   /*=====================================================================*/
bool Enum::Equals(const Enum& ref) const
{
	return (this->miValue == ref.miValue);
}


   /*=====================================================================*/
const int Enum::ToInt() const
{
	return miValue;
}


String Enum::ToString() const
{
	return String("System::Enum");
}


   /*=====================================================================*/
Enum& Enum::operator=(const Enum& ref)
{
	//std::cout << "= operator" << std::endl;

	this->miValue = ref.miValue;
	return *this;
}


   /*=====================================================================*/
bool Enum::operator==(const Enum& ref) const
{
	//std::cout << "== operator" << std::endl;

	return (bool)(this->miValue == ref.miValue);
}


bool Enum::operator==(const Object& obj) const
{
	//std::cout << "== operator obj" << std::endl;

	// FIXME: type checking
	Object& constlessObj = const_cast<Object&>(obj);
	const Enum& enumRef = dynamic_cast<Enum&>(constlessObj);

	return (bool)(*this == enumRef);
}


   /*=====================================================================*/



SPF_NAMESPACE_END
