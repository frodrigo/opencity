/***************************************************************************
                        CEnum.cpp  -  description
							-------------------
	begin                : January 2nd, 2009
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

// Framework headers
#include "CEnum.h"			// System::Enum class
#include "CString.h"		// System::String class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Enum::Enum() {}


Enum::~Enum() {}


   /*=====================================================================*/
String Enum::ToString() const
{
	return String("System::Enum");
}


SPF_NAMESPACE_END
