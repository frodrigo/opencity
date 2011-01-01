/***************************************************************************
                        AButtonBase.cpp  -  description
							-------------------
	begin                : January 1st, 2011
	copyright            : (C) 2011 by Duong Khang NGUYEN
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
#include "AButtonBase.h"		// System::Windows::Forms::ButtonBase class
#include "System/CString.h"		// System::String class


SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Windows, Forms)


   /*=====================================================================*/
ButtonBase::ButtonBase() {}


ButtonBase::~ButtonBase() {}


String ButtonBase::ToString() const
{
	return String("System::Windows::Forms::ButtonBase");
}


   /*=====================================================================*/


SPF_NAMESPACE_NESTED_END
SPF_NAMESPACE_END
