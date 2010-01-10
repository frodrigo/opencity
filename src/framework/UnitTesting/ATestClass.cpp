/***************************************************************************
                        CTestClass.cpp  -  description
							-------------------
	begin                : November 29th, 2009
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
#include "ATestClass.h"				// UnitTesting::TestClass abstract class
#include "System/CString.h"			// System::String class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestClass::TestClass() {}


TestClass::~TestClass() {}


   /*=====================================================================*/
System::String TestClass::ToString() const
{
	return System::String("UnitTesting::TestClass");
}


SPF_NAMESPACE_END
