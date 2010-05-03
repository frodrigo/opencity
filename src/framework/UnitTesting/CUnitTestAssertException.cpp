/***************************************************************************
                        CUnitTestAssertException.cpp  -  description
							-------------------
	begin                : January 10th, 2010
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
#include "CUnitTestAssertException.h"		// UnitTestAssertException class

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
UnitTestAssertException::UnitTestAssertException() {}


UnitTestAssertException::UnitTestAssertException(const System::String& message) :
	Exception(message)
{}


UnitTestAssertException::~UnitTestAssertException() {}
   /*=====================================================================*/


SPF_NAMESPACE_END
