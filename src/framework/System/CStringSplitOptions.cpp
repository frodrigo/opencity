/***************************************************************************
                  CStringSplitOptions.cpp  -  description
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
#include "CStringSplitOptions.h"		// System::StringSplitOptions class
#include "CString.h"					// System::String class


   /*=====================================================================*/
namespace System
{
	StringSplitOptions::StringSplitOptions() {}


	StringSplitOptions::~StringSplitOptions() {}


   /*=====================================================================*/
	String StringSplitOptions::ToString() const
	{
		return String("System::StringSplitOptions");
	}

} // namespace System
