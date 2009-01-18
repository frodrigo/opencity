/***************************************************************************
                        CString.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CSTRING_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CSTRING_H_ 1

// Framework headers
#include "CObject.h"

// Standard C++ headers
#include <string>
#include <ostream>

SPF_NAMESPACE_BEGIN(System)

/**
	Represents text string instance
*/
class String : public Object
{
	public:
		String();
		String(const char* const value);
		explicit String(std::string value);
		virtual ~String();

//		String[] Split(String[] separator, const int options = StringSplitOptions::None);

		/**
			Gets the number of characters in the current String object.
		*/
		int GetLength() const;


		virtual String ToString() const;

		operator const char*() const;
		String& operator=(const char* const value);
		String& operator+(const char* const value);
		String& operator+(const String& value);
		String operator+(const String& value) const;
		friend std::ostream& operator<<(std::ostream& os, const String& value);

	private:
		std::string msString;
}; // class System::String

SPF_NAMESPACE_END

#endif
