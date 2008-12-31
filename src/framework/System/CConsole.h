/***************************************************************************
                        CConsole.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CCONSOLE_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CCONSOLE_H_ 1

// Framework headers
#include "CObject.h"			// Object class

namespace System
{
	// Forward System::String class declaration
	class String;

	class Console : Object
	{
		public:
			Console();
			virtual ~Console();

			/**
				Writes the text representation of the specified object to the standard output stream.
				\param value The Object value to write.
				\return Nothing.
			*/
			static void Write(const Object& value);

			/**
				Writes the specified string value to the standard output stream.
				\param value The String value to write.
				\return Nothing.
			*/
			static void Write(const String& value);


			virtual String ToString() const;

			/**
				Writes the text representation of the specified object to the standard output stream.
				\param value The Object value to write.
				\return The modified Console reference.
			*/
			Console& operator<<(const Object& value);

			/**
				Writes the character string to the standard output stream.
				\param s The character string to write.
				\return The modified Console reference.
			*/
			Console& operator<<(char const* s);
	}; // class System::Console

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CCONSOLE_CPP_
	extern Console Terminal;
#endif
} // namespace System

#endif
