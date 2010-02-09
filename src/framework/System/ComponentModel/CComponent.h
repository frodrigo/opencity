/***************************************************************************
                        CComponent.h  -  description
							-------------------
	begin                : February 9th, 2010
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

#ifndef _SPF_SYSTEM_COMPONENTMODEL_CCOMPONENT_H_
#define _SPF_SYSTEM_COMPONENTMODEL_CCOMPONENT_H_ 1

// Framework headers
#include "System/AMarshalByRefObject.h"		// System::MarshalByRefObject abstract class
#include "IComponent.h"						// System::IComponent interface

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_BEGIN(ComponentModel)


/**
	Provides the base implementation for the IComponent interface and enables
object sharing between applications.
*/
class Component : public MarshalByRefObject, public IComponent
{
	public:
		virtual String ToString() const;

	protected:
		Component();
		virtual ~Component();
}; // class System::ComponentModel::Component


// namespace System::ComponentModel
SPF_NAMESPACE_END

// namespace System
SPF_NAMESPACE_END

#endif
