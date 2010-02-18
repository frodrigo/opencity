/***************************************************************************
                        ADelegate.h  -  description
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

#ifndef _SPF_SYSTEM_ADELEGATE_H_
#define _SPF_SYSTEM_ADELEGATE_H_ 1

// Framework headers
#include "CObject.h"		// System::Object class

SPF_NAMESPACE_BEGIN(System)

// Framework System::IntPtr class forward declaration
class IntPtr;


/**
	A platform-specific type that is used to represent a pointer or a handle.
*/
class Delegate : public Object
{
	public:

// Constructors and destructor
		Delegate();
		Delegate(const Object& target, const MemberPointer0 pointer);
		virtual ~Delegate();

/**
	Dynamically invokes (late-bound) the method represented by the current
delegate.
*/
		void DynamicInvoke() const;

/**
	Converts the numeric value of the current Delegate object to its equivalent
string representation.
*/
		virtual String ToString() const;


	protected:

/**
	Dynamically invokes (late-bound) the method represented by the current
delegate.
*/
		virtual void DynamicInvokeImpl() const;

	private:
		const Object* mpTarget;
		MemberPointer0 mpTargetMethod;


}; // class System::Delegate

SPF_NAMESPACE_END

#endif
