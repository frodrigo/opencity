/***************************************************************************
                        CIntPtr.h  -  description
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

#ifndef _SPF_SYSTEM_CINTPTR_H_
#define _SPF_SYSTEM_CINTPTR_H_ 1

// Framework headers
#include "AValueType.h"		// System::ValueType class

SPF_NAMESPACE_BEGIN(System)


/**
	A platform-specific type that is used to represent a pointer or a handle.
*/
class IntPtr : public ValueType
{
	public:

// Constructors and destructor
/**
	Initializes a new instance of IntPtr using the specified pointer to an
unspecified type.
	\param	value	A pointer to an unspecified type.
*/
		IntPtr(void* value);
		virtual ~IntPtr();

/**
	Converts the value of this instance to a pointer to an unspecified type.
	\return	A pointer to Void; that is, a pointer to memory containing data of
an unspecified type.
*/
		void* const ToPointer() const;

/**
	Converts the numeric value of the current IntPtr object to its equivalent
string representation.
*/
		virtual String ToString() const;

	private:
		void*	mpValue;
}; // class System::IntPtr

SPF_NAMESPACE_END

#endif
