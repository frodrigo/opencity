/***************************************************************************
                        CIntPtr.cpp  -  description
							-------------------
	begin                : January 23th, 2010
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
#include "CIntPtr.h"			// System::IntPtr class
#include "CString.h"			// System::String class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
IntPtr::IntPtr(void* value) : mpValue(value) {}


IntPtr::~IntPtr() {}


   /*=====================================================================*/
void* IntPtr::ToPointer() const
{
	return mpValue;
}


   /*=====================================================================*/
String IntPtr::ToString() const
{
	return String("System::IntPtr");
}


SPF_NAMESPACE_END
