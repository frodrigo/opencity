/***************************************************************************
                       CNullValue.cpp  -  description
							-------------------
	begin                : January 18th, 2009
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

// Do not include "extern NullValue Null"
#define _OPENCITY_FRAMEWORK_SYSTEM_CNULLVALUE_CPP_ 1

// Framework headers
#include "CNullValue.h"				// System::NullValue class
#include "CString.h"				// System::String class


SPF_NAMESPACE_BEGIN(System)


NullValue Null;


   /*=====================================================================*/
NullValue::NullValue()
{
	mbIsNull = true;
}


NullValue::NullValue(const NullValue& value)
{
	mbIsNull = value.mbIsNull;
}


NullValue::~NullValue() {}


   /*=====================================================================*/
bool NullValue::IsNull() const
{
	return mbIsNull;
}


String NullValue::ToString() const
{
	return String("System::NullValue");
}


   /*=====================================================================*/


SPF_NAMESPACE_END
