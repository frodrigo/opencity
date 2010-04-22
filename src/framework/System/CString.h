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

#ifndef _SPF_SYSTEM_CSTRING_H_
#define _SPF_SYSTEM_CSTRING_H_ 1

// Framework headers
#include "CObject.h"

// Standard C++ headers
#include <string>
#include <ostream>

SPF_NAMESPACE_BEGIN(System)


/**
	Represents text string instance.
*/
class String : public Object
{
	public:
// Constructors and destructor
		String();
		String(const char* const& value);
		String(const unsigned char* const& value);
		explicit String(std::string value);
		virtual ~String();

//		String[] Split(String[] separator, const int options = StringSplitOptions::None);

// Methods
		virtual String ToString() const;

// Properties
/**
	Gets the number of characters in the current String object.
*/
		int GetLength() const;

// Operators
		operator const char*() const;
		String& operator=(const char* const value);
		String& operator=(const NullValue& null);

		const String operator+(const char* const value) const;
		const String operator+(const String& value) const;

		String& operator+=(const String& value);

		bool operator==(const char* const value) const;
		bool operator==(const String& value) const;

		String& operator<<(const unsigned char value);
		String& operator<<(const int value);
		String& operator<<(const char* const value);

		friend std::ostream& operator<<(std::ostream& os, const String& value);

		static const String Empty;


	private:
		std::string msString;

}; // class System::String


SPF_NAMESPACE_END

#endif
