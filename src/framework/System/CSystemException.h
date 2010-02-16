/***************************************************************************
                      CSystemException.h  -  description
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

#ifndef _SPF_SYSTEM_CSYSTEMEXCEPTION_H_
#define _SPF_SYSTEM_CSYSTEMEXCEPTION_H_ 1

// Framework headers
#include "CException.h"			// System::SystemException class

SPF_NAMESPACE_BEGIN(System)

/**
	Defines the base class for predefined exceptions in the System namespace.
*/
class SystemException : public Exception
{
	public:
		SystemException();
		SystemException(const String& message);
		virtual ~SystemException();

}; // class System::SystemException

SPF_NAMESPACE_END

#endif
