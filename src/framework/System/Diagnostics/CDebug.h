/***************************************************************************
                        CDebug.h  -  description
							-------------------
	begin                : December 31th, 2008
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_DIAGNOSTICS_CDEBUG_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_DIAGNOSTICS_CDEBUG_H_ 1

// Framework headers
#include "../CObject.h"			// Object class

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_BEGIN(Diagnostics)

/**
	Provides a set of methods and properties that help debug your code
built with the Sharp Plus Framework.
*/
class Debug : public Object
{
	public:
		Debug();
		virtual ~Debug();

		static void Assert(const bool condition);

		/**
			Checks for a condition and displays a message if the condition is false.
		*/
		static void Assert(const bool condition, const String& message);

		virtual String ToString() const;
}; // class System::Diagnostics::Debug

// namespace System::Diagnostics
SPF_NAMESPACE_END

// namespace System
SPF_NAMESPACE_END

#endif
