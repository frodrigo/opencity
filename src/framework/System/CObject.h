/***************************************************************************
                        CObject.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_COBJECT_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_COBJECT_H_ 1

// Sharp Plus Framework definitions
#include "SharpPlus.h"


SPF_NAMESPACE_BEGIN(System)

// Framework System::String class forward declaration
class String;

// Framework System::Type class forward declaration
class Type;

// Framework System::NullValue class forward declaration
class NullValue;


/**
	Supports all classes in the Sharp Plus Framework class hierarchy
and provides low-level services to derived classes.
*/
class Object
{
	public:
		Object();
		Object(const NullValue& null);
		virtual ~Object();

		bool IsNull() const;
		virtual String ToString() const;
		virtual Type GetType() const;

		virtual Object& operator=(const NullValue& null);

	protected:
		// We use pointer here to avoid circular include problem
		NullValue* poNullValue;
}; // class Object

SPF_NAMESPACE_END

#endif
