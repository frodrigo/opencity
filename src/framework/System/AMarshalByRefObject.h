/***************************************************************************
                    AMarshalByRefObject.h  -  description
							-------------------
	begin                : January 11th, 2009
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

#ifndef _SPF_SYSTEM_AMARSHALBYREFOBJECT_H_
#define _SPF_SYSTEM_AMARSHALBYREFOBJECT_H_ 1

// Framework headers
#include "CObject.h"			// System::Object class


SPF_NAMESPACE_BEGIN(System)


/**
	Enables access to objects across application domain boundaries in
applications that support remoting.
*/
class MarshalByRefObject : public Object
{
	public:
		virtual String ToString() const;

	protected:
		MarshalByRefObject();
		virtual ~MarshalByRefObject();

}; // class System::MarshalByRefObject

SPF_NAMESPACE_END

#endif
