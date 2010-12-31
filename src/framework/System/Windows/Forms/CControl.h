/***************************************************************************
                        CControl.h  -  description
							-------------------
	begin                : December 31th, 2010
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

#ifndef _SPF_SYSTEM_WINDOWS_FORMS_CCONTROL_H_
#define _SPF_SYSTEM_WINDOWS_FORMS_CCONTROL_H_ 1

// Framework headers
#include "System/ComponentModel/CComponent.h"

SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Windows, Forms)

/**
	Defines the base class for controls, which are components with
visual representation.
*/
class Control : public System::ComponentModel::Component
{
	public:
		Control();
		virtual ~Control();

		virtual String ToString() const;

}; // class System::Windows::Forms::Control

// namespace Windows::Forms
SPF_NAMESPACE_NESTED_END

// namespace System
SPF_NAMESPACE_END

#endif
