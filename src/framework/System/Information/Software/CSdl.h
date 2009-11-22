/***************************************************************************
                        CSdl.h  -  description
							-------------------
	begin                : November 1st, 2009
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

#ifndef _SPF_SYSTEM_INFORMATION_SOFTWARE_CSDL_H_
#define _SPF_SYSTEM_INFORMATION_SOFTWARE_CSDL_H_ 1

// Framework headers
#include "System/CObject.h"

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_NESTED_BEGIN(Information, Software)

/**
	Provides the information about the linked SDL library.
*/
class Sdl : public Object
{
	public:
		Sdl();
		virtual ~Sdl();

		virtual String ToString() const;

		/**
			Gets the SDL library's compile-time version.
		*/
		static const String GetCompiletimeVersion();

		/**
			Gets the SDL library's run-time version.
		*/
		static const String GetRuntimeVersion();


}; // class System::Sdl

SPF_NAMESPACE_NESTED_END
SPF_NAMESPACE_END

#endif
