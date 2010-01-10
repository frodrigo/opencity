/***************************************************************************
                        CTestContext.cpp  -  description
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
#include "CTestContext.h"			// UnitTesting::TestContext class
#include "System/CString.h"			// System::String class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestContext::TestContext() {}


TestContext::~TestContext() {}


   /*=====================================================================*/
System::String TestContext::ToString() const
{
	return System::String("UnitTesting::TestContext");
}


SPF_NAMESPACE_END
