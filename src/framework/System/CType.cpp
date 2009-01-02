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


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Type::Type() {}


Type::Type(const String& name) : System::Reflection::MemberInfo(name) {}


Type::Type(const String& name, const String& space) :
	System::Reflection::MemberInfo(name),
	msNamespace( space )
{
	msFullName = msNamespace + "::" + msName;
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


SPF_NAMESPACE_END
