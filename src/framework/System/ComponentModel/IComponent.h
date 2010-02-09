/***************************************************************************
                        IComponent.h  -  description
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

#ifndef _SPF_SYSTEM_COMPONENTMODEL_ICOMPONENT_H_
#define _SPF_SYSTEM_COMPONENTMODEL_ICOMPONENT_H_ 1

// Framework headers
#include "System/IDisposable.h"		// System::IDisposable interface

SPF_NAMESPACE_NESTED_BEGIN(System, ComponentModel)


/**
	Provides functionality required by all components.
*/
class IComponent : public System::IDisposable
{
	protected:
		IComponent() {}
		virtual ~IComponent() {}

}; // class System::ComponentModel::IComponent

SPF_NAMESPACE_NESTED_END

#endif
