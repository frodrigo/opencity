/***************************************************************************
                  CNotSupportedException.h  -  description
							-------------------
	begin                : February 16th, 2010
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

#ifndef _SPF_SYSTEM_CNOTSUPPORTEDEXCEPTION_H_
#define _SPF_SYSTEM_CNOTSUPPORTEDEXCEPTION_H_ 1

// Framework headers
#include "CSystemException.h"			// System::SystemException class


SPF_NAMESPACE_BEGIN(System)


/**
	The exception that is thrown when an invoked method is not supported, or
when there is an attempt to read, seek, or write to a stream that does not
support the invoked functionality.
*/
class NotSupportedException : public SystemException
{
	public:
/**
	Initializes a new instance of the NotSupportedException class with
default properties.
*/
		NotSupportedException();
		NotSupportedException(const String& message);
		virtual ~NotSupportedException();

}; // class System::NotSupportedException

SPF_NAMESPACE_END

#endif
