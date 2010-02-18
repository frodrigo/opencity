/***************************************************************************
                        ADelegate.cpp  -  description
							-------------------
	begin                : February 3rd, 2010
	copyright            : (C) 2010 by Duong Khang NGUYEN
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
#include "ADelegate.h"			// System::Delegate class
//#include "CString.h"			// System::String class
#include "CIntPtr.h"			// System::IntPtr
//#include "CNullValue.h"			// System::NullValue class

#include "CNotImplementedException.h"	// Testing

// Standard C++ headers
#include <functional>			// mem_fun, mem_fun_t


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Delegate::Delegate() :
mpTarget(NULL),
mpTargetMethod(NULL)
{
}


Delegate::Delegate(const Object& target, const MemberPointer0 pointer) :
mpTarget(&target),
mpTargetMethod(pointer)
{
}


Delegate::~Delegate() {}


   /*=====================================================================*/
void Delegate::DynamicInvoke() const
{
	return this->DynamicInvokeImpl();
}


   /*=====================================================================*/
String Delegate::ToString() const
{
	return String("System::Delegate");
}


   /*=====================================================================*/
void Delegate::DynamicInvokeImpl() const
{
	(const_cast<Object*>(mpTarget)->*mpTargetMethod)();
}


   /*=====================================================================*/


SPF_NAMESPACE_END
