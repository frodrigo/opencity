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

/**
	Dynamically invokes (late-bound) the method represented by the current
delegate.
	\param	intPtr	A pointer to the object that is the argument to pass to the
method represented by the current delegate.
*/
		Object DynamicInvoke(const IntPtr& intPtr) const;


/**
	Converts the numeric value of the current Delegate object to its equivalent
string representation.
*/
		virtual String ToString() const;

	protected:

// Constructors and destructor
		Delegate();
		virtual ~Delegate();

/**
	Dynamically invokes (late-bound) the method represented by the current
delegate.
	\param	intPtr	A pointer to the object that is the argument to pass to the
method represented by the current delegate.
*/
		virtual Object DynamicInvokeImpl(const IntPtr& intPtr) const;


}; // class System::Delegate

SPF_NAMESPACE_END

#endif
