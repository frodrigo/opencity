/***************************************************************************
                  CNotImplementedException.h  -  description
							-------------------
	begin                : February 7th, 2010
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

#ifndef _SPF_SYSTEM_CNOTIMPLEMENTEDEXCEPTION_H_
#define _SPF_SYSTEM_CNOTIMPLEMENTEDEXCEPTION_H_ 1

// Framework headers
#include "CException.h"			// System::Exception class


SPF_NAMESPACE_BEGIN(System)


/**
	The exception that is thrown when a requested method or operation is not
implemented.
*/
class NotImplementedException : public Exception
{
	public:
/**
	Initializes a new instance of the NotImplementedException class with
default properties.
*/
		NotImplementedException();
		NotImplementedException(const String& message);
		virtual ~NotImplementedException();

}; // class System::NotImplementedException

SPF_NAMESPACE_END

#endif
