/***************************************************************************
                    AMulticastDelegate.h  -  description
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

#ifndef _SPF_SYSTEM_AMULTICASTDELEGATE_H_
#define _SPF_SYSTEM_AMULTICASTDELEGATE_H_ 1

// Framework headers
#include "ADelegate.h"		// System::Delegate class

SPF_NAMESPACE_BEGIN(System)


/**
	A platform-specific type that is used to represent a pointer or a handle.
*/
class MulticastDelegate : public Delegate
{
	public:
/**
	Converts the numeric value of the current MulticastDelegate object to its equivalent
string representation.
*/
		virtual String ToString() const;

	protected:

// Constructors and destructor
		MulticastDelegate();
		virtual ~MulticastDelegate();

}; // class System::MulticastDelegate

SPF_NAMESPACE_END

#endif
