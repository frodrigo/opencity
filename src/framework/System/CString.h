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

namespace System
{
	class String : Object
	{
		public:
			String();
			String(const char*);
			virtual ~String();


		private:
			std::string msString;
	}; // class System::String
} // namespace System

#endif
