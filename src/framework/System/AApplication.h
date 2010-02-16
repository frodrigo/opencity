/***************************************************************************
                       AApplication.h  -  description
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

#ifndef _SPF_SYSTEM_AAPPLICATION_H_
#define _SPF_SYSTEM_AAPPLICATION_H_ 1

// Framework headers
#include "CObject.h"			// System::Object class

SPF_NAMESPACE_BEGIN(System)


/**
	Provides static methods and properties to manage an application, such
as methods to start and stop an application, to process messages, and
properties to get information about an application.
*/
class Application : public Object
{
	public:
		virtual String ToString() const;

	protected:
		Application();
		virtual ~Application();
}; // class System::Application

SPF_NAMESPACE_END

#endif
