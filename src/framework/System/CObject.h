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

#ifndef _SPF_SYSTEM_COBJECT_H_
#define _SPF_SYSTEM_COBJECT_H_ 1

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

/**
	Determines whether the specified Object is equal to the current Object.

	\param	obj	The Object to compare with the current Object.
	\return true if the specified Object is equal to the current Object;
otherwise, false.
*/
		virtual bool Equals(const Object& obj) const;

		virtual String ToString() const;
		virtual Type GetType() const;

		virtual Object& operator=(const NullValue& null);
		virtual bool operator==(const Object& obj) const;

	protected:
		bool mbIsNull;		/// Avoids circular dependency

}; // class Object

// Defines the member pointer types. They are used to build the delegate system.
typedef void	(Object::* MemberPointer0)();
typedef void	(Object::* MemberPointer1)(Object);
typedef Object	(Object::* ObjectMemberPointer0)();
typedef Object	(Object::* ObjectMemberPointer1)(Object);


SPF_NAMESPACE_END

#endif
