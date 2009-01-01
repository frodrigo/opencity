/***************************************************************************
                        CException.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CEXCEPTION_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CEXCEPTION_H_ 1

#ifndef __EXCEPTIONS
	#error "C++ exception support required."
#endif

// Framework headers
#include "CObject.h"			// Object class
#include "CString.h"			// String class

namespace System
{
	class Exception : public Object
	{
		public:
			Exception();
			Exception(const String& message);
			virtual ~Exception();

			/**
				Gets a message that describes the current exception.
				\return The error message that explains the reason for the exception, or an empty string("").
			*/
			const String& GetMessage() const;


			virtual String ToString() const;

		private:
			String msMessage;
	}; // class System::String
} // namespace System

#endif
