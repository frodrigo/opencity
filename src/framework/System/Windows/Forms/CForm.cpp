/***************************************************************************
                        CForm.cpp  -  description
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

// Framework headers
#include "CForm.h"				// System::Windows::Forms::Form class
#include "System/CString.h"		// System::String class


SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Windows, Forms)


   /*=====================================================================*/
Form::Form() {}


Form::~Form() {}


String Form::ToString() const
{
	return String("System::Windows::Forms::Form");
}


   /*=====================================================================*/


SPF_NAMESPACE_NESTED_END
SPF_NAMESPACE_END
