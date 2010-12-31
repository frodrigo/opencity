/***************************************************************************
                        CForm.h  -  description
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

#ifndef _SPF_SYSTEM_WINDOWS_FORMS_CFORM_H_
#define _SPF_SYSTEM_WINDOWS_FORMS_CFORM_H_ 1

// Framework headers
#include "CContainerControl.h"

SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Windows, Forms)

/**
	Provides focus-management functionality for controls that can function
as a container for other controls.
*/
class Form : public ContainerControl
{
	public:
		Form();
		virtual ~Form();

		virtual String ToString() const;

}; // class System::Windows::Forms::Form

// namespace Windows::Forms
SPF_NAMESPACE_NESTED_END

// namespace System
SPF_NAMESPACE_END

#endif
