/***************************************************************************
                        CType.cpp  -  description
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
#include "CType.h"			// System::Type class

// GCC headers				// GCC demangle functionality
#include "cxxabi.h"

// Standard C++ headers
#include <cstdlib>			// free()


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Type::Type() {}


Type::Type(const NullValue& null): Reflection::MemberInfo(null) {}


Type::Type(const std::type_info& typeInfo)
{
	int iStatus;
	char* sRealName = abi::__cxa_demangle(typeInfo.name(), 0, 0, &iStatus);

	msName = sRealName;
	msFullName = sRealName;
	free(sRealName);
}


Type::~Type() {}


   /*=====================================================================*/
const String& Type::GetNamespace() const
{
	return msNamespace;
}


   /*=====================================================================*/
const String& Type::GetFullName() const
{
	return msFullName;
}


   /*=====================================================================*/
String Type::ToString() const
{
	return String("System::Type");
}


   /*=====================================================================*/
Type& Type::operator=(const Type& type)
{
	msName = type.msName;
	msNamespace = type.msNamespace;
	msFullName = type.msFullName;

	return *this;
}


bool Type::operator==(const Type& type) const
{
	return (msFullName == type.msFullName);
}


SPF_NAMESPACE_END
