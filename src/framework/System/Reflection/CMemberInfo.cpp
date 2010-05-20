/***************************************************************************
                        CMemberInfo.cpp  -  description
							-------------------
	begin                : January 1st, 2009
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
#include "CMemberInfo.h"


SPF_NAMESPACE_NESTED_BEGIN(System, Reflection)


   /*=====================================================================*/
MemberInfo::MemberInfo() {}


MemberInfo::MemberInfo(const NullValue& null): System::Object(null) {}


MemberInfo::MemberInfo(const String& name) : msName(name) {}


MemberInfo::~MemberInfo() {}


   /*=====================================================================*/
const String& MemberInfo::GetName() const
{
	return msName;
}


   /*=====================================================================*/
String MemberInfo::ToString() const
{
	return String("System::Reflection::MemberInfo");
}


SPF_NAMESPACE_NESTED_END
